
#include <utility> // std::move
#include <memory> // std::nothrow
#include <string>
#include <cstring> // strcmp()

#include "lightsky/draw/Camera.h"
#include "lightsky/draw/Color.h"
#include "lightsky/draw/ImageResource.h"
#include "lightsky/draw/IndexBuffer.h"
#include "lightsky/draw/PackedVertex.h"
#include "lightsky/draw/SceneFileLoader.h"
#include "lightsky/draw/SceneFileUtility.h"
#include "lightsky/draw/SceneMaterial.h"
#include "lightsky/draw/TextureAssembly.h"
#include "lightsky/draw/Transform.h"
#include "lightsky/draw/VAOAssembly.h"
#include "lightsky/draw/VAOAttrib.h"
#include "lightsky/draw/VertexArray.h"
#include "lightsky/draw/VertexBuffer.h"
#include "lightsky/draw/VertexUtils.h"



namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * SceneGraph Class
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
VboGroupMarker::~VboGroupMarker() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
VboGroupMarker::VboGroupMarker() noexcept :
    vertType{(common_vertex_t)0},
    numVboBytes{0},
    vboOffset{0},
    meshOffset{0},
    baseVert{0}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
VboGroupMarker::VboGroupMarker(const VboGroupMarker& v) noexcept :
    vertType{v.vertType},
    numVboBytes{v.numVboBytes},
    vboOffset{v.vboOffset},
    meshOffset{v.meshOffset},
    baseVert{v.baseVert}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
VboGroupMarker::VboGroupMarker(VboGroupMarker&& v) noexcept :
    vertType{v.vertType},
    numVboBytes{v.numVboBytes},
    vboOffset{v.vboOffset},
    meshOffset{v.meshOffset},
    baseVert{v.baseVert}
{
    v.vertType = (common_vertex_t)0;
    v.numVboBytes = 0;
    v.vboOffset = 0;
    v.meshOffset = 0;
    v.baseVert = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
VboGroupMarker& VboGroupMarker::operator=(const VboGroupMarker& v) noexcept {
    vertType = v.vertType;
    numVboBytes = v.numVboBytes;
    vboOffset = v.vboOffset;
    meshOffset = v.meshOffset;
    baseVert = v.baseVert;
    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
VboGroupMarker& VboGroupMarker::operator=(VboGroupMarker&& v) noexcept {
    vertType = v.vertType;
    v.vertType = (common_vertex_t)0;

    numVboBytes = v.numVboBytes;
    v.numVboBytes = 0;

    vboOffset = v.vboOffset;
    v.vboOffset = 0;

    meshOffset = v.meshOffset;
    v.meshOffset = 0;

    baseVert = v.baseVert;
    v.baseVert = 0;

    return *this;
}



/*-----------------------------------------------------------------------------
 * SceneFilePreLoader Class
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
SceneFilePreLoader::~SceneFilePreLoader() noexcept {
    unload();
}



/*-------------------------------------
 * Constructor
-------------------------------------*/
SceneFilePreLoader::SceneFilePreLoader() noexcept :
    filepath{},
    importer{nullptr},
    sceneInfo{},
    sceneData{},
    baseFileDir{"./"},
    vboMarkers{},
    texturePaths{}
{}



/*-------------------------------------
 * SceneResource Move Constructor
-------------------------------------*/
SceneFilePreLoader::SceneFilePreLoader(SceneFilePreLoader&& s) noexcept :
    filepath{std::move(s.filepath)},
    importer{std::move(s.importer)},
    sceneInfo{std::move(s.sceneInfo)},
    sceneData{std::move(s.sceneData)},
    baseFileDir{std::move(s.baseFileDir)},
    vboMarkers{std::move(s.vboMarkers)},
    texturePaths{std::move(s.texturePaths)}
{}



/*-------------------------------------
 * Mesh Loader move operator
-------------------------------------*/
SceneFilePreLoader& SceneFilePreLoader::operator=(SceneFilePreLoader&& s) noexcept {
    unload();

    filepath = std::move(s.filepath);
    importer = std::move(s.importer);
    sceneInfo = std::move(s.sceneInfo);
    sceneData = std::move(s.sceneData);
    baseFileDir = std::move(s.baseFileDir);
    vboMarkers = std::move(s.vboMarkers);
    texturePaths = std::move(s.texturePaths);

    return *this;
}



/*-------------------------------------
 * SceneResource Destructor
-------------------------------------*/
void SceneFilePreLoader::unload() noexcept {
    filepath.clear();

    importer.reset();

    sceneInfo = SceneFileMetaData{};

    sceneData.terminate();

    baseFileDir = "./";

    vboMarkers.clear();

    texturePaths.clear();
}



/*-------------------------------------
 * Load a set of meshes from a file
-------------------------------------*/
bool SceneFilePreLoader::load(const std::string& filename) noexcept {
    unload();

    LS_LOG_MSG("Attempting to load 3D mesh file ", filename, '.');

    // load
    importer.reset(new Assimp::Importer);

    Assimp::Importer& fileImporter = *importer.get();
    //fileImporter.SetPropertyBool(AI_CONFIG_IMPORT_NO_SKELETON_MESHES, true);
    fileImporter.SetPropertyBool(AI_CONFIG_PP_FD_REMOVE, true); // remove degenerate triangles
    fileImporter.SetPropertyInteger(AI_CONFIG_FAVOUR_SPEED, AI_TRUE);

    if (!fileImporter.ReadFile(filename.c_str(), SCENE_FILE_IMPORT_FLAGS)) {
        LS_LOG_ERR(
            "\tError: Unable to load the mesh file ", filename,
            " due to an import error:\n\t", fileImporter.GetErrorString(), '\n'
        );
        unload();
        return false;
    }
    else {
        LS_LOG_MSG("\tMesh file successfully imported. Running post-process optimization.");

        const std::string::size_type baseDirIndex = filename.find_last_of(u8R"(\/)");
        if (baseDirIndex != std::string::npos) {
            baseFileDir = filename.substr(0, baseDirIndex+1);
        }
    }

    const aiScene* const pScene = preload_mesh_data();
    if (!pScene) {
        LS_LOG_ERR(
            "\tError: Failed to process the 3D mesh file ",
            filename, " in memory.\n"
        );
        unload();
        return false;
    }

    if (!allocate_cpu_data(pScene)) {
        LS_LOG_ERR(
            "\tError: Failed to allocate data for the 3D mesh file ",
            filename, ".\n"
        );
        unload();
        return false;
    }

    LS_LOG_MSG(
        "\tDone. Successfully loaded the scene file \"", filename, ".\"",
        "\n\t\tTotal Meshes:     ", sceneData.meshes.size(),
        "\n\t\tTotal Textures:   ", sceneData.renderData.textures.size(),
        "\n\t\tTotal Nodes:      ", sceneData.nodes.size(),
        "\n\t\tTotal Cameras:    ", sceneData.cameras.size(),
        "\n\t\tTotal Animations: ", sceneData.animations.size(),
        '\n'
    );

    filepath = filename;

    return true;
}



/*-------------------------------------
 * Verify *this contains data to pass to a SceneFileLoader
-------------------------------------*/
bool SceneFilePreLoader::is_loaded() const noexcept {
    return importer.get() && importer->GetScene() != nullptr;
}



/*-------------------------------------
 * Perform a pre-processing step in
 * order to ensure a one-time
 * allocation of vertex+index data.
-------------------------------------*/
const aiScene* SceneFilePreLoader::preload_mesh_data() noexcept {
    const aiScene* const pScene = importer->GetScene();

    if (!pScene) {
        LS_LOG_ERR("\tERROR: Unable to pre-process a scene file in-memory.");
        return nullptr;
    }

    for (unsigned meshIter = 0; meshIter < pScene->mNumMeshes; ++meshIter) {
        const aiMesh* const pMesh           = pScene->mMeshes[meshIter];
        const common_vertex_t inVertType    = convert_assimp_verts(pMesh);
        VboGroupMarker* outMeshMarker       = get_matching_marker(inVertType, vboMarkers);

        // Keep track of where in the output VBO a mesh's data should be placed.
        // Use the following information to contiguously group all mesh
        // vertices of similar types within the output VBO
        if (!outMeshMarker) {
            vboMarkers.push_back(VboGroupMarker{});
            outMeshMarker               = &vboMarkers[vboMarkers.size() - 1];
            outMeshMarker->vertType     = inVertType;
            outMeshMarker->numVboBytes  = 0;
            outMeshMarker->vboOffset    = 0;
            outMeshMarker->meshOffset   = 0;
            outMeshMarker->baseVert     = 0;
        }

        const unsigned numMeshVerts = pMesh->mNumVertices;
        sceneInfo.totalVertices += numMeshVerts;

        const unsigned numMeshBytes = numMeshVerts * get_vertex_byte_size(outMeshMarker->vertType);
        outMeshMarker->numVboBytes += numMeshBytes;
        sceneInfo.totalVboBytes += numMeshBytes;

        // NOTE: I don't believe grouping indices by type in a single IBO
        // should matter, it's possible they can take advantage of the GPU's
        // caches but we'll hold off on that for now.
        unsigned numIndices = 0;
        for (unsigned faceIter = 0; faceIter < pMesh->mNumFaces; ++faceIter) {
            numIndices += pMesh->mFaces[faceIter].mNumIndices;
        }
        sceneInfo.totalIndices += numIndices;
    }

    sceneInfo.indexType = get_required_index_type(sceneInfo.totalIndices);
    sceneInfo.totalIboBytes = get_index_byte_size(sceneInfo.indexType) * sceneInfo.totalIndices;

    // calculate all of the vertex strides
    unsigned totalVboOffset = 0;
    for (VboGroupMarker& m : this->vboMarkers) {
        m.vboOffset = totalVboOffset;
        totalVboOffset += m.numVboBytes;
    }

    LS_LOG_MSG(
        "\tScene File Memory requirements:",
        "\n\t\tVBO Byte Size:   ", sceneInfo.totalVboBytes,
        "\n\t\tVertex Count:    ", sceneInfo.totalVertices,
        "\n\t\tIBO Byte Size:   ", sceneInfo.totalIboBytes,
        "\n\t\tIndex Count:     ", sceneInfo.totalIndices,
        "\n\t\tVAO Count:       ", vboMarkers.size()
    );
    for (unsigned i = 0; i < vboMarkers.size(); ++i) {
        VboGroupMarker& m = vboMarkers[i];
        LS_LOG_MSG("\t\t                 VAO ", i, ": 0x", std::hex, m.vertType, std::dec);

        for (unsigned i = 0; i < COMMON_VERTEX_NAMES_COUNT; ++i) {
            if (!!(m.vertType & COMMON_VERTEX_FLAGS_LIST[i])) {
                LS_LOG_MSG("\t\t                        ", get_common_vertex_names()[i]);
            }
        }
    }

    return pScene;
}



/*-------------------------------------
 * Allocate all required CPU-side memory for a scene.
-------------------------------------*/
bool SceneFilePreLoader::allocate_cpu_data(const aiScene* const pScene) noexcept {
    sceneData.cameras.resize(pScene->mNumCameras);
    sceneData.meshes.resize(pScene->mNumMeshes);
    sceneData.materials.resize(pScene->mNumMaterials);

    for (SceneMaterial& m : sceneData.materials) {
        m.reset();
    }

    // reserve space for textures using ASSIMP's materials count, the
    // "pScene->mNumTextures" member only counts textures saved into the file
    // itself. Materials store file paths.
    GLContextData& renderData = sceneData.renderData;
    renderData.textures.reserve(pScene->mNumMaterials);

    texturePaths.reserve(pScene->mNumMaterials);

    // Reserve data here. There's no telling whether all nodes can be imported
    // or not while Assimp's bones and lights remain unsupported.
    const unsigned numSceneNodes = count_assimp_nodes(pScene->mRootNode);
    sceneData.bounds.reserve(numSceneNodes);
    sceneData.nodes.reserve(numSceneNodes);
    sceneData.baseTransforms.reserve(numSceneNodes);
    sceneData.currentTransforms.reserve(numSceneNodes);
    sceneData.nodeNames.reserve(numSceneNodes);
    sceneData.animations.resize(pScene->mNumAnimations);
    sceneData.nodeMeshCounts.reserve(pScene->mNumMeshes);
    sceneData.nodeMeshes.reserve(pScene->mNumMeshes);

    return true;
}



/*-----------------------------------------------------------------------------
 * SceneFileLoader Class
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
SceneFileLoader::~SceneFileLoader() noexcept {
}



/*-------------------------------------
 * Constructor
-------------------------------------*/
SceneFileLoader::SceneFileLoader() noexcept :
    preloader{}
{}



/*-------------------------------------
 * SceneResource Move Constructor
-------------------------------------*/
SceneFileLoader::SceneFileLoader(SceneFileLoader&& s) noexcept :
    preloader{std::move(s.preloader)}
{}



/*-------------------------------------
 * Mesh Loader move operator
-------------------------------------*/
SceneFileLoader& SceneFileLoader::operator=(SceneFileLoader&& s) noexcept {
    preloader = std::move(s.preloader);

    return *this;
}



/*-------------------------------------
 * SceneResource Destructor
-------------------------------------*/
void SceneFileLoader::unload() noexcept {
    preloader.unload();
}



/*-------------------------------------
 * Load a set of meshes from a file
-------------------------------------*/
bool SceneFileLoader::load(const std::string& filename) noexcept {
    unload();

    if (!preloader.load(filename)) {
        return false;
    }

    return load_scene(preloader.importer->GetScene());
}



/*-------------------------------------
 * Load a set of meshes from a file
-------------------------------------*/
bool SceneFileLoader::load(SceneFilePreLoader&& p) noexcept {
    LS_DEBUG_ASSERT(&p != &preloader);

    unload();

    if (p.is_loaded()) {
        preloader = std::move(p);
        return load_scene(preloader.importer->GetScene());
    }

    return false;
}



/*-------------------------------------
 * Load a set of meshes from a file
-------------------------------------*/
bool SceneFileLoader::load_scene(const aiScene* const pScene) noexcept {
    const std::string& filename = preloader.filepath;
    SceneGraph& sceneData = preloader.sceneData;

    LS_LOG_MSG("\tAllocating GPU memory for 3D scene data.");
    if (!allocate_gpu_data()) {
        unload();
        LS_LOG_ERR("\t\tUnable to initialize 3D scene data on the CPU.\n");
        return false;
    }

    if (!import_materials(pScene)) {
        LS_LOG_ERR("\tError: Unable to load materials for the 3D mesh ", filename, "!\n");
        unload();
        return false;
    }

    if (!import_mesh_data(pScene)) {
        LS_LOG_ERR("\tError: Failed to load the 3D mesh ", filename, "!\n");
        unload();
        return false;
    }

    read_node_hierarchy(pScene, pScene->mRootNode, scene_property_t::SCENE_GRAPH_ROOT_ID);

    if (!import_animations(pScene)) {
      LS_LOG_ERR("\tWarning: Failed to animations from ", filename, "!\n");
    }

    LS_LOG_MSG(
        "\tDone. Successfully loaded the scene file \"", filename, ".\"",
        "\n\t\tTotal Meshes:     ", sceneData.meshes.size(),
        "\n\t\tTotal Textures:   ", sceneData.renderData.textures.size(),
        "\n\t\tTotal Nodes:      ", sceneData.nodes.size(),
        "\n\t\tTotal Cameras:    ", sceneData.cameras.size(),
        "\n\t\tTotal Animations: ", sceneData.animations.size(),
        '\n'
    );

    return true;
}



/*-------------------------------------
 * Allocate all required GPU-side memory for a scene.
-------------------------------------*/
bool SceneFileLoader::allocate_gpu_data() noexcept {
    SceneGraph& sceneData = preloader.sceneData;
    std::vector<VboGroupMarker> vboMarkers = preloader.vboMarkers;
    SceneFileMetaData& sceneInfo = preloader.sceneInfo;
    GLContextData& renderData = sceneData.renderData;

    VertexBuffer vbo;
    IndexBuffer ibo;

    if (!vboMarkers.size()) {
        LS_LOG_MSG("\t\tNo Vertex types available to load.");
        return true;
    }

    unsigned totalMeshTypes = vboMarkers.size();
    utils::Pointer<common_vertex_t[]> vertTypes{new common_vertex_t[totalMeshTypes]};

    // initialize the VBO attributes
    for (unsigned i = 0; i < vboMarkers.size(); ++i) {
        vertTypes[i] = vboMarkers[i].vertType;
    }

    if (sceneInfo.totalVertices) {
        if (!vbo.init() || !vbo.setup_attribs(vertTypes.get(), totalMeshTypes)) {
            LS_LOG_ERR("\t\tFailed to initialize a VBO to hold all mesh data for the currently loading scene file.");
            return false;
        }

        vbo.bind();
        vbo.set_data(sceneInfo.totalVboBytes, nullptr, buffer_access_t::VBO_STATIC_DRAW);
        vbo.unbind();

        LS_LOG_MSG("\t\tAllocated ", sceneInfo.totalVboBytes, " bytes for ", vboMarkers.size(), " types of vertices.");
    }

    if (sceneInfo.totalIndices) {
        // We're only creating one IBO for loading mesh data
        if (!ibo.init() || !ibo.setup_attribs(1)) {
            vbo.terminate();
            LS_LOG_ERR("\t\tFailed to initialize a IBO to hold all mesh data for the currently loading scene file.");
            return false;
        }

        ibo.bind();
        ibo.set_data(sceneInfo.totalIboBytes, nullptr, buffer_access_t::VBO_STATIC_DRAW);
        ibo.unbind();
        LS_LOG_MSG("\t\tAllocated ", sceneInfo.totalIboBytes, " bytes for indices.");
    }

    utils::Pointer<VAOAssembly> assembly{new(std::nothrow) VAOAssembly{}};

    if (!assembly) {
        LS_LOG_ERR("\t\tFailed to instantiate a VAO assembly pipeline for the currently loading scene file.");
        vbo.terminate();
        ibo.terminate();
        return false;
    }

    // Start adding the mesh descriptors and GL handles
    VAODataList& vaos = renderData.vaos;
    vaos.clear();
    vaos.reserve(totalMeshTypes);

    unsigned currentVboAttribId = 0;
    const char* const* attribNames = get_common_vertex_names();

    for (unsigned i = 0; i < totalMeshTypes; ++i) {
        VertexArray vao{};
        common_vertex_t inAttribs   = vertTypes[i];
        unsigned currentVaoAttribId = 0;
        VboGroupMarker& m           = vboMarkers[i];

        assembly->clear();

        for (unsigned k = 0; k < COMMON_VERTEX_FLAGS_COUNT; ++k) {
            if (0 != (inAttribs & COMMON_VERTEX_FLAGS_LIST[k])) {
                VBOAttrib& a = vbo.get_attrib(currentVboAttribId);
                a.set_offset((void*)(ptrdiff_t)(m.vboOffset + get_vertex_attrib_offset(inAttribs, COMMON_VERTEX_FLAGS_LIST[k])));

                assembly->set_vbo_attrib(currentVaoAttribId, vbo, currentVboAttribId);
                assembly->set_attrib_name(currentVaoAttribId, attribNames[k]);
                currentVboAttribId++;
                currentVaoAttribId++;
            }
        }

        if (ibo.is_valid()) {
            assembly->set_ibo_attrib(ibo);
        }

        LS_ASSERT(assembly->assemble(vao));
        vaos.add(std::move(vao));
    }

    renderData.vbos.add(std::move(vbo));
    renderData.ibos.add(std::move(ibo));

    return true;
}



/*-------------------------------------
 * Import all materials used by the imported meshes
-------------------------------------*/
bool SceneFileLoader::import_materials(const aiScene* const pScene) noexcept {
    static constexpr aiTextureType texTypes[] = {
        aiTextureType_DIFFUSE,
        aiTextureType_HEIGHT,
        aiTextureType_SPECULAR,
        aiTextureType_AMBIENT,
        aiTextureType_EMISSIVE,
        aiTextureType_NORMALS,
        aiTextureType_SHININESS,
        aiTextureType_OPACITY,
        aiTextureType_DISPLACEMENT,
        aiTextureType_LIGHTMAP,
        aiTextureType_REFLECTION,
        aiTextureType_UNKNOWN
    };

    const unsigned numMaterials = pScene->mNumMaterials;

    LS_LOG_MSG("\tImporting ", numMaterials, " materials from the imported mesh.");

    if (!numMaterials) {
        LS_LOG_MSG("\t\tDone.");
        return true;
    }

    SceneGraph& sceneData                       = preloader.sceneData;
    std::vector<SceneMaterial>& materials       = sceneData.materials;
    utils::Pointer<TextureAssembly> texMaker    {new TextureAssembly{}};
    utils::Pointer<ImageResource> imgLoader     {new ImageResource{}};

    for (unsigned i = 0; i < numMaterials; ++i) {
        const aiMaterial* const pMaterial = pScene->mMaterials[i];
        SceneMaterial& newMaterial = materials[i];

        for (unsigned j = 0; j < LS_ARRAY_SIZE(texTypes); ++j) {
            import_texture_path(pMaterial, texTypes[j], newMaterial, *imgLoader, *texMaker);
        }
    }

    LS_LOG_MSG("\t\tDone.");
    return true;
}



/*-------------------------------------
    Read and import a single texture path
-------------------------------------*/
void SceneFileLoader::import_texture_path(
    const aiMaterial* const pMaterial,
    const int slotType,
    SceneMaterial& outMaterial,
    ImageResource& imgLoader,
    TextureAssembly& texAssembly
) noexcept {
    imgLoader.unload();
    texAssembly.clear();

    SceneGraph& sceneData       = preloader.sceneData;
    GLContextData& renderData   = sceneData.renderData;
    TextureDataList& textures   = renderData.textures;
    const unsigned maxTexCount  = pMaterial->GetTextureCount((aiTextureType)slotType);

    switch (slotType) {
        case aiTextureType::aiTextureType_DIFFUSE:
            LS_LOG_MSG("\t\tLocated ", maxTexCount, " diffuse textures.");
            break;
        case aiTextureType::aiTextureType_HEIGHT:
            LS_LOG_MSG("\t\tLocated ", maxTexCount, " normal maps.");
            break;
        case aiTextureType::aiTextureType_SPECULAR:
            LS_LOG_MSG("\t\tLocated ", maxTexCount, " specular maps.");
            break;

        case aiTextureType::aiTextureType_AMBIENT:
            LS_LOG_MSG("\t\tLocated ", maxTexCount, " ambient textures.");
            break;

        // TODO: other textures are unsupported at the moment.
        default:
            LS_LOG_MSG("\t\tLocated ", maxTexCount, " miscellaneous textures.");
            break;
    }

    // Get an offset to the next texture's index within the current material
    unsigned bindSlotOffset = 0;
    for (unsigned i = 0; i < active_texture_t::MAX_ACTIVE_TEXTURES; ++i) {
        if (outMaterial.bindSlots[i] == (int)material_property_t::INVALID_MATERIAL_TEXTURE) {
            bindSlotOffset = i;
            break;
        }
    }

    // iterate
    aiString inPath;
    aiTextureMapMode inWrapMode;

    for (unsigned i = 0; i < maxTexCount; ++i) {

        const unsigned activeTexSlot = i+bindSlotOffset;

        // Insurance... Allow the texture to be loaded anyway in case the active
        // binding may change at a later time.
        if (activeTexSlot < active_texture_t::MAX_ACTIVE_TEXTURES) {
            outMaterial.bindSlots[activeTexSlot] = tex_slot_t::TEXTURE_SLOT_GPU_OFFSET + activeTexSlot;
        }

        inPath.Clear();

        if (pMaterial->GetTexture((aiTextureType)slotType, i, &inPath, nullptr, nullptr, nullptr, nullptr, &inWrapMode) != aiReturn_SUCCESS) {
            LS_LOG_ERR("\t\t\tFailed to load the texture ", inPath.C_Str());
            continue;
        }

        // add the imported texture to the appropriate array in textureSet.
        const std::string& baseFileDir = preloader.baseFileDir;
        const std::string texPath{baseFileDir + inPath.C_Str()};
        std::unordered_map<std::string, unsigned>& texturePaths = preloader.texturePaths;
        const std::unordered_map<std::string, unsigned>::const_iterator& iter = texturePaths.find(texPath);
        GLuint texId = 0;

        if (iter != texturePaths.cend()) {
            LS_LOG_MSG("\t\t\tDuplicate texture detected: ", texPath);
            const Texture& loadedTex = renderData.textures[iter->second];
            texId = loadedTex.gpu_id();
        }
        else {
            const tex_wrap_t wrapMode = convert_assimp_tex_wrapping(inWrapMode);
            const unsigned texIndex = load_texture_at_path(texPath, imgLoader, texAssembly, wrapMode);

            if (texIndex != material_property_t::INVALID_MATERIAL_TEXTURE) {
                texId = textures[texIndex].gpu_id();
                texturePaths[texPath] = texIndex;
            }
            else {
                // only set the texture to 0, don't modify the bind slot in
                // case something still needs to be rendered. OpenGL will just
                // use a black texture.
                texId = 0;
            }
        }

        // redundancy
        if (activeTexSlot >= active_texture_t::MAX_ACTIVE_TEXTURES) {
            LS_LOG_ERR("\t\t\tWarning: Texture ", texPath, " may not be used at this time. Too many texture slots have been used already.");
        }
        else {
            outMaterial.textures[activeTexSlot] = texId;
        }
    }
}



/*-------------------------------------
 * Attempt to load a texture from the local filesystem
-------------------------------------*/
unsigned SceneFileLoader::load_texture_at_path(
    const std::string& path,
    ImageResource& imgLoader,
    TextureAssembly& texAssembly,
    const tex_wrap_t wrapMode
) noexcept {
    TextureDataList& textures = preloader.sceneData.renderData.textures;
    Texture outTex;

    if (!imgLoader.load_file(path)) {
        return material_property_t::INVALID_MATERIAL_TEXTURE;
    }

    LS_ASSERT(texAssembly.set_size_attrib(imgLoader.get_pixel_size()));
    LS_ASSERT(texAssembly.set_format_attrib(imgLoader.get_internal_format()));

    LS_ASSERT(texAssembly.set_int_attrib(tex_param_t::TEX_PARAM_WRAP_S, wrapMode));
    LS_ASSERT(texAssembly.set_int_attrib(tex_param_t::TEX_PARAM_WRAP_T, wrapMode));
    LS_ASSERT(texAssembly.set_int_attrib(tex_param_t::TEX_PARAM_WRAP_R, wrapMode));

    LS_ASSERT(texAssembly.set_int_attrib(tex_param_t::TEX_PARAM_MAG_FILTER, tex_filter_t::TEX_FILTER_LINEAR));
    LS_ASSERT(texAssembly.set_int_attrib(tex_param_t::TEX_PARAM_MIN_FILTER, tex_filter_t::TEX_FILTER_LINEAR));

    // OpenGL ES doesn't support the GL_BGR & GL_BGRA storage formats
    #ifdef LS_DRAW_BACKEND_GLES
        if (imgLoader.get_internal_format() == pixel_format_t::COLOR_FMT_DEFAULT_RGB
        || imgLoader.get_internal_format() == pixel_format_t::COLOR_FMT_DEFAULT_RGBA
        ) {
            LS_ASSERT(texAssembly.set_int_attrib(tex_param_t::TEX_PARAM_SWIZZLE_R, pixel_swizzle_t::SWIZZLE_BLUE));
            LS_ASSERT(texAssembly.set_int_attrib(tex_param_t::TEX_PARAM_SWIZZLE_G, pixel_swizzle_t::SWIZZLE_GREEN));
            LS_ASSERT(texAssembly.set_int_attrib(tex_param_t::TEX_PARAM_SWIZZLE_B, pixel_swizzle_t::SWIZZLE_RED));
        }
    #endif

    if (texAssembly.assemble(outTex, imgLoader.get_data())) {
        textures.add(std::move(outTex));
        return textures.size()-1;
    }

    return material_property_t::INVALID_MATERIAL_TEXTURE;
}



/*-------------------------------------
 * Use all information from the
 * preprocess step to allocate and
 * import mesh information
-------------------------------------*/
bool SceneFileLoader::import_mesh_data(const aiScene* const pScene) noexcept {
    std::vector<VboGroupMarker> tempVboMarks    = preloader.vboMarkers;
    SceneGraph& sceneData                       = preloader.sceneData;
    GLContextData& renderData                   = sceneData.renderData;
    const SceneFileMetaData& sceneInfo          = preloader.sceneInfo;

    LS_LOG_MSG("\tImporting vertices and indices of individual meshes from a file.");

    VertexBuffer& vbo   = renderData.vbos.back();
    IndexBuffer& ibo    = renderData.ibos.back();
    unsigned baseIndex  = 0;
    char* const pVbo    = map_scene_file_buffer(vbo, sceneInfo.totalVboBytes);
    char* pIbo          = map_scene_file_buffer(ibo, sceneInfo.totalIboBytes);

    if (!pVbo || !pIbo) {
        vbo.unmap_data();
        vbo.unbind();
        ibo.unmap_data();
        ibo.unbind();
        return false;
    }

    std::vector<SceneMesh>& meshes = sceneData.meshes;

    // vertex data in ASSIMP is not interleaved. It has to be converted into
    // the internally used vertex format which is recommended for use on mobile
    // devices.
    for (unsigned meshId = 0; meshId < pScene->mNumMeshes; ++meshId) {
        const aiMesh* const pMesh       = pScene->mMeshes[meshId];
        const common_vertex_t vertType  = convert_assimp_verts(pMesh);

        const unsigned meshGroupId      = get_mesh_group_marker(vertType, preloader.vboMarkers);
        VboGroupMarker& meshGroup       = tempVboMarks[meshGroupId];
        LS_DEBUG_ASSERT                (meshGroup.vertType == vertType);

        SceneMesh& mesh                 = meshes[meshId];
        mesh.drawParams.materialId      = pMesh->mMaterialIndex;
        mesh.drawParams.vaoId           = renderData.vaos[meshGroupId].gpu_id();
        mesh.vboId                      = vbo.gpu_id();
        mesh.iboId                      = ibo.gpu_id();

        MeshMetaData& metaData          = mesh.metaData;
        metaData.vertTypes              = meshGroup.vertType;
        metaData.totalVerts             = pMesh->mNumVertices;
        const unsigned meshOffset       = meshGroup.vboOffset + meshGroup.meshOffset;

        upload_mesh_vertices(pMesh, pVbo + meshOffset, meshGroup.vertType);

        meshGroup.meshOffset            += metaData.calc_total_vertex_bytes();
        metaData.indexType              = sceneInfo.indexType;
        pIbo                            = upload_mesh_indices(pMesh, pIbo, baseIndex, meshGroup.baseVert, mesh);
        meshGroup.baseVert              += metaData.totalVerts;
        baseIndex                       += metaData.calc_total_index_bytes();
    }

    vbo.unmap_data();
    vbo.unbind();
    ibo.unmap_data();
    ibo.unbind();

    LS_LOG_MSG("\t\tDone.");

    return true;
}



/*-------------------------------------
    Read all face data (triangles)
-------------------------------------*/
char* SceneFileLoader::upload_mesh_indices(
    const aiMesh* const pMesh,
    char* pIbo,
    const unsigned baseIndex,
    const unsigned baseVertex,
    SceneMesh& outMesh
) noexcept {
    const SceneFileMetaData& sceneInfo = preloader.sceneInfo;
    MeshMetaData& metaData = outMesh.metaData;

    // iterate through all faces in the mesh
    for (unsigned faceIter = 0; faceIter < pMesh->mNumFaces; ++faceIter) {
        const aiFace& face = pMesh->mFaces[faceIter];

        for (unsigned i = 0; i < face.mNumIndices; ++i) {
            const unsigned idx = face.mIndices[i] + baseVertex;

            switch(sceneInfo.indexType) {
                case index_element_t::INDEX_TYPE_UBYTE:
                    *reinterpret_cast<unsigned char*>(pIbo) = (unsigned char)(idx);
                    break;

                case index_element_t::INDEX_TYPE_USHORT:
                    *reinterpret_cast<unsigned short*>(pIbo) = (unsigned short)(idx);
                    break;

                case index_element_t::INDEX_TYPE_UINT:
                    *reinterpret_cast<unsigned int*>(pIbo) = (unsigned int)(idx);
                    break;

                case index_element_t::INDEX_TYPE_NONE:
                default:
                    LS_ASSERT(false && "Unknown index type.");
                    break;
            }

            pIbo = (char*)((ptrdiff_t)pIbo + metaData.calc_index_stride());
        }

        metaData.totalIndices += face.mNumIndices;
    }

    // store the first/last vertex indices
    DrawCommandParams& drawParams = outMesh.drawParams;
    drawParams.drawFunc = draw_func_t::DRAW_ELEMENTS;
    drawParams.drawMode = convert_assimp_draw_mode(pMesh);
    drawParams.indexType = sceneInfo.indexType;
    drawParams.offset   = (void*)((ptrdiff_t)baseIndex);
    drawParams.count    = metaData.totalIndices;

    return pIbo;
}



/*-------------------------------------
 * Retrieve a single VBO Marker
-------------------------------------*/
unsigned SceneFileLoader::get_mesh_group_marker(
    const common_vertex_t vertType,
    const std::vector<VboGroupMarker>& markers
) const noexcept {
    for (unsigned i = 0; i < markers.size(); ++i) {
        if (vertType == markers[i].vertType) {
            return i;
        }
    }

    LS_DEBUG_ASSERT(false);

    return (unsigned)-1;
}

/*-------------------------------------
    Read and import all nodes in a scene
-------------------------------------*/
void SceneFileLoader::read_node_hierarchy(
    const aiScene* const pScene,
    const aiNode* const pInNode,
    const unsigned parentId
) noexcept {
    // use the size of the node list as an index which should be returned to
    // the parent node's child indices.
    SceneGraph& sceneData                   = preloader.sceneData;
    std::vector<SceneNode>& nodeList        = sceneData.nodes;
    std::vector<std::string>& nodeNames     = sceneData.nodeNames;
    std::vector<math::mat4>& baseTransforms = sceneData.baseTransforms;
    std::vector<Transform>& currTransforms  = sceneData.currentTransforms;
    std::vector<math::mat4>& modelMatrices  = sceneData.modelMatrices;

    //LS_LOG_MSG("\tImporting Scene Node ", nodeList.size(), ": ", pInNode->mName.C_Str());

    nodeList.emplace_back(SceneNode());
    SceneNode& currentNode = nodeList.back();

    // initialize
    currentNode.reset();
    currentNode.nodeId = nodeList.size() - 1;

    // import the node name
    nodeNames.emplace_back(std::string{pInNode->mName.C_Str()});

    // import the node transformation
    // This is also needed for camera nodes to be imported properly
    {
        Transform baseTrans;
        aiVector3D inPos, inScale;
        aiQuaternion inRotation;

        pInNode->mTransformation.Decompose(inScale, inRotation, inPos);

        baseTrans.set_position(convert_assimp_vector(inPos));
        baseTrans.set_scale(convert_assimp_vector(inScale));
        baseTrans.set_orientation(convert_assimp_quaternion(inRotation));

        // Store the transform as specified by the scene graph hierarchy.
        currTransforms.push_back(baseTrans);

        // Store the original, unmodified, unparented transform
        baseTrans.apply_transform();
        baseTransforms.push_back(baseTrans.get_transform());
    }

    const int camIndex = is_node_type<aiCamera>(pInNode, pScene->mCameras, pScene->mNumCameras);

    if (camIndex >= 0) {
        currentNode.type = scene_node_t::NODE_TYPE_CAMERA;
        import_camera_node(pScene, camIndex, currentNode);
    }
    else if (is_node_type<aiMesh>(pInNode, pScene->mMeshes, pScene->mNumMeshes)) {
        currentNode.type = scene_node_t::NODE_TYPE_MESH;
        import_mesh_node(pInNode, currentNode);
    }
    else {
        currentNode.type = scene_node_t::NODE_TYPE_EMPTY;
    }

    // Remaining transformatrion information
    {
        Transform& nodeTransform = currTransforms.back();
        nodeTransform.set_parent_index(parentId);

        // Only apply parented transforms here. Applying non-parented
        // transforms will cause root objects to swap X & Z axes.
        if (parentId != SCENE_GRAPH_ROOT_ID) {
            nodeTransform.apply_pre_transform(currTransforms[parentId].get_transform());
        }

        modelMatrices.push_back(currTransforms.back().get_transform());
    }

    // CYA
    LS_DEBUG_ASSERT(nodeList.size() == nodeNames.size());
    LS_DEBUG_ASSERT(nodeList.size() == baseTransforms.size());
    LS_DEBUG_ASSERT(nodeList.size() == currTransforms.size());
    LS_DEBUG_ASSERT(nodeList.size() == modelMatrices.size());

    // recursively load node children
    for (unsigned childId = 0; childId < pInNode->mNumChildren; ++childId) {
        const aiNode* const pChildNode = pInNode->mChildren[childId];

        read_node_hierarchy(pScene, pChildNode, currentNode.nodeId);
    }
}

/*-------------------------------------
    Import a mesh node
-------------------------------------*/
void SceneFileLoader::import_mesh_node(const aiNode* const pNode, SceneNode& outNode) noexcept {
    SceneGraph& sceneData = preloader.sceneData;
    std::vector<SceneMesh>& sceneMeshes = sceneData.meshes;
    std::vector<unsigned>& nodeMeshCounts = sceneData.nodeMeshCounts;
    std::vector<utils::Pointer<DrawCommandParams[]>>& meshList = sceneData.nodeMeshes;

    // The check for how many meshes a scene node has must have already been
    // performed.
    const unsigned numMeshes = pNode->mNumMeshes;
    LS_DEBUG_ASSERT(numMeshes > 0);

    utils::Pointer<DrawCommandParams[]> drawParams{new DrawCommandParams[numMeshes]};
    LS_ASSERT(!!drawParams);

    // map the internal indices to the assimp node's mesh list
    for (unsigned i = 0; i < pNode->mNumMeshes; ++i) {
        const SceneMesh& loadedMesh = sceneMeshes[pNode->mMeshes[i]];
        drawParams[i] = loadedMesh.drawParams;
    }

    // Very important
    outNode.dataId = meshList.size();

    nodeMeshCounts.push_back(numMeshes);
    meshList.emplace_back(std::move(drawParams));
}

/*-------------------------------------
    Import a camera node
-------------------------------------*/
void SceneFileLoader::import_camera_node(
    const aiScene* const pScene,
    const int camIndex,
    SceneNode& outNode
) noexcept {
    const aiCamera* const* const pCamList   = pScene->mCameras;
    SceneGraph& sceneData                   = preloader.sceneData;
    std::vector<Camera>& camList            = sceneData.cameras;

    camList.emplace_back(Camera{});

    // Very important
    outNode.dataId = camList.size();

    const aiCamera* const pInCam = pCamList[camIndex];
    Camera& camProj = camList.back();
    camProj.set_fov(pInCam->mHorizontalFOV);
    camProj.set_aspect_ratio(pInCam->mAspect, 1.f);
    camProj.set_near_plane(pInCam->mClipPlaneNear);
    camProj.set_far_plane(pInCam->mClipPlaneFar);
    camProj.set_projection_type(projection_type_t::PROJECTION_PERSPECTIVE);
    camProj.update();

    // A Transform object must have been added by the parent function
    Transform& camTrans = sceneData.currentTransforms.back();
    camTrans.set_type(transform_type_t::TRANSFORM_TYPE_VIEW_FPS); // ASSIMP never specified a default

    const aiNode* const pNode = pScene->mRootNode->FindNode(pInCam->mName);

    aiVector3D inPos, inDir, inUp;

    if (pNode) {
        const aiMatrix4x4& inMat = pNode->mTransformation;

        inPos = pInCam->mPosition;
        inPos *= inMat;

        inDir = pInCam->mLookAt;
        inDir *= inMat;

        inUp = pInCam->mUp;
        inUp *= inMat;
    }
    else {
        inPos = pInCam->mPosition;
        inDir = pInCam->mLookAt;
        inUp = pInCam->mUp;
    }

    const math::vec3&& finalPos = convert_assimp_vector(inPos);
    const math::vec3&& finalDir = convert_assimp_vector(inDir);
    const math::vec3&& finalUp = convert_assimp_vector(inUp);
    camTrans.look_at(finalPos, finalDir, finalUp);

    const math::vec3& camPos = camTrans.get_position();
    const math::vec3&& camUp = math::vec3{0.f, 1.f, 0.f};//nodeCam.get_up_direction();

    LS_LOG_MSG("\tLoaded the scene camera ", pInCam->mName.C_Str(), ':',
        "\n\t\tField of View: ", LS_RAD2DEG(camProj.get_fov()),
        "\n\t\tAspect Ratio:  ", camProj.get_aspect_ratio(),
        "\n\t\tNear Plane:    ", camProj.get_near_plane(),
        "\n\t\tFar Plane:     ", camProj.get_far_plane(),
        "\n\t\tPosition:      {", camPos[0], ", ", camPos[1], ", ", camPos[2], '}',
        "\n\t\tUp Direction:  {", camUp[0], ", ", camUp[1], ", ", camUp[2], '}'
    );
}

/*-------------------------------------
 * Import ASSIMP animations
-------------------------------------*/
bool SceneFileLoader::import_animations(const aiScene* const pScene) noexcept {
    bool ret = true;
    const aiAnimation* const* const pAnimations = pScene->mAnimations;
    SceneGraph& sceneData                       = preloader.sceneData;
    std::vector<Animation>& animations          = sceneData.animations;
    const unsigned totalAnimations              = pScene->mNumAnimations;
    std::vector<std::vector<AnimationChannel>>& allChannels = sceneData.nodeAnims;

    for (unsigned i = 0; i < totalAnimations; ++i) {
        const aiAnimation* const pInAnim = pAnimations[i];
        Animation& anim = animations[i];

        // The animation as a whole needs to have its properties imported from
        // ASSIMP.
        anim.set_duration(pInAnim->mDuration);
        anim.set_anim_name(std::string {pInAnim->mName.C_Str()});
        anim.set_ticks_per_sec(pInAnim->mTicksPerSecond > 0.0 ? pInAnim->mTicksPerSecond : 23.976);
        anim.reserve_anim_channels(pInAnim->mNumChannels);

        for (unsigned c = 0; c < pInAnim->mNumChannels; ++c) {
            AnimationChannel track;
            const aiNodeAnim* const pInTrack = pInAnim->mChannels[c];
            const unsigned nodeId = import_animation_track(pInTrack, track, anim.get_duration());

            if (nodeId == UINT_MAX) {
                // failing to load an Animation track is not an error.
                ret = false;
                continue;
            }

            // Grab the scene node which contains the data ID to map with a
            // transformation and animation channel
            SceneNode& node = sceneData.nodes[nodeId];
            
            // If a list of animation tracks doesn't exist for the current node
            // then be sure to add it.
            if (node.animListId == scene_property_t::SCENE_GRAPH_ROOT_ID) {
                node.animListId = allChannels.size();
                allChannels.emplace_back(std::vector<AnimationChannel>{});
            }
            
            std::vector<AnimationChannel>& nodeChannels = allChannels[node.animListId];
            nodeChannels.emplace_back(std::move(track));
            
            // Add the node's imported track to the current animation
            anim.add_anim_channel(node, nodeChannels.size()-1);
        }

        LS_LOG_MSG(
            "\tLoaded Animation ", i+1, '/', totalAnimations,
            "\n\t\tName:      ", anim.get_anim_name(),
            "\n\t\tDuration:  ", anim.get_duration(),
            "\n\t\tTicks/Sec: ", anim.get_ticks_per_sec(),
            "\n\t\tChannels:  ", anim.get_num_anim_channels()
        );
    }

    if (totalAnimations > 0) {
        Animation& initialState = animations[0];
        initialState.init(sceneData);
    }

    LS_LOG_MSG("\tSuccessfully loaded ", animations.size(), " animations.");

    return ret;
}

/*-------------------------------------
 * Import a single Animation track
-------------------------------------*/
unsigned SceneFileLoader::import_animation_track(
    const aiNodeAnim* const pInAnim,
    AnimationChannel& outAnim,
    const anim_prec_t animDuration
) noexcept {
    const unsigned posFrames                    = pInAnim->mNumPositionKeys;
    const unsigned sclFrames                    = pInAnim->mNumScalingKeys;
    const unsigned rotFrames                    = pInAnim->mNumRotationKeys;
    SceneGraph& sceneData                       = preloader.sceneData;
    const std::vector<std::string>& nodeNames   = sceneData.nodeNames;
    const char* const pInName                   = pInAnim->mNodeName.C_Str();
    unsigned nodeId                             = 0;

    // Locate the node associated with the current animation track.
    for (; nodeId < nodeNames.size(); ++nodeId) {
        if (nodeNames[nodeId] == pInName) {
            break;
        }
    }

    if (nodeId == nodeNames.size()) {
        LS_LOG_ERR("\tError: Unable to locate the animation track for a scene node: ", pInAnim);
        outAnim.clear();
        return UINT_MAX;
    }

    if (!outAnim.set_num_frames(posFrames, sclFrames, rotFrames)) {
        LS_LOG_MSG("Unable to import the Animation \"", pInAnim->mNodeName.C_Str(), "\".");
        return UINT_MAX;
    }

    AnimationKeyListVec3& outPosFrames = outAnim.positionFrames;
    AnimationKeyListVec3& outSclFrames = outAnim.scaleFrames;
    AnimationKeyListQuat& outRotFrames = outAnim.rotationFrames;

    // Convert all animation times into percentages for the internal animation
    // system.
    // Positions
    for (unsigned p=0; p < outPosFrames.size(); ++p) {
        const aiVectorKey& posKey = pInAnim->mPositionKeys[p];
        outPosFrames.set_frame(p, posKey.mTime/animDuration, convert_assimp_vector(posKey.mValue));
    }

    // Scalings
    for (unsigned s=0; s < outSclFrames.size(); ++s) {
        const aiVectorKey& sclKey = pInAnim->mScalingKeys[s];
        outSclFrames.set_frame(s, sclKey.mTime/animDuration, convert_assimp_vector(sclKey.mValue));
    }

    // Rotations
    for (unsigned r=0; r < outRotFrames.size(); ++r) {
        const aiQuatKey& rotKey = pInAnim->mRotationKeys[r];
        outRotFrames.set_frame(r, rotKey.mTime/animDuration, convert_assimp_quaternion(rotKey.mValue));
    }

    // Convert ASSIMP animation flags into internal ones.
    // ASSIMP allows for animation behavior to change between the starting and
    // ending animation frames. Those are currently unsupported, all imported
    // animations share the same flags from start to finish.
    unsigned animFlags = ANIM_FLAG_NONE;
    const auto flagFill = [&](const aiAnimBehaviour inFlag, const unsigned outFlag)->void {
        if (pInAnim->mPreState & inFlag || pInAnim->mPostState & inFlag) {
            animFlags |= outFlag;
        }
    };

    flagFill(aiAnimBehaviour_CONSTANT,  ANIM_FLAG_IMMEDIATE);
    flagFill(aiAnimBehaviour_DEFAULT,   ANIM_FLAG_INTERPOLATE);
    flagFill(aiAnimBehaviour_LINEAR,    ANIM_FLAG_INTERPOLATE);
    flagFill(aiAnimBehaviour_REPEAT,    ANIM_FLAG_REPEAT);
    
    outAnim.animationMode = (animation_flag_t)animFlags;

    LS_LOG_MSG(
        "\tSuccessfully imported the Animation \"", pInAnim->mNodeName.C_Str(), '\"',
        "\n\t\tPosition Keys: ", outPosFrames.size(), " @ ", outPosFrames.get_duration(),
        "\n\t\tScaling Keys:  ", outSclFrames.size(), " @ ", outSclFrames.get_duration(),
        "\n\t\tRotation Keys: ", outRotFrames.size(), " @ ", outRotFrames.get_duration()
    );

    return nodeId;
}


} // end draw namespace
} // end ls namespace
