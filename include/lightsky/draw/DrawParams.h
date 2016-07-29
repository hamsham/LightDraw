
#ifndef __LS_DRAW_DRAW_PARAMS_H__
#define __LS_DRAW_DRAW_PARAMS_H__

#include <cstdint> // uint32_t

#include "ls/draw/Setup.h"
#include "ls/draw/VertexUtils.h"



namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Flags to determine how to render a submesh
-----------------------------------------------------------------------------*/
enum draw_func_t : uint32_t {
    DRAW_ARRAYS = 0x00000001,
    DRAW_ELEMENTS = 0x00000002,
    DRAW_INSTANCED = 0x10000000,
};



/*-----------------------------------------------------------------------------
 * Draw functions to be used for each submesh
-----------------------------------------------------------------------------*/
enum draw_mode_t : uint32_t {
    DRAW_MODE_POINTS        = GL_POINTS,
    DRAW_MODE_LINES         = GL_LINES,
    DRAW_MODE_LINE_STRIP    = GL_LINE_STRIP,
    DRAW_MODE_LINE_LOOP     = GL_LINE_LOOP,
    DRAW_MODE_TRIS          = GL_TRIANGLES,
    DRAW_MODE_TRI_STRIP     = GL_TRIANGLE_STRIP,
    DRAW_MODE_TRI_FAN       = GL_TRIANGLE_FAN,
    
    DRAW_MODE_DEFAULT       = DRAW_MODE_TRIS,
};



/*-----------------------------------------------------------------------------
 * Trivially-Copyable Parameters to render a single subset of geometry through
 * OpenGL.
-----------------------------------------------------------------------------*/
struct alignas(32) DrawCommandParams {
    /**
     * Indexed location of *this draw command's material within a scene graph.
     */
    uint32_t materialId;
    
    /**
     * @brief Non-owning handle to the VAO which can be bound in order to
     * render vertices specified in *this.
     */
    uint32_t vaoId;
    
    /**
     * @brief An enumeration to determine what OpenGL function should be used
     * to render the vertices referenced by *this.
     */
    draw_func_t drawFunc;

    /**
     * @brief Enumeration which can be used to determine if the vertices
     * referenced by *this should be rendered as points, lines, or triangles.
     */
    draw_mode_t drawMode;
    
    /**
     * @brief Enumeration to determine what type of index values are contained
     * within the referenced IBO, if they exist.
     */
    index_element_t indexType;

    /*
     * Union to contain the parameters for the current draw function.
     */
    union alignas(8) {
        /**
         * Use this member when rendering with a call to glDrawArrays().
         */
        uint64_t first;
        
        /**
         * Use this member when rendering with glDrawElements().
         */
        void* offset;
    };

    /**
     * For both glDrawArrays() and glDrawElements(), this member will determine
     * the number of total vertices or indices to render.
     */
    uint32_t count;
    
    /**
     * Destructor
     */
    ~DrawCommandParams() noexcept = default;
    
    /**
     * Constructor
     * 
     * Initializes all internal members to their default values
     */
    DrawCommandParams() noexcept;
    
    /**
     * Copy Constructor
     * 
     * Copies all data from the input parameter into *this.
     */
    DrawCommandParams(const DrawCommandParams& d) noexcept;
    
    /**
     * Move Constructor
     * 
     * Moves all data from the input parameter into *this, then resets the
     * input parameter's members to their default values.
     */
    DrawCommandParams(DrawCommandParams&& d) noexcept;
    
    /**
     * Copy Operator
     * 
     * Copies all data from the input parameter into *this.
     * 
     * @return A reference to *this.
     */
    DrawCommandParams& operator=(const DrawCommandParams& d) noexcept;
    
    /**
     * Move Operator
     * 
     * Moves all data from the input parameter into *this, then resets the
     * input parameter's members to their default values.
     * 
     * @return A reference to *this.
     */
    DrawCommandParams& operator=(DrawCommandParams&& d) noexcept;
    
    /**
     * @brief Reset all internal members to their default values.
     */
    void reset() noexcept;
};



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_DRAW_PARAMS_H__ */
