/* 
 * File:   imageResource.h
 * Author: Miles Lacey
 *
 * Created on February 2, 2014, 1:42 PM
 */

#ifndef __LS_DRAW_IMAGE_RESOURCE_H__
#define	__LS_DRAW_IMAGE_RESOURCE_H__

#include <string>

#include "lightsky/draw/Setup.h"
#include "lightsky/utils/resource.h"
#include "lightsky/draw/Color.h"

namespace ls {
namespace draw {

/**----------------------------------------------------------------------------
 * Enumerations for saving image files.
-----------------------------------------------------------------------------*/
enum class img_file_t {
    IMG_FILE_BMP,
    IMG_FILE_EXR,
    IMG_FILE_GIF,
    IMG_FILE_HDR,
    IMG_FILE_ICO,
    IMG_FILE_JPG,
    IMG_FILE_J2K,
    IMG_FILE_PNG, // default
    IMG_FILE_PPM,
    IMG_FILE_TGA,
    IMG_FILE_TIF,
    IMG_FILE_WBP,
    IMG_FILE_XPM
};

/**----------------------------------------------------------------------------
 * @brief A resource object to load image files.
 * 
 * This class can load images using the FreeImage library, The pData member
 * contains raw texture data.
 * 
 * The inherited member "pData" will be reinterpreted as a pointer to a
 * FIBITMAP structure so as to make loading and saving easier to maintain.
-----------------------------------------------------------------------------*/
class imageResource final : public ls::utils::resource {
    private:
        /**
         * imgSize represents the pixel width & height of a loaded image
         */
        math::vec2i imgSize = {0};
        
        /**
         * Pixel format of the loaded image
         */
        color_type_t pixelType = COLOR_TYPE_DEFAULT;
        
        /**
         * Number of bits per pixel in the image
         */
        unsigned bitsPerPixel = 0;
        
        /**
         * @brief CPU-Side format of an image
         * 
         * This can be any of the following:
         * LS_GRAY_N
         * LS_RG_N
         * LS_RGB_N
         * LS_RGBA_N
         * LS_RGB_32F
         * LS_RGBA_32F
         * where N is 8, 16, or 32
         */
        pixel_format_t intFormat = COLOR_FMT_DEFAULT;
        
        
        /**
         * @brief CPU-Side format of an image
         * 
         * This can be any of the following:
         * LS_GRAY
         * LS_RG
         * LS_RGB
         * LS_RGBA
         * LS_RGB16F
         * LS_RGBA32F
         * LS_RGB16F
         * LS_RGBA32F
         */
        pixel_layout_t extFormat = COLOR_LAYOUT_DEFAULT;

    public:
        /**
         * @brief Constructor
         */
        imageResource();
        
        /**
         * @brief Copy Constructor -- DELETED
         */
        imageResource(const imageResource&) = delete;
        
        /**
         * @brief Move Operator
         * 
         * Moves data from the source operand into *this. No copies are
         * performed.
         * 
         * @param ir
         * An r-value reference to a temporary image resource object.
         */
        imageResource(imageResource&& ir);
        
        /**
         * @brief Destructor
         * 
         * Calls "unload()" and releases all memory from *this.
         */
        virtual ~imageResource();
        
        /**
         * @brief Copy Operator -- DELETED
         */
        imageResource& operator=(const imageResource&) = delete;
        
        /**
         * @brief Move Operator
         * 
         * Moves all data from the source operand into *this. No copies are
         * performed.
         * 
         * @param ir
         * An r-value reference to a temporary image resource object.
         * 
         * @return a reference to *this.
         */
        imageResource& operator=(imageResource&& ir);

        /**
         * @brief Load an image file
         * 
         * @param filename
         * A string object containing the relative path name to a file that
         * should be loadable into memory.
         * 
         * @return true if the file was successfully loaded. False if not.
         */
        virtual bool loadFile(const std::string& filename) override;

        /**
         * @brief Save an image file
         * 
         * @param filename
         * A string object containing the relative path name to a file that
         * should be saved to the computer.
         * 
         * @return true if the file was successfully saved. False if not.
         */
        virtual bool saveFile(const std::string& filename) const override;

        /**
         * @brief Save an image file in a specific format
         * 
         * @param filename
         * A string object containing the relative path name to a file that
         * should be saved to the computer.
         * 
         * @param filetype
         * An img_file_t, representing the file format that should be used when
         * saving image data.
         * 
         * @return true if the file was successfully saved. False if not.
         */
        bool saveFile(const std::string& filename, img_file_t filetype) const;

        /**
         * @brief Save an image file in a specific format
         * 
         * @param filename
         * A wide string object containing the relative path name to a file that
         * should be saved to the computer.
         * 
         * @param filetype
         * An img_file_t, representing the file format that should be used when
         * saving image data.
         * 
         * @return true if the file was successfully saved. False if not.
         */
        virtual bool saveFile(const std::wstring& filename) const override;

        /**
         * @brief Unload
         * 
         * Free all memory used by *this.
         */
        virtual void unload() override;

        /**
         * Get the raw, loaded, image data contained within *this.
         * The underlying type has been cast from a FreeImage FIBITMAP*.
         * 
         * @return a void pointer to the raw image bits.
         */
        virtual void* getData() const override;
        
        /**
         * Get the pixel size of the currently loaded image
         * 
         * @return a 2D integer vector containing the width and height of the
         * loaded image, in pixels.
         */
        math::vec2i getPixelSize() const;
        
        /**
         * Get the format of the currently loaded image.
         * For example, LS_UNSIGNED_BYTE, LS_INT, LS_FLOAT, etc.
         */
        color_type_t getPixelType() const;
        
        /**
         * Get the number of bits per pixel in the image.
         * 
         * @return 0, 1, 2, 4, 8, 16, 24, 32, 48, 64, 96, or 128
         */
        unsigned getBpp() const;
        
        /**
         * Get the OpenGL-compatible CPU-Side image format.
         * 
         * @return An enumeration containing image format information that can
         * be used when setting up textures in OpenGL.
         */
        pixel_format_t getInternalFormat() const;
        
        /**
         * Get the OpenGL-compatible CPU-Side/GPU-Side image format pair.
         * 
         * @return An enumeration containing image format information that can
         * be used when setting up textures in OpenGL.
         */
        pixel_layout_t getExternalFormat() const;
};

/*-------------------------------------
 * Saving Data
-------------------------------------*/
inline bool imageResource::saveFile(const std::string& filename) const {
    return saveFile(filename, img_file_t::IMG_FILE_PNG);
}

/*-------------------------------------
 * Saving Data (wide-string)
-------------------------------------*/
inline bool imageResource::saveFile(const std::wstring& filename) const {
    const std::string&& file = ls::utils::convertWtoMb(filename);
    return saveFile(file, img_file_t::IMG_FILE_PNG);
}

/*-------------------------------------
 * Get the pixel size of the currently loaded image
-------------------------------------*/
inline math::vec2i imageResource::getPixelSize() const {
    return imgSize;
}

/*-------------------------------------
 * Get the GPU-compatible format of the currently loaded image
-------------------------------------*/
inline color_type_t imageResource::getPixelType() const {
    return pixelType;
}

/*-------------------------------------
 * Get the number of bits per pixel in the image.
-------------------------------------*/
inline unsigned imageResource::getBpp() const {
    return bitsPerPixel;
}

/*-------------------------------------
 * Get the OpenGL-compatible CPU-Side image format.
-------------------------------------*/
inline pixel_format_t imageResource::getInternalFormat() const {
    return intFormat;
}

/*-------------------------------------
 * Get the OpenGL-compatible GPU-Side image format.
-------------------------------------*/
inline pixel_layout_t imageResource::getExternalFormat() const {
    return extFormat;
}

} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_IMAGE_RESOURCE_H__ */

