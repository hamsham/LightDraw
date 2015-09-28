
#include <cassert>

#include "lightsky/draw/color.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Convert an InternalFormat to a standard format
-------------------------------------*/
pixel_layout_t getColorLayout(pixel_format_t internalFormat) {
    switch(internalFormat) {
        case COLOR_FMT_GRAY_8:
        case COLOR_FMT_GRAY_16I:
        //case COLOR_FMT_GRAY_16U:
        //case COLOR_FMT_GRAY_16F:
        case COLOR_FMT_GRAY_24:
        //case COLOR_FMT_GRAY_32I:
        //case COLOR_FMT_GRAY_32U:
        case COLOR_FMT_GRAY_32F:
            return COLOR_LAYOUT_GRAY;
            
        case COLOR_FMT_R_8:
        case COLOR_FMT_R_16I:
        case COLOR_FMT_R_16U:
        case COLOR_FMT_R_16F:
        case COLOR_FMT_R_32I:
        case COLOR_FMT_R_32U:
        case COLOR_FMT_R_32F:
        //case COLOR_FMT_R_C:
            return COLOR_LAYOUT_R;
            
        case COLOR_FMT_RG_8:
        case COLOR_FMT_RG_16I:
        case COLOR_FMT_RG_16U:
        case COLOR_FMT_RG_16F:
        case COLOR_FMT_RG_32I:
        case COLOR_FMT_RG_32U:
        case COLOR_FMT_RG_32F:
        //case COLOR_FMT_RG_C:
            return COLOR_LAYOUT_RG;
            
        case COLOR_FMT_RGB_8:
        case COLOR_FMT_RGB_16I:
        case COLOR_FMT_RGB_16U:
        case COLOR_FMT_RGB_16F:
        case COLOR_FMT_RGB_32I:
        case COLOR_FMT_RGB_32U:
        case COLOR_FMT_RGB_32F:
        //case COLOR_FMT_RGB_C:
            return COLOR_LAYOUT_RGB;
            
        case COLOR_FMT_RGBA_8:
        //case COLOR_FMT_RGBA_8_C:
        case COLOR_FMT_RGBA_16I:
        case COLOR_FMT_RGBA_16U:
        case COLOR_FMT_RGBA_16F:
        case COLOR_FMT_RGBA_32I:
        case COLOR_FMT_RGBA_32U:
        case COLOR_FMT_RGBA_32F:
        //case COLOR_FMT_RGBA_C:
            return COLOR_LAYOUT_RGBA;
            
        case COLOR_FMT_SRGB:
        case COLOR_FMT_SRGB_8:
        //case COLOR_FMT_SRGB_8_C:
            return COLOR_LAYOUT_RGB;
            
        //case COLOR_FMT_SRGBA_C:
        case COLOR_FMT_SRGBA_8:
        //case COLOR_FMT_SRGBA_8_C:
            return COLOR_LAYOUT_RGBA;
        
        case COLOR_FMT_DEFAULT:
            return COLOR_LAYOUT_DEFAULT;
        
        default:
            assert(false);
    }
    
    return COLOR_LAYOUT_DEFAULT;
}

} // end draw namespace
} // end ls namespace
