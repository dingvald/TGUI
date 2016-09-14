/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2016 Bruno Van de Velde (vdv_b@tgui.eu)
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


#ifndef TGUI_KNOB_HPP
#define TGUI_KNOB_HPP


#include <TGUI/Widget.hpp>
#include <TGUI/Renderers/KnobRenderer.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Knob widget
    ///
    /// Signals:
    ///     - ValueChanged
    ///         * Optional parameter int: new value of the knob
    ///         * Uses Callback member 'value'
    ///
    ///     - Inherited signals from Widget
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API Knob : public Widget
    {
    public:

        typedef std::shared_ptr<Knob> Ptr; ///< Shared widget pointer
        typedef std::shared_ptr<const Knob> ConstPtr; ///< Shared constant widget pointer


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Knob();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Creates a new knob widget
        ///
        /// @return The new knob
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static Knob::Ptr create();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of another knob
        ///
        /// @param knob  The other knob
        ///
        /// @return The new knob
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static Knob::Ptr copy(Knob::ConstPtr knob);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the renderer, which gives access to functions that determine how the widget is displayed
        ///
        /// @return Pointer to the renderer
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        KnobRenderer* getRenderer() const
        {
            return aurora::downcast<KnobRenderer*>(m_renderer.get());
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the size of the knob
        ///
        /// @param size  The new size of the knob
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setSize(const Layout2d& size) override;
        using Transformable::setSize;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the start rotation, which is the place where the value should be minimal
        ///
        /// @param startRotation  New start rotation
        ///
        /// The rotation is a number in the interval [0,360[, for which 0 to to the right and the rotation goes counter-clockwise.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setStartRotation(float startRotation);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the end rotation, which is the place where the value should be maximal
        ///
        /// @param endRotation  New end rotation
        ///
        /// The rotation is a number in the interval [0,360[, for which 0 to to the right and the rotation goes counter-clockwise.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setEndRotation(float endRotation);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the start rotation, which is the place where the value should be minimal
        ///
        /// @return number in the interval [0,360[, for which 0 to to the right and the rotation goes counter-clockwise
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getStartRotation()
        {
            return m_startRotation;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the end rotation, which is the place where the value should be maximal
        ///
        /// @return number in the interval [0,360[, for which 0 to to the right and the rotation goes counter-clockwise
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getEndRotation()
        {
            return m_endRotation;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the value for when the knob would be rotated in the direction of StartRotation
        ///
        /// @param minimum  The new minimum value
        ///
        /// The knob will be rotated to keep representing the value correctly.
        ///
        /// When the value is too small then it will be changed to this minimum.
        /// When the maximum value is lower than the new minimum then it will be changed to this new minimum value.
        /// The default minimum value is 0.
        ///
        /// @see setStartRotation
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setMinimum(int minimum);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the value for when the knob would be rotated in the direction of EndRotation
        ///
        /// @param maximum  The new maximum value
        ///
        /// The knob will be rotated to keep representing the value correctly.
        ///
        /// When the value is too big then it will be changed to this maximum.
        /// When the minimum value is higher than the new maximum then it will be changed to this new maximum value.
        /// The default maximum value is 360.
        ///
        /// @see setEndRotation
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setMaximum(int maximum);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the current value
        ///
        /// @param value  The new value
        ///
        /// The knob will be rotated to correctly represent this new value.
        ///
        /// The value can't be smaller than the minimum or bigger than the maximum.
        /// The default value is 0.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setValue(int value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the value when the knob would be rotated in the direction of StartRotation
        ///
        /// @return The current minimum value
        ///
        /// The default minimum value is 0.
        ///
        /// @see getStartRotation
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int getMinimum() const
        {
            return m_minimum;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the value when the knob would be rotated in the direction of EndRotation
        ///
        /// @return The current maximum value
        ///
        /// The default maximum value is 360.
        ///
        /// @see getEndRotation
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int getMaximum() const
        {
            return m_maximum;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the current value
        ///
        /// @return The current value, determined by the Minimum, Maximum, StartRotation and EndRotation
        ///
        /// The default value is 0.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int getValue() const
        {
            return m_value;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Should the value increase when turning the knob clockwise?
        ///
        /// @param clockwise  Does the value increase when turning clockwise?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setClockwiseTurning(bool clockwise);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns whether the value increases when turning the knob clockwise?
        ///
        /// @return Does the value increase when turning clockwise?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool getClockwiseTurning()
        {
            return m_clockwiseTurning;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool mouseOnWidget(sf::Vector2f pos) const override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void leftMousePressed(sf::Vector2f pos) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void leftMouseReleased(sf::Vector2f pos) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseMoved(sf::Vector2f pos) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void widgetFocused() override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Function called when one of the properties of the renderer is changed
        ///
        /// @param property  Lowercase name of the property that was changed
        /// @param value     New value of the property
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void rendererChanged(const std::string& property, ObjectConverter& value) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Returns the size without the borders
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::Vector2f getInnerSize() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Recalculates the rotation of the knob.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void recalculateRotation();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draws the widget on the render target.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Widget::Ptr clone() const override
        {
            return std::make_shared<Knob>(*this);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        bool m_clockwiseTurning = true; // Does rotating clockwise increment the value?
        float m_startRotation = 270;
        float m_endRotation = 270;

        int m_minimum = 0;
        int m_value = 0;
        int m_maximum = 360;

        float m_angle = 270;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_KNOB_HPP

