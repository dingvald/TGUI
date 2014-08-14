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


#ifndef TGUI_LOADING_BAR_HPP
#define TGUI_LOADING_BAR_HPP


#include <TGUI/Label.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    class LoadingBarRenderer;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API LoadingBar : public ClickableWidget
    {
    public:

        typedef std::shared_ptr<LoadingBar> Ptr;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief The fill direction of the loading bar
        ///
        /// Determines in which direction the loading bar is filled when value goes from minimum to maximum.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        enum class FillDirection
        {
            LeftToRight,  ///< Start filling at the left side and go to the right
            RightToLeft,  ///< Start filling at the right side and go to the left
            TopToBottom,  ///< Start filling at the top an go downward
            BottomToTop   ///< Start filling at the bottom and go upward
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        LoadingBar();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~LoadingBar() {}


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Create the loading bar
        ///
        /// @param configFileFilename  Filename of the config file.
        /// @param section             The section in the theme file to read.
        ///
        /// @throw Exception when the config file could not be opened.
        /// @throw Exception when the config file did not contain the requested section with the needed information.
        /// @throw Exception when one of the images, described in the config file, could not be loaded.
        ///
        /// When an empty string is passed as filename, the built-in white theme will be used.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static LoadingBar::Ptr create(const std::string& configFileFilename = "", const std::string& section = "Loadingbar");


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of another loading bar
        ///
        /// @param loadingBar  The other loading bar
        ///
        /// @return The new loading bar
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static LoadingBar::Ptr copy(const LoadingBar::Ptr& loadingBar)
        {
            return std::make_shared<LoadingBar>(*loadingBar);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the renderer, which gives access to functions that determine how the widget is displayed
        ///
        /// @return Reference to the renderer
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<LoadingBarRenderer> getRenderer() const
        {
            return std::static_pointer_cast<LoadingBarRenderer>(m_renderer);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Set the position of the widget
        ///
        /// This function completely overwrites the previous position.
        /// See the move function to apply an offset based on the previous position instead.
        /// The default position of a transformable widget is (0, 0).
        ///
        /// @param position  New position
        ///
        /// @see move, getPosition
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setPosition(const Layout& position) override;
        using Transformable::setPosition;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the size of the loading bar.
        ///
        /// @param size  The new size of the loading bar
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSize(const Layout& size) override;
        using Transformable::setSize;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets a minimum value.
        ///
        /// @param minimum  The new minimum value
        ///
        /// When the value is too small then it will be changed to this minimum.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setMinimum(unsigned int minimum);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets a maximum value.
        ///
        /// @param maximum  The new maximum value
        ///
        /// When the value is too big then it will be changed to this maximum.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setMaximum(unsigned int maximum);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the current value.
        ///
        /// @param value  The new value
        ///
        /// The value can't be smaller than the minimum or bigger than the maximum.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setValue(unsigned int value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the minimum value.
        ///
        /// @return The current minimum value
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getMinimum() const
        {
            return m_minimum;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the maximum value.
        ///
        /// @return The current maximum value
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getMaximum() const
        {
            return m_maximum;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the current value.
        ///
        /// @return The current value
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getValue() const
        {
            return m_value;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Increment the value.
        ///
        /// @return the new value
        ///
        /// The value can never exceed the maximum.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int incrementValue();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the caption of the loading bar.
        ///
        /// @param text  New text to draw on top of the loading bar
        ///
        /// This text will be centered in the loading bar. It could e.g. contain the progress.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setText(const sf::String& text);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the caption of the loading bar.
        ///
        /// @return Text that is drawn on top of the loading bar
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::String getText() const
        {
            return m_textBack.getText();
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the character size of the text.
        ///
        /// @param size  The new text size.
        ///              If the size is set to 0 then the text will be auto-sized to fit inside the loading bar.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextSize(unsigned int size);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the character size of the text.
        ///
        /// @return The current text size.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getTextSize() const
        {
            return m_textBack.getTextSize();
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the fill direction of the loading bar
        ///
        /// @param direction  In which direction is the loading bar filled when the value goes from minimum to maximum?
        ///
        /// By default the loading bar is filled from left to right.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setFillDirection(FillDirection direction);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the fill direction of the loading bar
        ///
        /// @return In which direction is the loading bar filled when the value goes from minimum to maximum?
        ///
        /// By default the loading bar is filled from left to right.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FillDirection getFillDirection()
        {
            return m_fillDirection;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the transparency of the widget.
        ///
        /// @param transparency  The transparency of the widget.
        ///                      0 is completely transparent, while 255 (default) means fully opaque.
        ///
        /// Note that this will only change the transparency of the images. The parts of the widgets that use a color will not
        /// be changed. You must change them yourself by setting the alpha channel of the color.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setTransparency(unsigned char transparency) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This function is called when the widget is added to a container.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void initialize(Container *const container) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Widget::Ptr clone() override
        {
            return std::make_shared<LoadingBar>(*this);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // When the value changes, or when the minimum/maximum limits change then a smaller of bigger piece of the front image
        // must be drawn. This function is called to calculate the size of the piece to draw.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void recalculateSize();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draws the widget on the render target.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Defines specific triggers to LoadingBar.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        enum LoadingBarCallbacks
        {
            ValueChanged = ClickableWidgetCallbacksCount * 1,    ///< Value changed
            LoadingBarFull = ClickableWidgetCallbacksCount * 2,  ///< Value reached the max value.
            LoadingBarCallbacksCount = ClickableWidgetCallbacksCount * 4
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        unsigned int m_minimum = 0;
        unsigned int m_maximum = 100;
        unsigned int m_value = 0;

        Label m_textBack;
        Label m_textFront;
        unsigned int m_textSize = 0;

        sf::FloatRect m_frontRect;

        FillDirection m_fillDirection = FillDirection::LeftToRight;

        friend class LoadingBarRenderer;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class LoadingBarRenderer : public WidgetRenderer, public WidgetBorders
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor
        ///
        /// @param loadingBar  The loading bar that is connected to the renderer
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        LoadingBarRenderer(LoadingBar* loadingBar) : m_loadingBar{loadingBar} {}


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Dynamically change a property of the renderer, without even knowing the type of the widget.
        ///
        /// This function should only be used when you don't know the type of the widget.
        /// Otherwise you can make a direct function call to make the wanted change.
        ///
        /// @param property  The property that you would like to change
        /// @param value     The new value that you like to assign to the property
        /// @param rootPath  Path that should be placed in front of any resource filename
        ///
        /// @throw Exception when the property doesn't exist for this widget.
        /// @throw Exception when the value is invalid for this property.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setProperty(std::string property, const std::string& value, const std::string& rootPath = getResourcePath()) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the font of the text.
        ///
        /// When you don't call this function then the global font will be use.
        /// This global font can be changed with the setGlobalFont function from the parent.
        ///
        /// @param font  The new font
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextFont(const sf::Font& font);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the color of the text that is optionally displayed on top of the loading bar.
        ///
        /// @param color  The new text color
        ///
        /// This changes both the back and front text colors.
        ///
        /// @see setTextColorBack
        /// @see setTextColorFront
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextColor(const sf::Color& color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the color of the text that is optionally displayed on top of the loading bar.
        ///
        /// @param color  The new text color that is displayed on top of the background color/image.
        ///
        /// This color is displayed on top of the unfilled part. The front text color will be used on top of the filled part.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextColorBack(const sf::Color& color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the color of the text that is optionally displayed on top of the loading bar.
        ///
        /// @param color  The new text color that is displayed on top of the foreground color/image.
        ///
        /// This color is displayed on top of the filled part. The back text color will be used on top of the unfilled part.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextColorFront(const sf::Color& color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the background color of the loading bar.
        ///
        /// @param color  The new background color
        ///
        /// This is the color that you see in the part of the loading bar that is not filled.
        ///
        /// Note that this color is ignored when you set an image as background.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setBackgroundColor(const sf::Color& color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the foreground color of the loading bar.
        ///
        /// @param color  The new foreground color
        ///
        /// This is the color that is used to fill the loading bar and is drawn on top of the background color.
        ///
        /// Note that this color is ignored when you set an image as foreground.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setForegroundColor(const sf::Color& color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the border color.
        ///
        /// @param color  The color that is used for the borders that are optionally drawn around the loading bar
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setBorderColor(const sf::Color& color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Change the background image of the loading bar
        ///
        /// When this image and the front image are set, the background color property will be ignored.
        ///
        /// Pass an empty string to unset the image, in this case the background color property will be used again.
        ///
        /// @param filename   Filename of the image to load.
        /// @param partRect   Load only part of the image. Don't pass this parameter if you want to load the full image.
        /// @param middlePart Choose the middle part of the image for 9-slice scaling (relative to the part defined by partRect)
        /// @param repeated   Should the image be repeated or stretched when the size is bigger than the image?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setBackImage(const std::string& filename,
                          const sf::IntRect& partRect = sf::IntRect(0, 0, 0, 0),
                          const sf::IntRect& middlePart = sf::IntRect(0, 0, 0, 0),
                          bool repeated = false);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Change the foreground image of the loading bar
        ///
        /// When this image and the back image are set, the background color property will be ignored.
        ///
        /// Pass an empty string to unset the image.
        ///
        /// @param filename   Filename of the image to load.
        /// @param partRect   Load only part of the image. Don't pass this parameter if you want to load the full image.
        /// @param middlePart Choose the middle part of the image for 9-slice scaling (relative to the part defined by partRect)
        /// @param repeated   Should the image be repeated or stretched when the size is bigger than the image?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setFrontImage(const std::string& filename,
                           const sf::IntRect& partRect = sf::IntRect(0, 0, 0, 0),
                           const sf::IntRect& middlePart = sf::IntRect(0, 0, 0, 0),
                           bool repeated = false);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draws the widget on the render target.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the renderer
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual std::shared_ptr<WidgetRenderer> clone(Widget* widget) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        LoadingBarRenderer(const LoadingBarRenderer&) = default;
        LoadingBarRenderer& operator=(const LoadingBarRenderer&) = delete;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        LoadingBar* m_loadingBar;

        Texture  m_textureBack;
        Texture  m_textureFront;

        sf::Color m_backgroundColor = {245, 245, 245};
        sf::Color m_foregroundColor = {0, 110, 255};

        sf::Color m_borderColor = {0, 0, 0};

        friend class LoadingBar;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_LOADING_BAR_HPP
