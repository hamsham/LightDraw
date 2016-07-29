
#include "ls/draw/DrawParams.h"



namespace ls {
namespace draw {

/*-------------------------------------
 * Constructor
-------------------------------------*/
DrawCommandParams::DrawCommandParams() noexcept :
    materialId{0},
    vaoId{0},
    drawFunc{draw_func_t::DRAW_ARRAYS},
    drawMode{draw_mode_t::DRAW_MODE_TRIS},
    indexType{index_element_t::INDEX_TYPE_NONE},
    first{0},
    count{0}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
DrawCommandParams::DrawCommandParams(const DrawCommandParams& d) noexcept :
    materialId{d.materialId},
    vaoId{d.vaoId},
    drawFunc{d.drawFunc},
    drawMode{d.drawMode},
    indexType{d.indexType}
{
    if (indexType != index_element_t::INDEX_TYPE_NONE) {
        offset = d.offset;
    }
    else {
        first = d.first;
    }
    
    count = d.count;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
DrawCommandParams::DrawCommandParams(DrawCommandParams&& d) noexcept :
    materialId{d.materialId},
    vaoId{d.vaoId},
    drawFunc{d.drawFunc},
    drawMode{d.drawMode},
    indexType{d.indexType}
{
    d.materialId = 0;
    d.vaoId = 0;

    if (indexType != index_element_t::INDEX_TYPE_NONE) {
        offset = d.offset;
    }
    else {
        first = d.first;
    }
    
    d.drawFunc = draw_func_t::DRAW_ARRAYS;
    d.drawMode = draw_mode_t::DRAW_MODE_TRIS;
    d.indexType = index_element_t::INDEX_TYPE_NONE;
    d.first = 0;
    
    count = d.count;
    d.count = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
DrawCommandParams& DrawCommandParams::operator=(const DrawCommandParams& d) noexcept {
    materialId = d.materialId;
    vaoId = d.vaoId;
    drawFunc = d.drawFunc;
    drawMode = d.drawMode;
    indexType = d.indexType;
    
    if (indexType != index_element_t::INDEX_TYPE_NONE) {
        offset = d.offset;
    }
    else {
        first = d.first;
    }
    
    count = d.count;
    
    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
DrawCommandParams& DrawCommandParams::operator=(DrawCommandParams&& d) noexcept {
    materialId = d.materialId;
    d.materialId = 0;
    
    vaoId = d.vaoId;
    d.vaoId = 0;
    
    drawFunc = d.drawFunc;
    drawMode = d.drawMode;
    indexType = d.indexType;

    if (indexType != index_element_t::INDEX_TYPE_NONE) {
        offset = d.offset;
    }
    else {
        first = d.first;
    }
    
    d.drawFunc = draw_func_t::DRAW_ARRAYS;
    d.drawMode = draw_mode_t::DRAW_MODE_TRIS;
    d.indexType = index_element_t::INDEX_TYPE_NONE;
    d.first = 0;
    
    count = d.count;
    d.count = 0;
    
    return *this;
}



/*-------------------------------------
 * Reset all internal data
-------------------------------------*/
void DrawCommandParams::reset() noexcept {
    materialId = 0;
    vaoId = 0;
    drawFunc = draw_func_t::DRAW_ARRAYS;
    drawMode = draw_mode_t::DRAW_MODE_DEFAULT;
    indexType = index_element_t::INDEX_TYPE_DEFAULT;
    first = 0;
    count = 0;
}



} // end draw namespace
} // end ls namespace
