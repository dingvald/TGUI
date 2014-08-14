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


#include <TGUI/TGUI.hpp>

#include <stack>
#include <cassert>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Container::Container()
    {
        m_containerWidget = true;
        m_animatedWidget = true;
        m_allowFocus = true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Container::Container(const Container& containerToCopy) :
        Widget                   {containerToCopy},
        m_focusedWidget          {0},
        m_globalFont             {containerToCopy.m_globalFont},
        m_fontPtr                {containerToCopy.m_fontPtr},
        m_globalCallbackFunctions(containerToCopy.m_globalCallbackFunctions) // Did not compile on mingw 4.7 when using braces
    {
        if (m_fontPtr == &containerToCopy.m_globalFont)
            m_fontPtr = &m_globalFont;

        // Copy all the widgets
        for (unsigned int i = 0; i < containerToCopy.m_widgets.size(); ++i)
        {
            m_widgets.push_back(containerToCopy.m_widgets[i]->clone());
            m_objName.push_back(containerToCopy.m_objName[i]);

            m_widgets.back()->initialize(this);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Container& Container::operator= (const Container& right)
    {
        // Make sure it is not the same widget
        if (this != &right)
        {
            Widget::operator=(right);

            // Copy the font and the callback functions
            m_focusedWidget = 0;
            m_globalFont = right.m_globalFont;
            m_globalCallbackFunctions = right.m_globalCallbackFunctions;

            if (right.m_fontPtr == &right.m_globalFont)
                m_fontPtr = &m_globalFont;
            else
                m_fontPtr = right.m_fontPtr;

            // Remove all the old widgets
            removeAllWidgets();

            // Copy all the widgets
            for (unsigned int i = 0; i < right.m_widgets.size(); ++i)
            {
                m_widgets.push_back(right.m_widgets[i]->clone());
                m_objName.push_back(right.m_objName[i]);

                m_widgets.back()->initialize(this);
            }
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::setGlobalFont(const std::string& filename)
    {
        if (m_globalFont.loadFromFile(getResourcePath() + filename))
            m_fontPtr = &m_globalFont;
        else
        {
            m_fontPtr = nullptr;
            throw Exception{"Failed to load font '" + filename + "'."};
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::setGlobalFont(const sf::Font& font)
    {
        m_globalFont = font;
        m_fontPtr = &m_globalFont;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::add(const Widget::Ptr& widgetPtr, const sf::String& widgetName)
    {
        assert(widgetPtr != nullptr);

        widgetPtr->initialize(this);
        m_widgets.push_back(widgetPtr);
        m_objName.push_back(widgetName);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Widget::Ptr Container::get(const sf::String& widgetName, bool recursive) const
    {
        for (unsigned int i = 0; i < m_objName.size(); ++i)
        {
            if (m_objName[i] == widgetName)
            {
                return m_widgets[i];
            }
            else if (recursive && m_widgets[i]->m_containerWidget)
            {
                Widget::Ptr widget = std::static_pointer_cast<Container>(m_widgets[i])->get(widgetName, true);
                if (widget != nullptr)
                    return widget;
            }
        }

        return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::remove(const Widget::Ptr& widget)
    {
        remove(widget.get());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::remove(Widget* widget)
    {
        // Loop through every widget
        for (unsigned int i = 0; i < m_widgets.size(); ++i)
        {
            // Check if the pointer matches
            if (m_widgets[i].get() == widget)
            {
                // Unfocus the widget if it was focused
                if (m_focusedWidget == i+1)
                    unfocusWidgets();

                // Change the index of the focused widget if this is needed
                else if (m_focusedWidget > i+1)
                    m_focusedWidget--;

                // Remove the widget
                m_widgets.erase(m_widgets.begin() + i);

                // Also emove the name it from the list
                m_objName.erase(m_objName.begin() + i);

                break;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::removeAllWidgets()
    {
        // Clear the lists
        m_widgets.clear();
        m_objName.clear();

        // There are no more widgets, so none of the widgets can be focused
        m_focusedWidget = 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Container::setWidgetName(const Widget::Ptr& widget, const std::string& name)
    {
        for (unsigned int i = 0; i < m_widgets.size(); ++i)
        {
            if (m_widgets[i] == widget)
            {
                m_objName[i] = name;
                return true;
            }
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Container::getWidgetName(const Widget::Ptr& widget, std::string& name) const
    {
        for (unsigned int i = 0; i < m_widgets.size(); ++i)
        {
            if (m_widgets[i] == widget)
            {
                name = m_objName[i];
                return true;
            }
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::focusWidget(const Widget::Ptr& widget)
    {
        focusWidget(widget.get());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::focusWidget(Widget *const widget)
    {
        // Loop all the widgets
        for (unsigned int i = 0; i < m_widgets.size(); ++i)
        {
            // Search for the widget that has to be focused
            if (m_widgets[i].get() == widget)
            {
                // Only continue when the widget wasn't already focused
                if (m_focusedWidget != i+1)
                {
                    // Unfocus the currently focused widget
                    if (m_focusedWidget)
                    {
                        m_widgets[m_focusedWidget-1]->m_focused = false;
                        m_widgets[m_focusedWidget-1]->widgetUnfocused();
                    }

                    // Focus the new widget
                    m_focusedWidget = i+1;
                    widget->m_focused = true;
                    widget->widgetFocused();
                }

                break;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::focusNextWidget()
    {
        // Loop all widgets behind the focused one
        for (unsigned int i = m_focusedWidget; i < m_widgets.size(); ++i)
        {
            // If you are not allowed to focus the widget, then skip it
            if (m_widgets[i]->m_allowFocus == true)
            {
                // Make sure that the widget is visible and enabled
                if ((m_widgets[i]->m_visible) && (m_widgets[i]->m_enabled))
                {
                    if (m_focusedWidget)
                    {
                        // unfocus the current widget
                        m_widgets[m_focusedWidget-1]->m_focused = false;
                        m_widgets[m_focusedWidget-1]->widgetUnfocused();
                    }

                    // Focus on the new widget
                    m_focusedWidget = i+1;
                    m_widgets[i]->m_focused = true;
                    m_widgets[i]->widgetFocused();
                    return;
                }
            }
        }

        // None of the widgets behind the focused one could be focused, so loop the ones before it
        if (m_focusedWidget)
        {
            for (unsigned int i = 0; i < m_focusedWidget - 1; ++i)
            {
                // If you are not allowed to focus the widget, then skip it
                if (m_widgets[i]->m_allowFocus == true)
                {
                    // Make sure that the widget is visible and enabled
                    if ((m_widgets[i]->m_visible) && (m_widgets[i]->m_enabled))
                    {
                        // unfocus the current widget
                        m_widgets[m_focusedWidget-1]->m_focused = false;
                        m_widgets[m_focusedWidget-1]->widgetUnfocused();

                        // Focus on the new widget
                        m_focusedWidget = i+1;
                        m_widgets[i]->m_focused = true;
                        m_widgets[i]->widgetFocused();

                        return;
                    }
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::focusPreviousWidget()
    {
        // Loop the widgets before the focused one
        if (m_focusedWidget)
        {
            for (unsigned int i = m_focusedWidget - 1; i > 0; --i)
            {
                // If you are not allowed to focus the widget, then skip it
                if (m_widgets[i-1]->m_allowFocus == true)
                {
                    // Make sure that the widget is visible and enabled
                    if ((m_widgets[i-1]->m_visible) && (m_widgets[i-1]->m_enabled))
                    {
                        // unfocus the current widget
                        m_widgets[m_focusedWidget-1]->m_focused = false;
                        m_widgets[m_focusedWidget-1]->widgetUnfocused();

                        // Focus on the new widget
                        m_focusedWidget = i;
                        m_widgets[i-1]->m_focused = true;
                        m_widgets[i-1]->widgetFocused();

                        return;
                    }
                }
            }
        }

        // None of the widgets before the focused one could be focused, so loop all widgets behind the focused one
        for (unsigned int i = m_widgets.size(); i > m_focusedWidget; --i)
        {
            // If you are not allowed to focus the widget, then skip it
            if (m_widgets[i-1]->m_allowFocus == true)
            {
                // Make sure that the widget is visible and enabled
                if ((m_widgets[i-1]->m_visible) && (m_widgets[i-1]->m_enabled))
                {
                    if (m_focusedWidget)
                    {
                        // unfocus the current widget
                        m_widgets[m_focusedWidget-1]->m_focused = false;
                        m_widgets[m_focusedWidget-1]->widgetUnfocused();
                    }

                    // Focus on the new widget
                    m_focusedWidget = i;
                    m_widgets[i-1]->m_focused = true;
                    m_widgets[i-1]->widgetFocused();
                    return;
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::unfocusWidgets()
    {
        if (m_focusedWidget)
        {
            m_widgets[m_focusedWidget-1]->m_focused = false;
            m_widgets[m_focusedWidget-1]->widgetUnfocused();
            m_focusedWidget = 0;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::uncheckRadioButtons()
    {
        // Loop through all radio buttons and uncheck them
        for (unsigned int i = 0; i < m_widgets.size(); ++i)
        {
            if (m_widgets[i]->m_callback.widgetType == Type_RadioButton)
                std::static_pointer_cast<RadioButton>(m_widgets[i])->uncheck();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::moveWidgetToFront(Widget *const widget)
    {
        // Loop through all widgets
        for (unsigned int i = 0; i < m_widgets.size(); ++i)
        {
            // Check if the widget is found
            if (m_widgets[i].get() == widget)
            {
                // Copy the widget
                m_widgets.push_back(m_widgets[i]);
                m_objName.push_back(m_objName[i]);

                // Focus the correct widget
                if ((m_focusedWidget == 0) || (m_focusedWidget == i+1))
                    m_focusedWidget = m_widgets.size()-1;
                else if (m_focusedWidget > i+1)
                    --m_focusedWidget;

                // Remove the old widget
                m_widgets.erase(m_widgets.begin() + i);
                m_objName.erase(m_objName.begin() + i);

                break;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::moveWidgetToBack(Widget *const widget)
    {
        // Loop through all widgets
        for (unsigned int i = 0; i < m_widgets.size(); ++i)
        {
            // Check if the widget is found
            if (m_widgets[i].get() == widget)
            {
                // Copy the widget
                Widget::Ptr obj = m_widgets[i];
                std::string name = m_objName[i];
                m_widgets.insert(m_widgets.begin(), obj);
                m_objName.insert(m_objName.begin(), name);

                // Focus the correct widget
                if (m_focusedWidget == i + 1)
                    m_focusedWidget = 1;
                else if (m_focusedWidget)
                    ++m_focusedWidget;

                // Remove the old widget
                m_widgets.erase(m_widgets.begin() + i + 1);
                m_objName.erase(m_objName.begin() + i + 1);

                break;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::setTransparency(unsigned char transparency)
    {
        Widget::setTransparency(transparency);

        for (unsigned int i = 0; i < m_widgets.size(); ++i)
            m_widgets[i]->setTransparency(transparency);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::bindGlobalCallback(std::function<void(const Callback&)> func)
    {
        m_globalCallbackFunctions.push_back(func);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::unbindGlobalCallback()
    {
        m_globalCallbackFunctions.clear();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::addChildCallback(const Callback& callback)
    {
        // If there is no global callback function then send the callback to the parent
        if (m_globalCallbackFunctions.empty())
            m_parent->addChildCallback(callback);
        else
        {
            // Loop through all callback functions and call them
            for (auto it = m_globalCallbackFunctions.cbegin(); it != m_globalCallbackFunctions.cend(); ++it)
                (*it)(callback);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::leftMousePressed(float x, float y)
    {
        sf::Event event;
        event.type = sf::Event::MouseButtonPressed;
        event.mouseButton.button = sf::Mouse::Left;
        event.mouseButton.x = static_cast<int>(x - getPosition().x);
        event.mouseButton.y = static_cast<int>(y - getPosition().y);

        // Let the event manager handle the event
        handleEvent(event);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::leftMouseReleased(float x , float y)
    {
        sf::Event event;
        event.type = sf::Event::MouseButtonReleased;
        event.mouseButton.button = sf::Mouse::Left;
        event.mouseButton.x = static_cast<int>(x - getPosition().x);
        event.mouseButton.y = static_cast<int>(y - getPosition().y);

        // Let the event manager handle the event
        handleEvent(event);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::mouseMoved(float x, float y)
    {
        sf::Event event;
        event.type = sf::Event::MouseMoved;
        event.mouseMove.x = static_cast<int>(x - getPosition().x);
        event.mouseMove.y = static_cast<int>(y - getPosition().y);

        // Let the event manager handle the event
        handleEvent(event);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::keyPressed(const sf::Event::KeyEvent& event)
    {
        sf::Event newEvent;
        newEvent.type = sf::Event::KeyPressed;
        newEvent.key = event;

        // Let the event manager handle the event
        handleEvent(newEvent);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::textEntered(sf::Uint32 key)
    {
        sf::Event event;
        event.type = sf::Event::TextEntered;
        event.text.unicode = key;

        // Let the event manager handle the event
        handleEvent(event);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::mouseWheelMoved(int delta, int x, int y)
    {
        sf::Event event;
        event.type = sf::Event::MouseWheelMoved;
        event.mouseWheel.delta = delta;
        event.mouseWheel.x = static_cast<int>(x - getPosition().x);
        event.mouseWheel.y = static_cast<int>(y - getPosition().y);

        // Let the event manager handle the event
        handleEvent(event);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::mouseNotOnWidget()
    {
        if (m_mouseHover == true)
        {
            mouseLeftWidget();

            for (unsigned int i = 0; i < m_widgets.size(); ++i)
                m_widgets[i]->mouseNotOnWidget();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::mouseNoLongerDown()
    {
        Widget::mouseNoLongerDown();

        for (unsigned int i = 0; i < m_widgets.size(); ++i)
            m_widgets[i]->mouseNoLongerDown();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::widgetUnfocused()
    {
        unfocusWidgets();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::initialize(Container *const parent)
    {
        Widget::initialize(parent);

        if (!m_fontPtr && m_parent->getGlobalFont())
            setGlobalFont(*m_parent->getGlobalFont());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::update()
    {
        // Loop through all widgets
        for (unsigned int i = 0; i < m_widgets.size(); ++i)
        {
            // Check if the widget is a container or a widget that uses the time
            if (m_widgets[i]->m_animatedWidget)
            {
                // Update the elapsed time
                m_widgets[i]->m_animationTimeElapsed += m_animationTimeElapsed;
                m_widgets[i]->update();
            }
        }

        m_animationTimeElapsed = {};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Container::handleEvent(sf::Event& event)
    {
        // Check if a mouse button has moved
        if (event.type == sf::Event::MouseMoved)
        {
            // Loop through all widgets
            for (unsigned int i = 0; i < m_widgets.size(); ++i)
            {
                // Check if the mouse went down on the widget
                if (m_widgets[i]->m_mouseDown)
                {
                    // Some widgets should always receive mouse move events while dragging them, even if the mouse is no longer on top of them.
                    if ((m_widgets[i]->m_draggableWidget) || (m_widgets[i]->m_containerWidget))
                    {
                        m_widgets[i]->mouseMoved(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
                        return true;
                    }
                }
            }

            // Check if the mouse is on top of a widget
            Widget::Ptr widget = mouseOnWhichWidget(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
            if (widget != nullptr)
            {
                // Send the event to the widget
                widget->mouseMoved(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
                return true;
            }

            return false;
        }

        // Check if a mouse button was pressed
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            // Check if the left mouse was pressed
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // Check if the mouse is on top of a widget
                Widget::Ptr widget = mouseOnWhichWidget(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                if (widget != nullptr)
                {
                    // Focus the widget
                    focusWidget(widget.get());

                    // Check if the widget is a container
                    if (widget->m_containerWidget)
                    {
                        // If another widget was focused then unfocus it now
                        if ((m_focusedWidget) && (m_widgets[m_focusedWidget-1] != widget))
                        {
                            m_widgets[m_focusedWidget-1]->m_focused = false;
                            m_widgets[m_focusedWidget-1]->widgetUnfocused();
                            m_focusedWidget = 0;
                        }
                    }

                    widget->leftMousePressed(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                    return true;
                }
                else // The mouse did not went down on a widget, so unfocus the focused widget
                    unfocusWidgets();
            }

            return false;
        }

        // Check if a mouse button was released
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            // Check if the left mouse was released
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // Check if the mouse is on top of a widget
                Widget::Ptr widget = mouseOnWhichWidget(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                if (widget != nullptr)
                    widget->leftMouseReleased(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                // Tell all the other widgets that the mouse has gone up
                for (std::vector<Widget::Ptr>::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
                {
                    if (*it != widget)
                        (*it)->mouseNoLongerDown();
                }

                if (widget != nullptr)
                    return true;
            }

            return false;
        }

        // Check if a key was pressed
        else if (event.type == sf::Event::KeyPressed)
        {
            // Only continue when the character was recognised
            if (event.key.code != sf::Keyboard::Unknown)
            {
                // Check if there is a focused widget
                if (m_focusedWidget)
                {
                    // Tell the widget that the key was pressed
                    m_widgets[m_focusedWidget-1]->keyPressed(event.key);

                    return true;
                }
            }

            return false;
        }

        // Check if a key was released
        else if (event.type == sf::Event::KeyReleased)
        {
            // Change the focus to another widget when the tab key was pressed
            if (event.key.code == sf::Keyboard::Tab)
                return tabKeyPressed();
            else
                return false;
        }

        // Also check if text was entered (not a special key)
        else if (event.type == sf::Event::TextEntered)
        {
            // Check if the character that we pressed is allowed
            if ((event.text.unicode >= 32) && (event.text.unicode != 127))
            {
                // Tell the widget that the key was pressed
                if (m_focusedWidget)
                {
                    m_widgets[m_focusedWidget-1]->textEntered(event.text.unicode);
                    return true;
                }
            }

            return false;
        }

        // Check for mouse wheel scrolling
        else if (event.type == sf::Event::MouseWheelMoved)
        {
            // Find the widget under the mouse
            Widget::Ptr widget = mouseOnWhichWidget(static_cast<float>(event.mouseWheel.x), static_cast<float>(event.mouseWheel.y));
            if (widget != nullptr)
            {
                // Send the event to the widget
                widget->mouseWheelMoved(event.mouseWheel.delta, event.mouseWheel.x,  event.mouseWheel.y);
                return true;
            }

            return false;
        }
        else // Event is ignored
            return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Container::focusNextWidgetInContainer()
    {
        // Don't do anything when the tab key usage is disabled
        if (TGUI_TabKeyUsageEnabled == false)
            return false;

        // Loop through all widgets
        for (unsigned int i = m_focusedWidget; i < m_widgets.size(); ++i)
        {
            // If you are not allowed to focus the widget, then skip it
            if (m_widgets[i]->m_allowFocus == true)
            {
                // Make sure that the widget is visible and enabled
                if ((m_widgets[i]->m_visible) && (m_widgets[i]->m_enabled))
                {
                    // Container widgets can only be focused it they contain focusable widgets
                    if ((!m_widgets[i]->m_containerWidget) || (std::static_pointer_cast<Container>(m_widgets[i])->focusNextWidgetInContainer()))
                    {
                        if (m_focusedWidget > 0)
                        {
                            // Unfocus the current widget
                            m_widgets[m_focusedWidget-1]->m_focused = false;
                            m_widgets[m_focusedWidget-1]->widgetUnfocused();
                        }

                        // Focus on the new widget
                        m_focusedWidget = i+1;
                        m_widgets[i]->m_focused = true;
                        m_widgets[i]->widgetFocused();

                        return true;
                    }
                }
            }
        }

        // We have the highest id
        unfocusWidgets();
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Container::tabKeyPressed()
    {
        // Don't do anything when the tab key usage is disabled
        if (TGUI_TabKeyUsageEnabled == false)
            return false;

        // Check if a container is focused
        if (m_focusedWidget)
        {
            if (m_widgets[m_focusedWidget-1]->m_containerWidget)
            {
                // Focus the next widget in container
                if (std::static_pointer_cast<Container>(m_widgets[m_focusedWidget-1])->focusNextWidgetInContainer())
                    return true;
            }
        }

        // Loop all widgets behind the focused one
        for (unsigned int i = m_focusedWidget; i < m_widgets.size(); ++i)
        {
            // If you are not allowed to focus the widget, then skip it
            if (m_widgets[i]->m_allowFocus == true)
            {
                // Make sure that the widget is visible and enabled
                if ((m_widgets[i]->m_visible) && (m_widgets[i]->m_enabled))
                {
                    if (m_focusedWidget)
                    {
                        // unfocus the current widget
                        m_widgets[m_focusedWidget-1]->m_focused = false;
                        m_widgets[m_focusedWidget-1]->widgetUnfocused();
                    }

                    // Focus on the new widget
                    m_focusedWidget = i+1;
                    m_widgets[i]->m_focused = true;
                    m_widgets[i]->widgetFocused();
                    return true;
                }
            }
        }

        // None of the widgets behind the focused one could be focused, so loop the ones before it
        if (m_focusedWidget)
        {
            for (unsigned int i = 0; i < m_focusedWidget-1; ++i)
            {
                // If you are not allowed to focus the widget, then skip it
                if (m_widgets[i]->m_allowFocus == true)
                {
                    // Make sure that the widget is visible and enabled
                    if ((m_widgets[i]->m_visible) && (m_widgets[i]->m_enabled))
                    {
                        // unfocus the current widget
                        m_widgets[m_focusedWidget-1]->m_focused = false;
                        m_widgets[m_focusedWidget-1]->widgetUnfocused();

                        // Focus on the new widget
                        m_focusedWidget = i+1;
                        m_widgets[i]->m_focused = true;
                        m_widgets[i]->widgetFocused();
                        return true;
                    }
                }
            }
        }

        // If the currently focused container widget is the only widget to focus, then focus its next child widget
        if ((m_focusedWidget) && (m_widgets[m_focusedWidget-1]->m_containerWidget))
        {
            std::static_pointer_cast<Container>(m_widgets[m_focusedWidget-1])->tabKeyPressed();
            return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Widget::Ptr Container::mouseOnWhichWidget(float x, float y)
    {
        bool widgetFound = false;
        Widget::Ptr widget = nullptr;

        // Loop through all widgets
        for (std::vector<Widget::Ptr>::reverse_iterator it = m_widgets.rbegin(); it != m_widgets.rend(); ++it)
        {
            // Check if the widget is visible and enabled
            if (((*it)->m_visible) && ((*it)->m_enabled))
            {
                if (widgetFound == false)
                {
                    // Return the widget if the mouse is on top of it
                    if ((*it)->mouseOnWidget(x, y))
                    {
                        widget = *it;
                        widgetFound = true;
                    }
                }
                else // The widget was already found, so tell the other widgets that the mouse can't be on them
                    (*it)->mouseNotOnWidget();
            }
        }

        return widget;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Container::drawWidgetContainer(sf::RenderTarget* target, const sf::RenderStates& states) const
    {
        // Draw all widgets when they are visible
        for (unsigned int i = 0; i < m_widgets.size(); ++i)
        {
            if (m_widgets[i]->m_visible)
                m_widgets[i]->draw(*target, states);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void GuiContainer::unbindGlobalCallback()
    {
        m_globalCallbackFunctions.erase(++m_globalCallbackFunctions.begin(), m_globalCallbackFunctions.end());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void GuiContainer::setSize(const Layout&)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool GuiContainer::mouseOnWidget(float, float)
    {
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void GuiContainer::draw(sf::RenderTarget&, sf::RenderStates) const
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
