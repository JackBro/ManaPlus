/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef USE_SDL2

#include "resources/sdlimagehelper.h"

#include "resources/dye/dye.h"
#include "resources/dye/dyepalette.h"

#include "resources/image/image.h"

#include "utils/checkutils.h"
#include "utils/sdlcheckutils.h"

#include <SDL_gfxBlitFunc.h>
#include <SDL_image.h>

#include "debug.h"

#ifndef SDL_BIG_ENDIAN
#error missing SDL_endian.h
#endif  // SDL_BYTEORDER

bool SDLImageHelper::mEnableAlphaCache = false;

Image *SDLImageHelper::load(SDL_RWops *const rw, Dye const &dye)
{
    SDL_Surface *const tmpImage = loadPng(rw);
    if (!tmpImage)
    {
        reportAlways("Error, image load failed: %s",
            IMG_GetError());
        return nullptr;
    }

    SDL_PixelFormat rgba;
    rgba.palette = nullptr;
    rgba.BitsPerPixel = 32;
    rgba.BytesPerPixel = 4;
    rgba.colorkey = 0;
    rgba.alpha = 255;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rgba.Rmask = 0x000000FF;
    rgba.Gmask = 0x0000FF00;
    rgba.Bmask = 0x00FF0000;
    rgba.Amask = 0xFF000000;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    rgba.Rmask = 0xFF000000;
    rgba.Gmask = 0x00FF0000;
    rgba.Bmask = 0x0000FF00;
    rgba.Amask = 0x000000FF;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    SDL_Surface *const surf = MSDL_ConvertSurface(
        tmpImage, &rgba, SDL_SWSURFACE);
    MSDL_FreeSurface(tmpImage);
    if (!surf)
        return nullptr;

    uint32_t *pixels = static_cast<uint32_t *>(surf->pixels);
    const int type = dye.getType();

    switch (type)
    {
        case 1:
        {
            const DyePalette *const pal = dye.getSPalete();
            if (pal)
                pal->replaceSColor(pixels, surf->w * surf->h);
            break;
        }
        case 2:
        {
            const DyePalette *const pal = dye.getAPalete();
            if (pal)
                pal->replaceAColor(pixels, surf->w * surf->h);
            break;
        }
        case 0:
        default:
        {
            dye.normalDye(pixels, surf->w * surf->h);
            break;
        }
    }

    Image *const image = loadSurface(surf);
    MSDL_FreeSurface(surf);
    return image;
}

Image *SDLImageHelper::loadSurface(SDL_Surface *const tmpImage)
{
    return _SDLload(tmpImage);
}

Image *SDLImageHelper::createTextSurface(SDL_Surface *const tmpImage,
                                         const int width A_UNUSED,
                                         const int height A_UNUSED,
                                         const float alpha)
{
    if (!tmpImage)
        return nullptr;

    bool hasAlpha = false;
    const size_t sz = tmpImage->w * tmpImage->h;

    // The alpha channel to be filled with alpha values
    uint8_t *alphaChannel = new uint8_t[sz];

    const SDL_PixelFormat *const fmt = tmpImage->format;
    if (fmt->Amask)
    {
        for (size_t i = 0; i < sz; ++ i)
        {
            uint32_t c = (static_cast<uint32_t*>(tmpImage->pixels))[i];

            const unsigned v = (c & fmt->Amask) >> fmt->Ashift;
            const uint8_t a = static_cast<const uint8_t>((v << fmt->Aloss)
                + (v >> (8 - (fmt->Aloss << 1))));

            const uint8_t a2 = CAST_U8(
                static_cast<float>(a) * alpha);

            c &= ~fmt->Amask;
            c |= ((a2 >> fmt->Aloss) << fmt->Ashift & fmt->Amask);
            (static_cast<uint32_t*>(tmpImage->pixels))[i] = c;

            if (a != 255)
                hasAlpha = true;

            alphaChannel[i] = a;
        }
    }

    SDL_Surface *image;

    // Convert the surface to the current display format
    if (hasAlpha)
    {
        image = MSDL_DisplayFormatAlpha(tmpImage);
    }
    else
    {
        image = MSDL_DisplayFormat(tmpImage);

        // We also delete the alpha channel since
        // it's not used.
        delete [] alphaChannel;
        alphaChannel = nullptr;
    }

    if (!image)
    {
        reportAlways("Error: Image convert failed.");
        delete [] alphaChannel;
        return nullptr;
    }

    Image *const img = new Image(image, hasAlpha, alphaChannel);
    img->mAlpha = alpha;
    return img;
}

SDL_Surface* SDLImageHelper::SDLDuplicateSurface(SDL_Surface *const tmpImage)
{
    if (!tmpImage || !tmpImage->format)
        return nullptr;

    return MSDL_ConvertSurface(tmpImage, tmpImage->format, SDL_SWSURFACE);
}

Image *SDLImageHelper::_SDLload(SDL_Surface *tmpImage)
{
    if (!tmpImage)
        return nullptr;

    bool hasAlpha = false;
    bool converted = false;

    if (tmpImage->format->BitsPerPixel != 32)
    {
        tmpImage = convertTo32Bit(tmpImage);

        if (!tmpImage)
            return nullptr;
        converted = true;
    }

    const size_t sz = tmpImage->w * tmpImage->h;

    // The alpha channel to be filled with alpha values
    uint8_t *alphaChannel = new uint8_t[sz];

    // Figure out whether the image uses its alpha layer
    if (!tmpImage->format->palette)
    {
        const SDL_PixelFormat *const fmt = tmpImage->format;
        if (fmt->Amask)
        {
            const uint32_t amask = fmt->Amask;
            const uint8_t ashift = fmt->Ashift;
            const uint8_t aloss = fmt->Aloss;
            const uint32_t *pixels = static_cast<uint32_t*>(tmpImage->pixels);
            cilk_for (size_t i = 0; i < sz; ++ i)
            {
                const unsigned v = (pixels[i] & amask) >> ashift;
                const uint8_t a = static_cast<const uint8_t>((v << aloss)
                    + (v >> (8 - (aloss << 1))));

                if (a != 255)
                    hasAlpha = true;

                alphaChannel[i] = a;
            }
        }
        else
        {
            if (SDL_ALPHA_OPAQUE != 255)
            {
                hasAlpha = true;
                memset(alphaChannel, SDL_ALPHA_OPAQUE, sz);
            }
        }
    }
    else
    {
        if (SDL_ALPHA_OPAQUE != 255)
        {
            hasAlpha = true;
            memset(alphaChannel, SDL_ALPHA_OPAQUE, sz);
        }
    }

    SDL_Surface *image;

    // Convert the surface to the current display format
    if (hasAlpha)
    {
        image = MSDL_DisplayFormatAlpha(tmpImage);
    }
    else
    {
        image = MSDL_DisplayFormat(tmpImage);

        // We also delete the alpha channel since
        // it's not used.
        delete [] alphaChannel;
        alphaChannel = nullptr;
    }

    if (!image)
    {
        reportAlways("Error: Image convert failed.");
        delete [] alphaChannel;
        return nullptr;
    }

    if (converted)
        MSDL_FreeSurface(tmpImage);
    return new Image(image, hasAlpha, alphaChannel);
}

int SDLImageHelper::combineSurface(SDL_Surface *restrict const src,
                                   SDL_Rect *restrict const srcrect,
                                   SDL_Surface *restrict const dst,
                                   SDL_Rect *restrict const dstrect)
{
    return SDL_gfxBlitRGBA(src, srcrect, dst, dstrect);
}

void SDLImageHelper::copySurfaceToImage(const Image *const image,
                                        const int x, const int y,
                                        SDL_Surface *const surface) const
{
    if (!image || !surface)
        return;

    SDL_SetAlpha(surface, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect rect =
    {
        CAST_S16(x), CAST_S16(y),
        CAST_U16(surface->w), static_cast<uint16_t>(surface->h)
    };

    SDL_BlitSurface(surface, nullptr, image->mSDLSurface, &rect);
}

#endif  // USE_SDL2
