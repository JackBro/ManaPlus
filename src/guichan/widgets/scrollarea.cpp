/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naess�n and Per Larsson
 * Copyright (C) 2011-2012  The ManaPlus Developers
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess�n a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/widgets/scrollarea.hpp"

#include "guichan/exception.hpp"
#include "guichan/graphics.hpp"

#include "debug.h"

namespace gcn
{
    ScrollArea::ScrollArea() :
        BasicContainer(),
        MouseListener(),
        mVScroll(0),
        mHScroll(0),
        mScrollbarWidth(12),
        mHPolicy(SHOW_AUTO),
        mVPolicy(SHOW_AUTO),
        mVBarVisible(false),
        mHBarVisible(false),
        mUpButtonPressed(false),
        mDownButtonPressed(false),
        mLeftButtonPressed(false),
        mRightButtonPressed(false),
        mUpButtonScrollAmount(10),
        mDownButtonScrollAmount(10),
        mLeftButtonScrollAmount(10),
        mRightButtonScrollAmount(10),
        mIsVerticalMarkerDragged(false),
        mIsHorizontalMarkerDragged(false),
        mHorizontalMarkerDragOffset(0),
        mVerticalMarkerDragOffset(0),
        mOpaque(true)
    {
        addMouseListener(this);
    }

    ScrollArea::ScrollArea(Widget *const content) :
        BasicContainer(),
        MouseListener(),
        mVScroll(0),
        mHScroll(0),
        mScrollbarWidth(12),
        mHPolicy(SHOW_AUTO),
        mVPolicy(SHOW_AUTO),
        mVBarVisible(false),
        mHBarVisible(false),
        mUpButtonPressed(false),
        mDownButtonPressed(false),
        mLeftButtonPressed(false),
        mRightButtonPressed(false),
        mUpButtonScrollAmount(10),
        mDownButtonScrollAmount(10),
        mLeftButtonScrollAmount(10),
        mRightButtonScrollAmount(10),
        mIsVerticalMarkerDragged(false),
        mIsHorizontalMarkerDragged(false),
        mHorizontalMarkerDragOffset(0),
        mVerticalMarkerDragOffset(0),
        mOpaque(true)
    {
        setContent(content);
        addMouseListener(this);
    }

    ScrollArea::ScrollArea(Widget *content,
                           ScrollPolicy hPolicy,
                           ScrollPolicy vPolicy) :
        BasicContainer(),
        MouseListener(),
        mVScroll(0),
        mHScroll(0),
        mScrollbarWidth(12),
        mHPolicy(hPolicy),
        mVPolicy(vPolicy),
        mVBarVisible(false),
        mHBarVisible(false),
        mUpButtonPressed(false),
        mDownButtonPressed(false),
        mLeftButtonPressed(false),
        mRightButtonPressed(false),
        mUpButtonScrollAmount(10),
        mDownButtonScrollAmount(10),
        mLeftButtonScrollAmount(10),
        mRightButtonScrollAmount(10),
        mIsVerticalMarkerDragged(false),
        mIsHorizontalMarkerDragged(false),
        mHorizontalMarkerDragOffset(0),
        mVerticalMarkerDragOffset(0),
        mOpaque(true)
    {
        setContent(content);
        addMouseListener(this);
    }

    ScrollArea::~ScrollArea()
    {
        setContent(nullptr);
    }

    void ScrollArea::setContent(Widget* widget)
    {
        if (widget)
        {
            clear();
            add(widget);
            widget->setPosition(0, 0);
        }
        else
        {
            clear();
        }

        checkPolicies();
    }

    Widget* ScrollArea::getContent()
    {
        if (!mWidgets.empty())
            return *mWidgets.begin();

        return nullptr;
    }

    void ScrollArea::setHorizontalScrollPolicy(ScrollPolicy hPolicy)
    {
        mHPolicy = hPolicy;
        checkPolicies();
    }

    ScrollArea::ScrollPolicy ScrollArea::getHorizontalScrollPolicy() const
    {
        return mHPolicy;
    }

    void ScrollArea::setVerticalScrollPolicy(ScrollPolicy vPolicy)
    {
        mVPolicy = vPolicy;
        checkPolicies();
    }

    ScrollArea::ScrollPolicy ScrollArea::getVerticalScrollPolicy() const
    {
        return mVPolicy;
    }

    void ScrollArea::setScrollPolicy(ScrollPolicy hPolicy,
                                     ScrollPolicy vPolicy)
    {
        mHPolicy = hPolicy;
        mVPolicy = vPolicy;
        checkPolicies();
    }

    void ScrollArea::setVerticalScrollAmount(int vScroll)
    {
        const int max = getVerticalMaxScroll();

        mVScroll = vScroll;

        if (vScroll > max)
            mVScroll = max;

        if (vScroll < 0)
            mVScroll = 0;
    }

    int ScrollArea::getVerticalScrollAmount() const
    {
        return mVScroll;
    }

    void ScrollArea::setHorizontalScrollAmount(int hScroll)
    {
        const int max = getHorizontalMaxScroll();

        mHScroll = hScroll;

        if (hScroll > max)
            mHScroll = max;
        else if (hScroll < 0)
            mHScroll = 0;
    }

    int ScrollArea::getHorizontalScrollAmount() const
    {
        return mHScroll;
    }

    void ScrollArea::setScrollAmount(int hScroll, int vScroll)
    {
        setHorizontalScrollAmount(hScroll);
        setVerticalScrollAmount(vScroll);
    }

    int ScrollArea::getHorizontalMaxScroll()
    {
        checkPolicies();

        if (!getContent())
            return 0;

        const int value = getContent()->getWidth() - getChildrenArea().width +
            2 * getContent()->getFrameSize();

        if (value < 0)
            return 0;

        return value;
    }

    int ScrollArea::getVerticalMaxScroll()
    {
        checkPolicies();

        if (!getContent())
            return 0;

        int value;

        value = getContent()->getHeight() - getChildrenArea().height +
            2 * getContent()->getFrameSize();

        if (value < 0)
            return 0;

        return value;
    }

    void ScrollArea::setScrollbarWidth(int width)
    {
        if (width > 0)
            mScrollbarWidth = width;
        else
            throw GCN_EXCEPTION("Width should be greater then 0.");
    }

    int ScrollArea::getScrollbarWidth() const
    {
        return mScrollbarWidth;
    }

    void ScrollArea::mousePressed(MouseEvent& mouseEvent)
    {
        const int x = mouseEvent.getX();
        const int y = mouseEvent.getY();

        if (getUpButtonDimension().isPointInRect(x, y))
        {
            setVerticalScrollAmount(getVerticalScrollAmount()
                                    - mUpButtonScrollAmount);
            mUpButtonPressed = true;
        }
        else if (getDownButtonDimension().isPointInRect(x, y))
        {
            setVerticalScrollAmount(getVerticalScrollAmount()
                                    + mDownButtonScrollAmount);
            mDownButtonPressed = true;
        }
        else if (getLeftButtonDimension().isPointInRect(x, y))
        {
            setHorizontalScrollAmount(getHorizontalScrollAmount()
                                      - mLeftButtonScrollAmount);
            mLeftButtonPressed = true;
        }
        else if (getRightButtonDimension().isPointInRect(x, y))
        {
            setHorizontalScrollAmount(getHorizontalScrollAmount()
                                      + mRightButtonScrollAmount);
            mRightButtonPressed = true;
        }
        else if (getVerticalMarkerDimension().isPointInRect(x, y))
        {
            mIsHorizontalMarkerDragged = false;
            mIsVerticalMarkerDragged = true;

            mVerticalMarkerDragOffset = y - getVerticalMarkerDimension().y;
        }
        else if (getVerticalBarDimension().isPointInRect(x, y))
        {
            if (y < getVerticalMarkerDimension().y)
            {
                setVerticalScrollAmount(getVerticalScrollAmount()
                    - static_cast<int>(getChildrenArea().height * 0.95));
            }
            else
            {
                setVerticalScrollAmount(getVerticalScrollAmount()
                    + static_cast<int>(getChildrenArea().height * 0.95));
            }
        }
        else if (getHorizontalMarkerDimension().isPointInRect(x, y))
        {
            mIsHorizontalMarkerDragged = true;
            mIsVerticalMarkerDragged = false;

            mHorizontalMarkerDragOffset = x - getHorizontalMarkerDimension().x;
        }
        else if (getHorizontalBarDimension().isPointInRect(x, y))
        {
            if (x < getHorizontalMarkerDimension().x)
            {
                setHorizontalScrollAmount(getHorizontalScrollAmount()
                    - static_cast<int>(getChildrenArea().width * 0.95));
            }
            else
            {
                setHorizontalScrollAmount(getHorizontalScrollAmount()
                    + static_cast<int>(getChildrenArea().width * 0.95));
            }
        }
    }

    void ScrollArea::mouseReleased(MouseEvent& mouseEvent)
    {
        mUpButtonPressed = false;
        mDownButtonPressed = false;
        mLeftButtonPressed = false;
        mRightButtonPressed = false;
        mIsHorizontalMarkerDragged = false;
        mIsVerticalMarkerDragged = false;

        mouseEvent.consume();
    }

    void ScrollArea::mouseDragged(MouseEvent& mouseEvent)
    {
        if (mIsVerticalMarkerDragged)
        {
            const Rectangle barDim = getVerticalBarDimension();

            const int pos = mouseEvent.getY() - barDim.y
                 - mVerticalMarkerDragOffset;
            const int length = getVerticalMarkerDimension().height;

            if ((barDim.height - length) > 0)
            {
                setVerticalScrollAmount((getVerticalMaxScroll() * pos)
                                         / (barDim.height - length));
            }
            else
            {
                setVerticalScrollAmount(0);
            }
        }

        if (mIsHorizontalMarkerDragged)
        {
            const Rectangle barDim = getHorizontalBarDimension();

            const int pos = mouseEvent.getX() - barDim.x 
                - mHorizontalMarkerDragOffset;
            const int length = getHorizontalMarkerDimension().width;

            if ((barDim.width - length) > 0)
            {
                setHorizontalScrollAmount((getHorizontalMaxScroll() * pos)
                                          / (barDim.width - length));
            }
            else
            {
                setHorizontalScrollAmount(0);
            }
        }

        mouseEvent.consume();
    }

    void ScrollArea::draw(Graphics *graphics A_UNUSED)
    {
    }

    void ScrollArea::drawHBar(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawVBar(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawBackground(Graphics *graphics A_UNUSED)
    {
    }

    void ScrollArea::drawUpButton(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawDownButton(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawLeftButton(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawRightButton(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawVMarker(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawHMarker(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::logic()
    {
        checkPolicies();

        setVerticalScrollAmount(getVerticalScrollAmount());
        setHorizontalScrollAmount(getHorizontalScrollAmount());

        if (getContent())
        {
            getContent()->setPosition(-mHScroll + getContent()->getFrameSize(),
                -mVScroll + getContent()->getFrameSize());
            getContent()->logic();
        }
    }

    void ScrollArea::checkPolicies()
    {
        const int w = getWidth();
        const int h = getHeight();

        mHBarVisible = false;
        mVBarVisible = false;

        if (!getContent())
        {
            mHBarVisible = (mHPolicy == SHOW_ALWAYS);
            mVBarVisible = (mVPolicy == SHOW_ALWAYS);
            return;
        }

        if (mHPolicy == SHOW_AUTO &&
            mVPolicy == SHOW_AUTO)
        {
            if (getContent()->getWidth() <= w
                && getContent()->getHeight() <= h)
            {
                mHBarVisible = false;
                mVBarVisible = false;
            }

            if (getContent()->getWidth() > w)
            {
                mHBarVisible = true;
            }

            if ((getContent()->getHeight() > h)
                || (mHBarVisible && getContent()->getHeight()
                > h - mScrollbarWidth))
            {
                mVBarVisible = true;
            }

            if (mVBarVisible && getContent()->getWidth() > w - mScrollbarWidth)
                mHBarVisible = true;

            return;
        }

        switch (mHPolicy)
        {
            case SHOW_NEVER:
                mHBarVisible = false;
                break;

            case SHOW_ALWAYS:
                mHBarVisible = true;
                break;

            case SHOW_AUTO:
                if (mVPolicy == SHOW_NEVER)
                {
                    mHBarVisible = (getContent()->getWidth() > w);
                }
                else // (mVPolicy == SHOW_ALWAYS)
                {
                    mHBarVisible = (getContent()->getWidth()
                        > w - mScrollbarWidth);
                }
                break;

            default:
                throw GCN_EXCEPTION("Horizontal scroll policy invalid.");
        }

        switch (mVPolicy)
        {
            case SHOW_NEVER:
                mVBarVisible = false;
                break;

            case SHOW_ALWAYS:
                mVBarVisible = true;
                break;

            case SHOW_AUTO:
                if (mHPolicy == SHOW_NEVER)
                {
                    mVBarVisible = (getContent()->getHeight() > h);
                }
                else // (mHPolicy == SHOW_ALWAYS)
                {
                    mVBarVisible = (getContent()->getHeight()
                        > h - mScrollbarWidth);
                }
                break;
            default:
                throw GCN_EXCEPTION("Vertical scroll policy invalid.");
        }
    }

    Rectangle ScrollArea::getUpButtonDimension() const
    {
        if (!mVBarVisible)
            return Rectangle(0, 0, 0, 0);

        return Rectangle(getWidth() - mScrollbarWidth, 0,
            mScrollbarWidth, mScrollbarWidth);
    }

    Rectangle ScrollArea::getDownButtonDimension() const
    {
        if (!mVBarVisible)
            return Rectangle(0, 0, 0, 0);

        if (mVBarVisible && mHBarVisible)
        {
            return Rectangle(getWidth() - mScrollbarWidth,
                             getHeight() - mScrollbarWidth*2,
                             mScrollbarWidth,
                             mScrollbarWidth);
        }

        return Rectangle(getWidth() - mScrollbarWidth,
                         getHeight() - mScrollbarWidth,
                         mScrollbarWidth,
                         mScrollbarWidth);
    }

    Rectangle ScrollArea::getLeftButtonDimension() const
    {
        if (!mHBarVisible)
            return Rectangle(0, 0, 0, 0);

        return Rectangle(0, getHeight() - mScrollbarWidth,
            mScrollbarWidth, mScrollbarWidth);
    }

    Rectangle ScrollArea::getRightButtonDimension() const
    {
        if (!mHBarVisible)
            return Rectangle(0, 0, 0, 0);

        if (mVBarVisible && mHBarVisible)
        {
            return Rectangle(getWidth() - mScrollbarWidth*2,
                             getHeight() - mScrollbarWidth,
                             mScrollbarWidth,
                             mScrollbarWidth);
        }

        return Rectangle(getWidth() - mScrollbarWidth,
                         getHeight() - mScrollbarWidth,
                         mScrollbarWidth,
                         mScrollbarWidth);
    }

    Rectangle ScrollArea::getChildrenArea()
    {
        const Rectangle area = Rectangle(0, 0,
            mVBarVisible ? (getWidth() - mScrollbarWidth) : getWidth(),
            mHBarVisible ? (getHeight() - mScrollbarWidth) : getHeight()); 

        if (area.width < 0 || area.height < 0)
            return Rectangle();

        return area;
    }

    Rectangle ScrollArea::getVerticalBarDimension() const
    {
        if (!mVBarVisible)
            return Rectangle(0, 0, 0, 0);

        if (mHBarVisible)
        {
            return Rectangle(getWidth() - mScrollbarWidth,
                             getUpButtonDimension().height,
                             mScrollbarWidth,
                             getHeight()
                             - getUpButtonDimension().height
                             - getDownButtonDimension().height
                             - mScrollbarWidth);
        }

        return Rectangle(getWidth() - mScrollbarWidth,
                         getUpButtonDimension().height,
                         mScrollbarWidth,
                         getHeight()
                         - getUpButtonDimension().height
                         - getDownButtonDimension().height);
    }

    Rectangle ScrollArea::getHorizontalBarDimension() const
    {
        if (!mHBarVisible)
            return Rectangle(0, 0, 0, 0);

        if (mVBarVisible)
        {
            return Rectangle(getLeftButtonDimension().width,
                             getHeight() - mScrollbarWidth,
                             getWidth()
                             - getLeftButtonDimension().width
                             - getRightButtonDimension().width
                             - mScrollbarWidth,
                             mScrollbarWidth);
        }

        return Rectangle(getLeftButtonDimension().width,
                         getHeight() - mScrollbarWidth,
                         getWidth()
                         - getLeftButtonDimension().width
                         - getRightButtonDimension().width,
                         mScrollbarWidth);
    }

    Rectangle ScrollArea::getVerticalMarkerDimension()
    {
        if (!mVBarVisible)
            return Rectangle(0, 0, 0, 0);

        int length, pos;
        int height;

        if (mHBarVisible)
        {
            height = getHeight() - getUpButtonDimension().height
                - getDownButtonDimension().height - mScrollbarWidth;
        }
        else
        {
            height = getHeight() - getUpButtonDimension().height
                - getDownButtonDimension().height;
        }

        if (getContent() && getContent()->getHeight() != 0)
        {
            length = (height * getChildrenArea().height)
                / getContent()->getHeight();
        }
        else
        {
            length = height;
        }

        if (length < mScrollbarWidth)
            length = mScrollbarWidth;

        if (length > height)
            length = height;

        if (getVerticalMaxScroll() != 0)
        {
            pos = ((height - length) * getVerticalScrollAmount())
                / getVerticalMaxScroll();
        }
        else
        {
            pos = 0;
        }

        return Rectangle(getWidth() - mScrollbarWidth,
            getUpButtonDimension().height + pos, mScrollbarWidth, length);
    }

    Rectangle ScrollArea::getHorizontalMarkerDimension()
    {
        if (!mHBarVisible)
            return Rectangle(0, 0, 0, 0);

        int length, pos;
        int width;

        if (mVBarVisible)
        {
            width = getWidth() - getLeftButtonDimension().width
                - getRightButtonDimension().width - mScrollbarWidth;
        }
        else
        {
            width = getWidth() - getLeftButtonDimension().width
                - getRightButtonDimension().width;
        }

        if (getContent() && getContent()->getWidth() != 0)
        {
            length = (width * getChildrenArea().width)
                / getContent()->getWidth();
        }
        else
        {
            length = width;
        }

        if (length < mScrollbarWidth)
            length = mScrollbarWidth;

        if (length > width)
            length = width;

        if (getHorizontalMaxScroll() != 0)
        {
            pos = ((width - length) * getHorizontalScrollAmount())
                / getHorizontalMaxScroll();
        }
        else
        {
            pos = 0;
        }

        return Rectangle(getLeftButtonDimension().width + pos,
            getHeight() - mScrollbarWidth, length, mScrollbarWidth);
    }

    void ScrollArea::showWidgetPart(Widget* widget, Rectangle area)
    {
        if (widget != getContent())
            throw GCN_EXCEPTION("Widget not content widget");

        BasicContainer::showWidgetPart(widget, area);

        setHorizontalScrollAmount(getContent()->getFrameSize()
            - getContent()->getX());
        setVerticalScrollAmount(getContent()->getFrameSize()
            - getContent()->getY());
    }

    Widget *ScrollArea::getWidgetAt(int x, int y)
    {
        if (getChildrenArea().isPointInRect(x, y))
            return getContent();

        return nullptr;
    }

    void ScrollArea::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed())
            return;

        setVerticalScrollAmount(getVerticalScrollAmount()
            - getChildrenArea().height / 8);

        mouseEvent.consume();
    }

    void ScrollArea::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed())
            return;

        setVerticalScrollAmount(getVerticalScrollAmount()
            + getChildrenArea().height / 8);

        mouseEvent.consume();
    }

    void ScrollArea::setWidth(int width)
    {
        Widget::setWidth(width);
        checkPolicies();
    }

    void ScrollArea::setHeight(int height)
    {
        Widget::setHeight(height);
        checkPolicies();
    }

    void ScrollArea::setDimension(const Rectangle& dimension)
    {
        Widget::setDimension(dimension);
        checkPolicies();
    }

    void ScrollArea::setLeftButtonScrollAmount(int amount)
    {
        mLeftButtonScrollAmount = amount;
    }

    void ScrollArea::setRightButtonScrollAmount(int amount)
    {
        mRightButtonScrollAmount = amount;
    }

    void ScrollArea::setUpButtonScrollAmount(int amount)
    {
        mUpButtonScrollAmount = amount;
    }

    void ScrollArea::setDownButtonScrollAmount(int amount)
    {
        mDownButtonScrollAmount = amount;
    }

    int ScrollArea::getLeftButtonScrollAmount() const
    {
        return mLeftButtonScrollAmount;
    }

    int ScrollArea::getRightButtonScrollAmount() const
    {
        return mRightButtonScrollAmount;
    }

    int ScrollArea::getUpButtonScrollAmount() const
    {
        return mUpButtonScrollAmount;
    }

    int ScrollArea::getDownButtonScrollAmount() const
    {
        return mDownButtonScrollAmount;
    }

    void ScrollArea::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    
    bool ScrollArea::isOpaque() const
    {
        return mOpaque;
    }
}

/*
 * Wow! This is a looooong source file.
 */
