
#include <utility> // std::move


#include "ls/draw/BlendObject.h"
#include "ls/draw/DepthObject.h"
#include "ls/draw/IndexBuffer.h"
#include "ls/draw/SceneRenderData.h"
#include "ls/draw/Texture.h"
#include "ls/draw/TextureAssembly.h"
#include "ls/draw/VertexArray.h"
#include "ls/draw/VertexBuffer.h"

namespace ls {
namespace draw {



/*-------------------------------------
 * Destructor
-------------------------------------*/
SceneRenderData::~SceneRenderData() noexcept {
    terminate();
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
SceneRenderData::SceneRenderData() noexcept :
    vaos{},
    vbos{},
    ibos{},
    textures{},
    blendModes{},
    depthModes{}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
SceneRenderData::SceneRenderData(const SceneRenderData& r) noexcept :
    vaos{r.vaos},
    vbos{r.vbos},
    ibos{r.ibos},
    textures{r.textures},
    blendModes{r.blendModes},
    depthModes{r.depthModes}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
SceneRenderData::SceneRenderData(SceneRenderData&& r) noexcept :
    vaos{std::move(r.vaos)},
    vbos{std::move(r.vbos)},
    ibos{std::move(r.ibos)},
    textures{std::move(r.textures)},
    blendModes{std::move(r.blendModes)},
    depthModes{std::move(r.depthModes)}
{}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
SceneRenderData& SceneRenderData::operator =(const SceneRenderData& r) noexcept {
    vaos = r.vaos;
    vbos = r.vbos;
    ibos = r.ibos;
    textures = r.textures;
    blendModes = r.blendModes;
    depthModes = r.depthModes;
    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
SceneRenderData& SceneRenderData::operator =(SceneRenderData&& r) noexcept {
    vaos = std::move(r.vaos);
    vbos = std::move(r.vbos);
    ibos = std::move(r.ibos);
    textures = std::move(r.textures);
    blendModes = std::move(r.blendModes);
    depthModes = std::move(r.depthModes);
    return *this;
}

/*-------------------------------------
 * Terminate
-------------------------------------*/
struct terminator {
    terminator() {}
    ~terminator() {}
    template <class T>
    void operator()(std::vector<T>& data) const noexcept {
        for (T& obj : data) {
            obj.terminate();
        }
        data.clear();
    }
};

void SceneRenderData::terminate() noexcept {
    const terminator t1000;  // teehee
    t1000(vaos);
    t1000(vbos);
    t1000(ibos);
    t1000(textures);
    
    blendModes.clear();
    depthModes.clear();
}



} // end draw namespace
} // end ls namespace
