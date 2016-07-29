
#ifndef __LS_DRAW_SCENE_RENDER_DATA_H__
#define __LS_DRAW_SCENE_RENDER_DATA_H__

#include <vector>

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class BlendObject;
class DepthObject;
class IndexBuffer;
class Texture;
class VertexArray;
class VertexBuffer;



/**----------------------------------------------------------------------------
 * SceneRenderData Class
-----------------------------------------------------------------------------*/
struct SceneRenderData {
    std::vector<VertexArray> vaos;
    
    std::vector<VertexBuffer> vbos;
    
    std::vector<IndexBuffer> ibos;
    
    std::vector<Texture> textures;
    
    std::vector<BlendObject> blendModes;
    
    std::vector<DepthObject> depthModes;
    
    
    
    /**
     * @brief Destructor
     * 
     * Calls 'terminate()' to delete all CPU and GPU-side resources.
     */
    ~SceneRenderData() noexcept;
    
    /**
     * Constructor
     * 
     * Initializes all members in *this to their default values.
     */
    SceneRenderData() noexcept;
    
    /**
     * Copy Constructor
     * 
     * @param r
     * A constant reference to another SceneRenderData object which will be
     * used to initialize *this.
     * 
     * This function will incur a large runtime overhead in order to copy all
     * dynamically allocated resources on both the CPU and GPU.
     */
    SceneRenderData(const SceneRenderData& r) noexcept;
    
    /**
     * Move Constructor
     * 
     * Initializes *this using data from the input parameter. No copies or data
     * reallocations will be performed.
     * 
     * @param r
     * An r-value reference to another SceneRenderData object who's data will
     * be moved into *this.
     */
    SceneRenderData(SceneRenderData&& r) noexcept;
    
    /**
     * Copy Constructor
     * 
     * @param r
     * A constant reference to another SceneRenderData object which will be
     * used to initialize *this.
     * 
     * This function will incur a large runtime overhead in order to copy all
     * dynamically allocated resources on both the CPU and GPU.
     * 
     * @return A reference to *this.
     */
    SceneRenderData& operator=(const SceneRenderData& r) noexcept;
    
    /**
     * Move Operator
     * 
     * Moves all data from the input parameter into *this. No copies or data
     * reallocations will be performed.
     * 
     * @param r
     * An r-value reference to another SceneRenderData object who's data will
     * be moved into *this.
     * 
     * @return A reference to *this.
     */
    SceneRenderData& operator=(SceneRenderData&& r) noexcept;
    
    /**
     * @brief Terminate A SceneRenderData object by cleaning up all CPu and
     * GPU-side resources.
     */
    void terminate() noexcept;
};



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_SCENE_RENDER_DATA_H__ */
