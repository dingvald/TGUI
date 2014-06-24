/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012-2014 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_LAYOUT_HPP
#define TGUI_LAYOUT_HPP

#include <memory>

#include <TGUI/SharedWidgetPtr.hpp>

/// TODO: Add documentation.
/// WARNING: File still under heavy development

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    class Widget;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API LayoutBind
    {
    public:

        enum class Param
        {
            X,
            Y
        };

        enum class Trigger
        {
            PositionChanged,
            SizeChanged
        };

        LayoutBind(const SharedWidgetPtr<Widget>& widget, Param param, float fraction = 1);

        void bind(Trigger trigger);

        float getValue() const;

    protected:
        SharedWidgetPtr<Widget> m_widget;
        float m_fraction;

        std::function<sf::Vector2f()> m_getter;
        Param m_param;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API Layout
    {
    public:

        enum class Operator
        {
            Add,
            Subtract,
            Multiply,
            Divide
        };

        Layout(float value) : m_value{value} {}

        explicit Layout(const std::shared_ptr<LayoutBind>& layout, LayoutBind::Trigger trigger);

        friend Layout operator+(const Layout& left, const Layout& right);
        friend Layout operator-(const Layout& left, const Layout& right);
        friend Layout operator*(const Layout& left, const Layout& right);
        friend Layout operator/(const Layout& left, const Layout& right);

    private:

        void recalculateResult();

    private:
        std::list<std::shared_ptr<LayoutBind>> m_bindings;
        std::list<Operator> m_operators;
        float m_value;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API Layout2d
    {
    public:

        Layout2d(const sf::Vector2f& value) : x(value.x), y(value.y) {}

        Layout2d(const Layout& valueX, const Layout& valueY) : x(valueX), y(valueY) {}


        Layout x;
        Layout y;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Layout bindLeft(const SharedWidgetPtr<Widget>& widget, float fraction = 1);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Layout bindTop(const SharedWidgetPtr<Widget>& widget, float fraction = 1);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Layout bindRight(const SharedWidgetPtr<Widget>& widget, float fraction = 1);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Layout bindBottom(const SharedWidgetPtr<Widget>& widget, float fraction = 1);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Layout bindWidth(const SharedWidgetPtr<Widget>& widget, float fraction = 1);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Layout bindHeight(const SharedWidgetPtr<Widget>& widget, float fraction = 1);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Layout2d bindPosition(const SharedWidgetPtr<Widget>& widget, const sf::Vector2f& fraction = {1,1});

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Layout2d bindSize(const SharedWidgetPtr<Widget>& widget, const sf::Vector2f& fraction = {1,1});

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_LAYOUT_HPP