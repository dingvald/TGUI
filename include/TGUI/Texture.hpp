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


#ifndef TGUI_TEXTURE_HPP
#define TGUI_TEXTURE_HPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <TGUI/TextureData.hpp>
#include <TGUI/TextureManager.hpp>

#include <TGUI/Global.hpp>

#include <functional>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API Texture : public sf::Transformable
    {
    public:

        using ImageLoaderFunc = std::function<std::shared_ptr<sf::Image>(const sf::String&)>;
        using TextureLoaderFunc = std::function<bool(Texture&, const sf::String&, const sf::IntRect&)>;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief The way the image should be scaled
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        enum class ScalingType
        {
            Normal,     ///< The image is not split and scaled normally
            Horizontal, ///< Image is split in Left, Middle and Right parts. Left and Right keep ratio, Middle gets stretched
            Vertical,   ///< Image is split in Top, Middle and Bottom parts. Top and Bottom keep ratio, Middle gets stretched
            NineSlice   ///< Image is split in 9 parts. Corners keep size, sides are stretched in one direction, middle is stretched in both directions
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Texture() {}


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor that created the texture
        ///
        /// @param id         Id for the the image to load (for the default loader, the id is the filename)
        /// @param partRect   Load only part of the image. Pass an empty rectangle if you want to load the full image
        /// @param middlePart Choose the middle part of the image for 9-slice scaling (relative to the part defined by partRect)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Texture(const char* id,
                const sf::IntRect& partRect = sf::IntRect(0, 0, 0, 0),
                const sf::IntRect& middlePart = sf::IntRect(0, 0, 0, 0))
            : Texture(sf::String{id}, partRect, middlePart)
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor that created the texture
        ///
        /// @param id         Id for the the image to load (for the default loader, the id is the filename)
        /// @param partRect   Load only part of the image. Pass an empty rectangle if you want to load the full image
        /// @param middlePart Choose the middle part of the image for 9-slice scaling (relative to the part defined by partRect)
        ///
        /// This constructor just calls the corresponding load function.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Texture(const sf::String& id,
                const sf::IntRect& partRect = sf::IntRect(0, 0, 0, 0),
                const sf::IntRect& middlePart = sf::IntRect(0, 0, 0, 0));


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor that created the texture from an existing sf::Texture
        ///
        /// @param texture    Existing texture to copy
        /// @param partRect   Load only part of the image. Pass an empty rectangle if you want to load the full image
        /// @param middlePart Choose the middle part of the image for 9-slice scaling (relative to the part defined by partRect)
        ///
        /// The texture will be copied, you do not have to keep the sf::Texture alive after calling this function.
        ///
        /// This constructor just calls the corresponding load function.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Texture(const sf::Texture& texture,
                const sf::IntRect& partRect = sf::IntRect(0, 0, 0, 0),
                const sf::IntRect& middlePart = sf::IntRect(0, 0, 0, 0));


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Copy constructor
        ///
        /// @param texture  Instance to copy
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Texture(const Texture& texture);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~Texture();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Overload of assignment operator
        ///
        /// @param right  Instance to assign
        ///
        /// @return Reference to itself
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Texture& operator=(const Texture& right);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Creates the texture
        ///
        /// @param id         Id for the the image to load (for the default loader, the id is the filename)
        /// @param partRect   Load only part of the image. Don't pass this parameter if you want to load the full image
        /// @param middleRect Choose the middle part of the image for 9-slice scaling (relative to the part defined by partRect)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void load(const sf::String& id,
                  const sf::IntRect& partRect = {},
                  const sf::IntRect& middleRect = {});


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Creates the texture from an existing sf::Texture
        ///
        /// @param texture    Existing texture to copy
        /// @param partRect   Load only part of the texture. Don't pass this parameter if you want to load the full image
        /// @param middleRect Choose the middle part of the image for 9-slice scaling (relative to the part defined by partRect)
        ///
        /// The texture will be copied, you do not have to keep the sf::Texture alive after calling this function.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void load(const sf::Texture& texture,
                  const sf::IntRect& partRect = {},
                  const sf::IntRect& middleRect = {});


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the texture
        ///
        /// @param data       New texture data
        /// @param middleRect Choose the middle part of the image part to determine scaling (e.g. 9-slice scaling)
        ///
        /// This function is not intended to be used directly, except from custom loaders.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTexture(std::shared_ptr<TextureData> data, const sf::IntRect& middleRect = {});


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the id that was used to load the texture (for the default loader, the id is the filename)
        ///
        /// @return Id of the texture
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::string getId() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns a reference to the texture data
        ///
        /// @return Data of the texture
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<TextureData>& getData();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the constant texture data
        ///
        /// @return Read-only data of the texture
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<const TextureData> getData() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the size that the image will have on the screen
        ///
        /// @param size  Size of the image
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSize(const sf::Vector2f& size);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the size that the image has on the screen
        ///
        /// @return Size of the image
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::Vector2f getSize() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the size that the loaded image
        ///
        /// @return Size of the image like it was when loaded (no scaling applied)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::Vector2f getImageSize() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the global color of the sprite
        ///
        /// This color is modulated (multiplied) with the sprite's texture. It can be used to colorize the sprite,
        /// or change its global opacity. Note that the alpha component is multiplied with the opacity set by setOpacity.
        ///
        /// By default, the sprite's color is opaque white.
        ///
        /// @param color  New color of the sprite
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setColor(const sf::Color& color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the global color of the sprite
        ///
        /// This color is modulated (multiplied) with the sprite's texture. It can be used to colorize the sprite,
        /// or change its global opacity.
        ///
        /// By default, the sprite's color is opaque white.
        ///
        /// @return Current color of the sprite
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Color& getColor() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the opacity of the texture
        ///
        /// @param opacity  The opacity of the texture. 0 means completely transparent, while 1 (default) means fully opaque
        ///
        /// The alpha component of the color specified with setColor is multiplied with this factor.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setOpacity(float opacity);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the opacity of the texture
        ///
        /// @return The opacity of the texture. 0 means completely transparent, while 1 (default) means fully opaque
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getOpacity() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the part of the texture that should be drawn
        ///
        /// @param textureRect Visible part of the texture
        ///
        /// Set this to (0, 0, 0, 0) to show the entire texture
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextureRect(const sf::FloatRect& textureRect);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the part of the texture that is drawn
        ///
        /// @return Visible part of the texture
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::FloatRect getTextureRect() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the middle rect of the texture which is used for 9-slice scaling
        ///
        /// @return Middle rect of the texture
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::IntRect getMiddleRect() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Enables or disable the smooth filter
        ///
        /// When the filter is activated, the texture appears smoother so that pixels are less noticeable.
        /// However if you want the texture to look exactly the same as its source file, you should leave it disabled.
        /// The smooth filter is disabled by default.
        ///
        /// @param smooth True to enable smoothing, false to disable it
        ///
        /// @see isSmooth
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSmooth(bool smooth);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Tells whether the smooth filter is enabled or not
        ///
        /// @return True if smoothing is enabled, false if it is disabled
        ///
        /// @see setSmooth
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isSmooth() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Checks if a certain pixel is transparent
        ///
        /// @param pos  Coordinate of the pixel
        ///
        /// @return True when the pixel is transparent, false when it is not
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isTransparentPixel(sf::Vector2f pos) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the way in which the image is being scaled
        ///
        /// @return Scaling type
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ScalingType getScalingType() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Checks if the texture has been correctly loaded
        ///
        /// @return True if texture was initialized
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isLoaded() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets a different image loader
        ///
        /// @param func  New image loader function
        ///
        /// The image loader will be called inside the texture loader to create the sf::Image.
        ///
        /// The default loader will simply load the image from a file.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void setImageLoader(const ImageLoaderFunc& func);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets a different texture loader
        ///
        /// @param func  New texture loader function
        ///
        /// The texture loader will initialize this Texture object.
        ///
        /// The default loader will use an internal texture manager to prevent the same thing from being loaded twice.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void setTextureLoader(const TextureLoaderFunc& func);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the used image loader
        ///
        /// @return Image loader that is currently being used
        ///
        /// @see setImageLoader
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static ImageLoaderFunc getImageLoader();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the used texture loader
        ///
        /// @return Texture loader that is currently being used
        ///
        /// @see setTextureLoader
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static TextureLoaderFunc getTextureLoader();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets a callback function for when this texture is copied
        ///
        /// @param func  Function that will be called when this texture is copied
        ///
        /// This function can be useful when implementing a resource manager.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setCopyCallback(const std::function<void(std::shared_ptr<TextureData>)> func);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets a callback function for when this texture is destroyed
        ///
        /// @param func  Function that will be called when this texture is destroyed
        ///
        /// This function can be useful when implementing a resource manager.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setDestructCallback(const std::function<void(std::shared_ptr<TextureData>)> func);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draws the texture
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Update the location of the vertices
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void updateVertices();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    private:
        std::shared_ptr<TextureData> m_data = std::make_shared<TextureData>();
        std::vector<sf::Vertex> m_vertices;

        sf::Vector2f  m_size;
        sf::IntRect   m_middleRect;
        sf::FloatRect m_textureRect;
        sf::Color     m_vertexColor = sf::Color::White;
        float         m_opacity = 1;

        ScalingType   m_scalingType = ScalingType::Normal;

        bool m_loaded = false;
        std::string m_id;

        std::function<void(std::shared_ptr<TextureData>)> m_copyCallback;
        std::function<void(std::shared_ptr<TextureData>)> m_destructCallback;
        static TextureLoaderFunc m_textureLoader;
        static ImageLoaderFunc m_imageLoader;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_TEXTURE_HPP
