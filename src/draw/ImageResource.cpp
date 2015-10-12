/* 
 * File:   imageResource.cpp
 * Author: Miles Lacey
 * 
 * Created on February 2, 2014, 1:42 PM
 */

#include <utility> // std::move
#include <string>

#include "lightsky/draw/Color.h"
#include "lightsky/draw/ImageResource.h"

#include <FreeImage.h>

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
    Utility/Helper Functions
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * FreeImage error handler callback
-------------------------------------*/
#ifdef LS_DEBUG
    void printImageLoadError(FREE_IMAGE_FORMAT fif, const char* msg) {
        LS_LOG_ERR(
            "\tAn image file error has occurred:",
            "\n\tFormat: ", FreeImage_GetFormatFromFIF(fif),
            "\n\t", msg, "."
        );
    }
#else
    void printImageLoadError(FREE_IMAGE_FORMAT, const char*) {}
#endif

/*-------------------------------------
 * Deduce an image's file format
-------------------------------------*/
FREE_IMAGE_FORMAT deduce_img_format(const std::string& filename) {
	FREE_IMAGE_FORMAT outFormat = FreeImage_GetFileType(filename.c_str(), 0);
	if (outFormat == FIF_UNKNOWN) {
		outFormat = FreeImage_GetFIFFromFilename(filename.c_str());
    }
    return outFormat;
}

/*-------------------------------------
 * Predefined image flags
-------------------------------------*/
int get_img_flags(FREE_IMAGE_FORMAT inFormat) {
	switch(inFormat) {
		case FIF_JPEG:  return JPEG_ACCURATE;
		case FIF_TARGA: return TARGA_LOAD_RGB888;
		case FIF_ICO:   return ICO_MAKEALPHA;
		default:        return 0;
	}
}

/*-------------------------------------
 * Get an image's pixel format, combined with its bits per pixel
-------------------------------------*/
color_type_t get_bitmap_size(FIBITMAP* pImg) {
    // Get the data type of the image. Convert to an internal format
    const int storageType = FreeImage_GetImageType(pImg);
    color_type_t dataType = COLOR_TYPE_DEFAULT;
    
    if (storageType == FIT_UNKNOWN
    ||  storageType == FIT_DOUBLE
    ||  storageType == FIT_COMPLEX
    ) {
        return COLOR_TYPE_INVALID;
    }
    
    switch(storageType) {
        // n-bit char
        case FIT_BITMAP:
            LS_LOG_MSG("\tImage pixel type: BYTE");
            dataType = COLOR_TYPE_UNSIGNED_BYTE;
            break;
            
        // 16-bit short
        case FIT_INT16:
            dataType = COLOR_TYPE_SHORT;
            LS_LOG_MSG("\tImage pixel type: SHORT");
            break;
                
        case FIT_UINT16:
            dataType = COLOR_TYPE_UNSIGNED_SHORT;
            LS_LOG_MSG("\tImage pixel type: UNSIGNED SHORT");
            break;
        
        // 32-bit int
        case FIT_INT32:
            dataType = COLOR_TYPE_INT;
            LS_LOG_MSG("\tImage pixel type: INT");
            break;
                
        case FIT_UINT32:
            dataType = COLOR_TYPE_UNSIGNED_INT;
            LS_LOG_MSG("\tImage pixel type: UNSIGNED INT");
            break;
        
        // 96-bit float
        case FIT_RGBF:
        // 128-bit float
        case FIT_RGBAF:
            dataType = COLOR_TYPE_FLOAT;
            LS_LOG_MSG("\tImage pixel type: FLOAT");
            break;
        
        // unknown
        default:
            dataType = COLOR_TYPE_INVALID;
            LS_LOG_MSG("\tImage pixel type: INVALID");
            break;
    }
    
    return dataType;
}

void get_pixel_format(
    FIBITMAP* pImg,
    unsigned bpp,
    pixel_format_t& intFmt,
    pixel_layout_t& extFmt
) {
    LS_LOG_MSG("\tImage Bits Per Pixel: ", bpp);
    
    // setup some default values in case nothing below returns.
    intFmt = COLOR_FMT_DEFAULT;
    extFmt = COLOR_LAYOUT_DEFAULT;
    
    // Get the data type of the image. Convert to an internal format
    const FREE_IMAGE_TYPE dataType = FreeImage_GetImageType(pImg);
    
    if (dataType == FIT_BITMAP) {
        if (bpp == 8)   {intFmt = COLOR_FMT_R_8;         extFmt = COLOR_LAYOUT_R;}
        if (bpp == 16)  {intFmt = COLOR_FMT_RG_8;        extFmt = COLOR_LAYOUT_RG;}
        if (bpp == 24)  {intFmt = COLOR_FMT_RGB_8;       extFmt = COLOR_LAYOUT_RGB;}
        if (bpp == 32)  {intFmt = COLOR_FMT_RGBA_8;      extFmt = COLOR_LAYOUT_RGBA;}
    }
    else if (dataType == FIT_INT16) {
        if (bpp == 16)  {intFmt = COLOR_FMT_R_16I;       extFmt = COLOR_LAYOUT_R;}
        if (bpp == 32)  {intFmt = COLOR_FMT_RG_16I;      extFmt = COLOR_LAYOUT_RG;}
        if (bpp == 48)  {intFmt = COLOR_FMT_RGB_16I;     extFmt = COLOR_LAYOUT_RGB;}
        if (bpp == 64)  {intFmt = COLOR_FMT_RGBA_16I;    extFmt = COLOR_LAYOUT_RGBA;}
    }
    else if (dataType == FIT_UINT16) {
        if (bpp == 16)  {intFmt = COLOR_FMT_R_16U;       extFmt = COLOR_LAYOUT_R;}
        if (bpp == 32)  {intFmt = COLOR_FMT_RG_16U;      extFmt = COLOR_LAYOUT_RG;}
        if (bpp == 48)  {intFmt = COLOR_FMT_RGB_16U;     extFmt = COLOR_LAYOUT_RGB;}
        if (bpp == 64)  {intFmt = COLOR_FMT_RGBA_16U;    extFmt = COLOR_LAYOUT_RGBA;}
    }
    else if (dataType == FIT_INT32) {
        if (bpp == 32)  {intFmt = COLOR_FMT_R_32I;       extFmt = COLOR_LAYOUT_R;}
        if (bpp == 64)  {intFmt = COLOR_FMT_RG_32I;      extFmt = COLOR_LAYOUT_RG;}
        if (bpp == 96)  {intFmt = COLOR_FMT_RGB_32I;     extFmt = COLOR_LAYOUT_RGB;}
        if (bpp == 128) {intFmt = COLOR_FMT_RGBA_32I;    extFmt = COLOR_LAYOUT_RGBA;}
    }
    else if (dataType == FIT_UINT32) {
        if (bpp == 32)  {intFmt = COLOR_FMT_R_32U;       extFmt = COLOR_LAYOUT_R;}
        if (bpp == 64)  {intFmt = COLOR_FMT_RG_32U;      extFmt = COLOR_LAYOUT_RG;}
        if (bpp == 96)  {intFmt = COLOR_FMT_RGB_32U;     extFmt = COLOR_LAYOUT_RGB;}
        if (bpp == 128) {intFmt = COLOR_FMT_RGBA_32U;    extFmt = COLOR_LAYOUT_RGBA;}
    }
    else if (dataType == FIT_FLOAT) {
        if (bpp == 32)  {intFmt = COLOR_FMT_R_32F;       extFmt = COLOR_LAYOUT_R;}
        if (bpp == 64)  {intFmt = COLOR_FMT_RG_32F;      extFmt = COLOR_LAYOUT_RG;}
        if (bpp == 96)  {intFmt = COLOR_FMT_RGB_32F;     extFmt = COLOR_LAYOUT_RGB;}
        if (bpp == 128) {intFmt = COLOR_FMT_RGBA_32F;    extFmt = COLOR_LAYOUT_RGBA;}
    }
    else if (dataType == FIT_RGB16) {
        intFmt = COLOR_FMT_RGB_16F;
        extFmt = COLOR_LAYOUT_RGB;
    }
    else if (dataType == FIT_RGBA16) {
        intFmt = COLOR_FMT_RGBA_16F;
        extFmt = COLOR_LAYOUT_RGBA;
    }
    else if (dataType == FIT_RGBF) {
        intFmt = COLOR_FMT_RGB_32F;
        extFmt = COLOR_LAYOUT_RGB;
    }
    else if (dataType == FIT_RGBAF) {
        intFmt = COLOR_FMT_RGBA_32F;
        extFmt = COLOR_LAYOUT_RGBA;
    }
}

/*-----------------------------------------------------------------------------
    Image Resource Method Definitions
-----------------------------------------------------------------------------*/
/*-------------------------------------
    Constructor
-------------------------------------*/
ImageResource::ImageResource() :
    Resource{}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
ImageResource::ImageResource(ImageResource&& img) :
    Resource{}
{
    this->operator =(std::move(img));
}
    
/*-------------------------------------
 * Destructor
-------------------------------------*/
ImageResource::~ImageResource() {
    unload();
}

/*-------------------------------------
 * Move operator
-------------------------------------*/
ImageResource& ImageResource::operator =(ImageResource&& img) {
    // Resolve movement for the base class members first
    unload();
    
    pData = img.pData;
    img.pData = nullptr;
    dataSize = img.dataSize;
    img.dataSize = 0;
    
    imgSize = img.imgSize;
    img.imgSize = math::vec2i{0};
    
    pixelType = img.pixelType;
    img.pixelType = COLOR_TYPE_DEFAULT;
    
    bitsPerPixel = img.bitsPerPixel;
    img.bitsPerPixel = 0;
    
    intFormat = img.intFormat;
    img.intFormat = COLOR_FMT_DEFAULT;
    
    extFormat = img.extFormat;
    img.extFormat = COLOR_LAYOUT_DEFAULT;
    
    return *this;
}

/*-------------------------------------
 * Loading
-------------------------------------*/
bool ImageResource::load_file(const std::string& filename) {
    LS_LOG_MSG("Attempting to load the image ", filename);
    unload();
    
    if (!filename.size()) {
        LS_LOG_ERR("\tFailed to load an image as no filename was provided.\n");
        return false;
    }
    
    // Set FreeImage's error function
    FreeImage_SetOutputMessage(&printImageLoadError);
    
    // Determine the file type that should be loaded
    FREE_IMAGE_FORMAT fileFormat = deduce_img_format(filename.c_str());
    
    if (fileFormat == FIF_UNKNOWN) {
        LS_LOG_ERR("\tUnable to determine the file type for ", filename, ".\n");
        return false;
    }
    
    if (FreeImage_FIFSupportsReading(fileFormat) == false) {
        LS_LOG_ERR(
            "\tSupport for the type of file used by ", filename,
            " is not currently implemented.\n"
        );
        return false;
    }
    
    // Preliminary setup passed. Attempt to load the file data
    
    // Use some predefined image flags
    const int fileFlags = get_img_flags(fileFormat);
    FIBITMAP* fileData = FreeImage_Load(fileFormat, filename.c_str(), fileFlags);
    
    if (!fileData) {
        LS_LOG_ERR(
            "\tUnable to load the image ", filename,
            " due to an internal library error.\n"
        );
        return false;
    }
    
    const color_type_t dataType = get_bitmap_size(fileData);
    if (dataType == COLOR_TYPE_INVALID) {
        LS_LOG_ERR('\t', filename, " contains an unsupported pixel format.\n");
        FreeImage_Unload(fileData);
        return false;
    }
    
    this->pData         = reinterpret_cast<char*>(fileData);
    this->imgSize[0]    = (int)FreeImage_GetWidth(fileData);
    this->imgSize[1]    = (int)FreeImage_GetHeight(fileData);
    this->bitsPerPixel  = (unsigned)FreeImage_GetBPP(fileData);
    this->pixelType     = dataType;
    this->dataSize      = this->imgSize[0] * this->imgSize[1];
    
    get_pixel_format(fileData, this->bitsPerPixel, intFormat, extFormat);
    
    LS_LOG_MSG("\tSuccessfully loaded ", filename, ".\n");
    
    return true;
}

/*-------------------------------------
 * Unloading
-------------------------------------*/
void ImageResource::unload() {
    if (pData == nullptr) {
        return;
    }
    
    FreeImage_Unload(reinterpret_cast<FIBITMAP*>(pData));
    
    pData = nullptr;
    dataSize = 0;
    imgSize = math::vec2i{0};
    pixelType = COLOR_TYPE_DEFAULT;
    bitsPerPixel = 0;
    intFormat = COLOR_FMT_DEFAULT;
    extFormat = COLOR_LAYOUT_DEFAULT;
}

/*-------------------------------------
 * saving
-------------------------------------*/
bool ImageResource::save_file(const std::string& filename, img_file_t format) const {
    if (this->pData == nullptr) {
        return false;
    }
    
    FREE_IMAGE_FORMAT fiFormat = FIF_PNG;
    
    switch(format) {
        case img_file_t::IMG_FILE_BMP:    fiFormat = FIF_BMP; break;
        case img_file_t::IMG_FILE_EXR:    fiFormat = FIF_EXR; break;
        case img_file_t::IMG_FILE_GIF:    fiFormat = FIF_GIF; break;
        case img_file_t::IMG_FILE_HDR:    fiFormat = FIF_HDR; break;
        case img_file_t::IMG_FILE_ICO:    fiFormat = FIF_ICO; break;
        case img_file_t::IMG_FILE_JPG:    fiFormat = FIF_JPEG; break;
        case img_file_t::IMG_FILE_J2K:    fiFormat = FIF_J2K; break;
        case img_file_t::IMG_FILE_PNG:    fiFormat = FIF_PNG; break;
        case img_file_t::IMG_FILE_PPM:    fiFormat = FIF_PPM; break;
        case img_file_t::IMG_FILE_TGA:    fiFormat = FIF_TARGA; break;
        case img_file_t::IMG_FILE_TIF:    fiFormat = FIF_TIFF; break;
        //case img_file_t::IMG_FILE_WBP:    fiFormat = FIF_WEBP; break;
        case img_file_t::IMG_FILE_XPM:    fiFormat = FIF_XPM; break;
        default:                        fiFormat = FIF_PNG;
    }
    
    return 0 != FreeImage_Save(fiFormat, reinterpret_cast<FIBITMAP*>(pData), filename.c_str());
}

/*-------------------------------------
 * Get the data stored in pData
-------------------------------------*/
void* ImageResource::get_data() const {
    return (void*) FreeImage_GetBits(reinterpret_cast<FIBITMAP*>(pData));
}

} // end draw namespace
} // end ls namespace

