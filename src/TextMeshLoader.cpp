/*
 * File:   draw/GeometryLoader.cpp
 * Author: miles
 *
 * Created on August 3, 2015, 9:50 PM
 */

#include <algorithm> // std::copy, std::fill
#include <cctype> // isgraph(...)
#include <new> // std::nothrow

#include "lightsky/setup/Macros.h"

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Log.h"

#include "lightsky/math/vec2.h"
#include "lightsky/math/vec3.h"

#include "lightsky/draw/Atlas.h"
#include "lightsky/draw/VertexBuffer.h"
#include "lightsky/draw/IndexBuffer.h"
#include "lightsky/draw/PackedVertex.h"
#include "lightsky/draw/VAOAttrib.h"
#include "lightsky/draw/VAOAssembly.h"
#include "lightsky/draw/GLContext.h"
#include "lightsky/draw/TextMeshLoader.h"
#include "lightsky/draw/VertexArray.h"


namespace ls {
namespace draw {

/*-------------------------------------
-------------------------------------*/
TextMeshLoader::~TextMeshLoader() noexcept {

}

/*-------------------------------------
-------------------------------------*/
TextMeshLoader::TextMeshLoader() noexcept :
    totalMetaData(),
    sceneData(),
    lineSpacing {(float)DEFAULT_TEXT_LINE_SPACING},
    horizTabSpacing {(float)DEFAULT_TEXT_SPACES_PER_TAB},
    vertTabSpacing {(float)DEFAULT_TEXT_SPACES_PER_TAB}
{}

/*-------------------------------------
-------------------------------------*/
TextMeshLoader::TextMeshLoader(const TextMeshLoader& t) noexcept {
    *this = t;
}

/*-------------------------------------
-------------------------------------*/
TextMeshLoader::TextMeshLoader(TextMeshLoader&& t) noexcept {
    *this = std::move(t);
}

/*-------------------------------------
-------------------------------------*/
TextMeshLoader& TextMeshLoader::operator =(const TextMeshLoader& t) noexcept {
    totalMetaData = t.totalMetaData;
    sceneData = t.sceneData;
    lineSpacing = t.lineSpacing;
    horizTabSpacing = t.horizTabSpacing;
    vertTabSpacing = t.vertTabSpacing;

    return *this;
}

/*-------------------------------------
-------------------------------------*/
TextMeshLoader& TextMeshLoader::operator =(TextMeshLoader&& t) noexcept {
    totalMetaData = t.totalMetaData;
    t.totalMetaData.reset();
    
    sceneData = std::move(t.sceneData);

    lineSpacing = t.lineSpacing;
    t.lineSpacing = text_property_t::DEFAULT_TEXT_LINE_SPACING;

    horizTabSpacing = t.horizTabSpacing;
    t.horizTabSpacing = text_property_t::DEFAULT_TEXT_SPACES_PER_TAB;

    vertTabSpacing = t.vertTabSpacing;
    t.vertTabSpacing = text_property_t::DEFAULT_TEXT_SPACES_PER_TAB;

    return *this;
}

/*-------------------------------------
 * Utility function to get all of the non-whitespace characters in a string
-------------------------------------*/
unsigned TextMeshLoader::get_num_drawable_chars(const std::string& str) noexcept {
    unsigned charCount = 0;

    for (unsigned i = 0; i < str.size(); ++i) {
        if (isgraph(str[i])) {
            ++charCount;
        }
    }

    return charCount;
}

/*-------------------------------------
 * Meta information about the text being generated.
-------------------------------------*/
void TextMeshLoader::generate_meta_data(const std::string& str, const common_vertex_t vertexTypes) noexcept {
    totalMetaData.numSubmeshes = get_num_drawable_chars(str);

    // ignore vertex types which are have no support within the text loader
    totalMetaData.vertTypes = (common_vertex_t)(vertexTypes & SUPPORTED_TEXT_VERTEX_TYPES);
    totalMetaData.totalVerts = totalMetaData.numSubmeshes * text_property_t::TEXT_VERTS_PER_GLYPH;

    totalMetaData.indexType = get_required_index_type(totalMetaData.totalVerts);
    totalMetaData.totalIndices = totalMetaData.numSubmeshes * text_property_t::TEXT_INDICES_PER_GLYPH;

    LS_LOG_MSG(
        "Text Geometry Meta Data:",
        "\n\tBytes Per Vertex:  ", totalMetaData.calc_vertex_stride(),
        "\n\tVertex Count:      ", totalMetaData.totalVerts,
        "\n\tVertex Bytes:      ", totalMetaData.calc_total_vertex_bytes(),
        "\n\tBytes Per Index:   ", totalMetaData.calc_index_stride(),
        "\n\tIndex Count:       ", totalMetaData.totalIndices,
        "\n\tIndex Bytes:       ", totalMetaData.calc_total_index_bytes(),
        "\n\tTotal Bytes:       ", totalMetaData.calc_total_bytes()
    );
}

/*-------------------------------------
 * Calculate the vertex positions that a glyph should represent.
-------------------------------------*/
unsigned TextMeshLoader::calc_text_geometry_pos(
    const AtlasGlyph& rGlyph,
    char* pVert,
    const math::vec2& posOffset,
    const unsigned charIndex
) noexcept {
    const float xOffset = posOffset[0];
    const float yOffset = posOffset[1];
    const math::vec2& glyphSize = rGlyph.size;

    const math::vec3 posData[4] = {
        math::vec3{xOffset,                 yOffset - glyphSize[1], 0.f},
        math::vec3{xOffset,                 yOffset,                0.f},
        math::vec3{xOffset + glyphSize[0],  yOffset - glyphSize[1], 0.f},
        math::vec3{xOffset + glyphSize[0],  yOffset,                0.f}
    };

    pVert = set_text_vertex_data(pVert, posData[0]);
    pVert = set_text_vertex_data(pVert, posData[1]);
    pVert = set_text_vertex_data(pVert, posData[2]);
    set_text_vertex_data(pVert, posData[3]);

    std::vector<BoundingBox>& boundsBuffer = sceneData.bounds;
    if (!boundsBuffer.empty()) {
        BoundingBox& bb = boundsBuffer[charIndex];
        bb.set_top_rear_right(posData[0]);
        bb.set_bot_front_left(posData[3]);
    }

    //return byte-stride to the next vertex attrib
    static const unsigned vertOffset = get_vertex_byte_size(common_vertex_t::POSITION_VERTEX);
    return vertOffset;
}

/*-------------------------------------
 * Function to calculate the UVs which should represent a glyph.
-------------------------------------*/
unsigned TextMeshLoader::calc_text_geometry_uvs(
    const AtlasGlyph& rGlyph,
    char* pVert
) noexcept {
    pVert = set_text_vertex_data(pVert, math::vec2{rGlyph.uv[0][0], rGlyph.uv[0][1]});
    pVert = set_text_vertex_data(pVert, math::vec2{rGlyph.uv[0][0], rGlyph.uv[1][1]});
    pVert = set_text_vertex_data(pVert, math::vec2{rGlyph.uv[1][0], rGlyph.uv[0][1]});
    set_text_vertex_data(pVert, math::vec2{rGlyph.uv[1][0], rGlyph.uv[1][1]});

    //return byte-stride to the next vertex attrib
    static const unsigned vertOffset = get_vertex_byte_size(common_vertex_t::TEXTURE_VERTEX);
    return vertOffset;
}

/*-------------------------------------
 * Function to calculate the UVs which should represent a glyph.
-------------------------------------*/
unsigned TextMeshLoader::calc_text_geometry_norms(
    char* pVert,
    const math::vec3& normDir
) noexcept {
    const int32_t norm = pack_vertex_normal(normDir);
    pVert = set_text_vertex_data(pVert, norm);
    pVert = set_text_vertex_data(pVert, norm);
    pVert = set_text_vertex_data(pVert, norm);
    set_text_vertex_data(pVert, norm);

    //return byte-stride to the next vertex attrib
    static const unsigned vertOffset = get_vertex_byte_size(common_vertex_t::NORMAL_VERTEX);
    return vertOffset;
}

/*-------------------------------------
 * Function to calculate the ID of a single character
-------------------------------------*/
unsigned TextMeshLoader::calc_text_geometry_indices(char* pVert, const unsigned indexId) noexcept {
    pVert = set_text_vertex_data(pVert, indexId);
    pVert = set_text_vertex_data(pVert, indexId);
    pVert = set_text_vertex_data(pVert, indexId);
    set_text_vertex_data(pVert, indexId);

    //return byte-stride to the next vertex attrib
    static const unsigned vertOffset = get_vertex_byte_size(common_vertex_t::INDEX_VERTEX);
    return vertOffset;
}

/*-------------------------------------
 * Function to dispatch all text-loading responsibilities to their respective loaders.
-------------------------------------*/
char* TextMeshLoader::gen_text_geometry_vert(
    const AtlasGlyph& rGlyph,
    char* const pData,
    const math::vec2& posOffset,
    const unsigned currChar
) noexcept {
    const common_vertex_t vertTypes = totalMetaData.vertTypes;
    char* pVert = pData;

    if (vertTypes & common_vertex_t::POSITION_VERTEX) {
        pVert += calc_text_geometry_pos(rGlyph, pVert, posOffset, currChar);
    }

    if (vertTypes & common_vertex_t::TEXTURE_VERTEX) {
        pVert += calc_text_geometry_uvs(rGlyph, pVert);
    }

    if (vertTypes & common_vertex_t::NORMAL_VERTEX) {
        pVert += calc_text_geometry_norms(pVert, math::vec3 {0.f, 0.f, 1.f});
    }

    if (vertTypes & common_vertex_t::TANGENT_VERTEX) {
        pVert += calc_text_geometry_norms(pVert, math::vec3 {1.f, 0.f, 0.f});
    }

    if (vertTypes & common_vertex_t::BITANGENT_VERTEX) {
        pVert += calc_text_geometry_norms(pVert, math::vec3 {0.f, 1.f, 0.f});
    }

    if (vertTypes & common_vertex_t::INDEX_VERTEX) {
        pVert += calc_text_geometry_indices(pVert, currChar);
    }

    return pData + (totalMetaData.calc_vertex_stride() * TEXT_VERTS_PER_GLYPH);
}

/*-------------------------------------
 * Set the index data required by geometry text.
-------------------------------------*/
char* TextMeshLoader::set_text_index_data(
    char* pIndices,
    const unsigned indexOffset
) noexcept {
    if (totalMetaData.indexType == index_element_t::INDEX_TYPE_USHORT) {
        return fill_geometry_indices<unsigned short>(pIndices, indexOffset);
    }

    return fill_geometry_indices<unsigned int>(pIndices, indexOffset);
}

/*-------------------------------------
 * Text/String Generation
-------------------------------------*/
bool TextMeshLoader::gen_text_geometry(
    const std::string& str,
    const Atlas& atlas
) noexcept {
    VertexBuffer& vbo = sceneData.renderData.vbos.back();
    IndexBuffer& ibo = sceneData.renderData.ibos.back();

    // VBO Mapping
    char* pVerts = (char*)vbo.map_data(0, totalMetaData.calc_total_vertex_bytes(), DEFAULT_VBO_MAP_FLAGS);
    if (!pVerts) {
        LS_LOG_ERR("\t\tAn error occurred while attempting to map a VBO for text geometry.");
        return false;
    }

    // IBO Mapping
    char* pIndices = (char*)ibo.map_data(0, totalMetaData.calc_total_index_bytes(), DEFAULT_VBO_MAP_FLAGS);
    if (!pIndices) {
        vbo.unmap_data();
        LS_LOG_ERR("\t\tAn error occurred while attempting to map an IBO for text geometry.");
        return false;
    }

    // Get pointers to the buffer data that will be filled with quads
    const ls::utils::Pointer < AtlasGlyph[]>& pGlyphs = atlas.get_glyphs();

    // The y-origin (starting 'yPos') was found using a lot of testing. This
    // was for resolution independence
    const AtlasGlyph& newline = pGlyphs[u'\n'];
    float yPos = newline.bearing[1] - (newline.bearing[1] - newline.size[1]);
    float xPos = 0.f;
    unsigned charId = 0;
    unsigned indexOffset = 0;

    for (unsigned i = 0; i < str.size(); ++i) {
        const unsigned currChar = (unsigned)str[i];
        const AtlasGlyph& rGlyph = pGlyphs[currChar];

        // Amount the each glyph "hangs" below its Y-origin
        const float vertHang = rGlyph.bearing[1] - rGlyph.size[1];

        if (currChar == u' ') {
            xPos += rGlyph.advance[0];
        }
        else if (currChar == u'\t') {
            xPos += rGlyph.advance[0] * horizTabSpacing;
        }
        else if (currChar == u'\n') {
            // formula found through trial and error.
            yPos += (rGlyph.bearing[1] + lineSpacing) + vertHang;
            xPos = 0.f;
        }
        else if (currChar == u'\r') {
            xPos = 0.f;
        }
        else if (currChar == u'\v') {
            yPos += ((rGlyph.bearing[1] + lineSpacing) + vertHang) * vertTabSpacing;
        }
        else {
            const math::vec2 posOffset {
                xPos + rGlyph.bearing[0], yPos - vertHang
            };
            xPos = xPos + rGlyph.advance[0];
            pVerts = gen_text_geometry_vert(rGlyph, pVerts, posOffset, charId++);
            pIndices = set_text_index_data(pIndices, indexOffset);
            indexOffset = indexOffset + TEXT_VERTS_PER_GLYPH;
        }
    }

    vbo.unmap_data();
    ibo.unmap_data();

    return true;
}

/*-------------------------------------
 * Update some remaining attributes not set by "vbo/ibo.setup_attribs()"
-------------------------------------*/
void TextMeshLoader::update_buffer_attribs(VertexBuffer& vbo, IndexBuffer& ibo) noexcept {
    for (unsigned i = 0; i < vbo.get_num_attribs(); ++i) {
        VBOAttrib& vboAttrib = vbo.get_attrib(i);

        vboAttrib.set_num_elements(1);
        vboAttrib.set_byte_stride(totalMetaData.calc_vertex_stride());
    }

    for (unsigned i = 0; i < ibo.get_num_attribs(); ++i) {
        IBOAttrib& attrib = ibo.get_attrib(i);
        attrib.set_attrib_count(totalMetaData.totalIndices);
        attrib.set_attrib_type(totalMetaData.indexType);
        attrib.set_attrib_offset(0);
    }
}

/*-------------------------------------
 * CPU Memory Initialization
-------------------------------------*/
unsigned TextMeshLoader::allocate_cpu_data(
    const std::string& str,
    const common_vertex_t vertexTypes,
    const bool loadBounds
) noexcept {
    generate_meta_data(str, vertexTypes);
    
    unsigned numBytes = 0;
    char* offset = nullptr;
    const unsigned numSubmeshes = totalMetaData.numSubmeshes;
    const unsigned indexStride = totalMetaData.calc_index_stride();
    
    // Initial setup for atlas texture data
    std::vector<SceneMaterial>& materials = sceneData.materials;
    materials.resize(1);
    materials.front().reset();
    
    // Setup the initial text scene graph with some default draw params
    std::vector<SceneMesh>& meshes = sceneData.meshes;
    meshes.resize(totalMetaData.numSubmeshes);

    for (SceneMesh& subMesh : sceneData.meshes) {
        DrawCommandParams& drawParams = subMesh.drawParams;
        MeshMetaData& meshMetaData  = subMesh.metaData;
        
        drawParams.materialId       = 0;
        drawParams.vaoId            = 0; // TBDE once the VAO has been loaded.
        drawParams.drawFunc         = draw_func_t::DRAW_ELEMENTS;
        drawParams.drawMode         = draw_mode_t::DRAW_MODE_TRIS;
        drawParams.indexType        = totalMetaData.indexType;
        drawParams.offset           = offset;
        drawParams.count            = text_property_t::TEXT_INDICES_PER_GLYPH;
        
        meshMetaData.numSubmeshes   = 1;
        meshMetaData.vertTypes      = (common_vertex_t)(vertexTypes & SUPPORTED_TEXT_VERTEX_TYPES);
        meshMetaData.totalVerts     = text_property_t::TEXT_VERTS_PER_GLYPH;
        meshMetaData.indexType      = totalMetaData.indexType;
        meshMetaData.totalIndices   = text_property_t::TEXT_INDICES_PER_GLYPH;
        
        offset += text_property_t::TEXT_INDICES_PER_GLYPH * indexStride;
    }
    
    numBytes += sizeof(SceneMesh) * meshes.size();

    if (loadBounds) {
        std::vector<BoundingBox>& boundsBuffer = sceneData.bounds;
        boundsBuffer.resize(numSubmeshes);
        numBytes += sizeof(BoundingBox) * numSubmeshes;
    }

    return numBytes;
}

/*-------------------------------------
 * GPU Memory Initialization
-------------------------------------*/
unsigned TextMeshLoader::allocate_gpu_data(const Atlas& atlas) noexcept {
    std::vector<SceneMaterial>& materials = sceneData.materials;
    SceneMaterial& material = materials.front();
    material.bindSlots[0] = tex_slot_t::TEXTURE_SLOT_GPU_OFFSET + tex_slot_t::TEXTURE_SLOT_DIFFUSE;
    material.textures[0] = atlas.get_texture().gpu_id();
    
    unsigned numBytes = 0;
    
    GLContextData& renderData = sceneData.renderData;
    renderData.vbos.add(VertexBuffer{});
    renderData.ibos.add(IndexBuffer{});
    
    VertexBuffer& vbo = renderData.vbos.front();
    IndexBuffer& ibo = renderData.ibos.front();

    // Initialize VBO
    if (!vbo.init() || !vbo.setup_attribs(totalMetaData.vertTypes)) {
        return 0;
    }

    // Initialize IBO
    if (!ibo.init() || !ibo.setup_attribs(1)) {
        return 0;
    }

    // Generate the text geometry
    LS_LOG_MSG("\tGenerating a VAO to store text mesh data.");
    if (!assemble_vao()) {
        LS_LOG_ERR("\t\tUnable create a mesh VAO from text data.\n");
        sceneData.terminate();
        return 0;
    }
    LS_LOG_MSG("\t\tDone. A VAO has been generated to store text mesh data.");

    vbo.bind();
    ibo.bind();

    // VBO Allocation
    vbo.set_data(totalMetaData.calc_total_vertex_bytes(), nullptr, buffer_access_t::VBO_STATIC_DRAW);
    LS_LOG_GL_ERR();
    numBytes += totalMetaData.calc_total_vertex_bytes();

    ibo.set_data(totalMetaData.calc_total_index_bytes(), nullptr, buffer_access_t::VBO_STATIC_DRAW);
    LS_LOG_GL_ERR();
    numBytes += totalMetaData.calc_total_index_bytes();
    
    // last bit of data linkage from GPU -> CPU.
    for (SceneMesh& subMesh : sceneData.meshes) {
        subMesh.drawParams.vaoId = renderData.vaos.front().gpu_id();
        subMesh.vboId = vbo.gpu_id();
        subMesh.iboId = ibo.gpu_id();
    }
    
    return numBytes;
}

/*-------------------------------------
 * Text/String loading
-------------------------------------*/
bool TextMeshLoader::assemble_vao() noexcept {
    GLContextData& renderData = sceneData.renderData;
    VertexBuffer& vbo = renderData.vbos.back();
    IndexBuffer& ibo = renderData.ibos.back();

    ls::utils::Pointer<draw::VAOAssembly> pAssembly {new(std::nothrow) draw::VAOAssembly {}};

    // using totalMetaData.vertTypes vs passing in the user-requested vertex
    // flags. some of the preprocessing functions may have added/removed flags.
    const unsigned setBitCount = math::count_set_bits(totalMetaData.vertTypes);
    unsigned vaoIndex = 0;

    pAssembly->set_vbo_attribs(vbo);
    pAssembly->set_ibo_attrib(ibo);

    LS_LOG_MSG("\t\tAssigning ", setBitCount, " attributes to a text mesh VAO.");

    for (unsigned i = 0; i < COMMON_VERTEX_NAMES_COUNT; ++i) {
        const common_vertex_t vertFlag = COMMON_VERTEX_FLAGS_LIST[i];

        if (0 == (totalMetaData.vertTypes & vertFlag)) {
            continue;
        }

        LS_ASSERT(pAssembly->set_attrib_name(vaoIndex++, draw::get_common_vertex_names()[i]));
    }

    renderData.vaos.add(VertexArray{});
    VertexArray& vao = renderData.vaos.back();

    LS_ASSERT(pAssembly->assemble(vao));

    LS_LOG_MSG("\t\tValidating there are ", setBitCount, " attributes within a text mesh VAO.");

    LS_LOG_MSG("\t\tValidating a text mesh VAO was successfully created.");
    return vao.is_valid();
}

/*-------------------------------------
 * Text/String loading
-------------------------------------*/
unsigned TextMeshLoader::load(
    const std::string& str,
    const common_vertex_t vertexTypes,
    const Atlas& atlas,
    const bool loadBounds
) noexcept {
    LS_LOG_MSG("Attempting to load text geometry.");
    unload(); // clear any prior data.

    LS_LOG_MSG("\tAllocating RAM for text mesh data.");
    const unsigned numBytes = allocate_cpu_data(str, vertexTypes, loadBounds);

    if (!numBytes) {
        LS_LOG_ERR("\t\tFailed to allocate ", numBytes, " bytes of memory for text mesh data.\n");
        sceneData.terminate();
        return 0;
    }
    LS_LOG_MSG("\t\tDone. Successfully allocated ", numBytes, " of memory in RAM.");

    LS_LOG_MSG("\tAllocating GPU memory for text mesh data.");
    if (!allocate_gpu_data(atlas)) {
        sceneData.terminate();
        LS_LOG_ERR("\t\tUnable to initialize text mesh data on the GPU.\n");
        return 0;
    }
    LS_LOG_MSG("\t\tDone.");

    GLContextData& renderData = sceneData.renderData;
    VertexBuffer& vbo = renderData.vbos.front();
    IndexBuffer& ibo = renderData.ibos.front();

    update_buffer_attribs(vbo, ibo);

    // Generate the text geometry
    LS_LOG_MSG("\tGenerating a text mesh on the GPU.");
    if (!gen_text_geometry(str, atlas)) {
        LS_LOG_ERR("\t\tUnable to send text geometry data to the GPU.\n");
        sceneData.terminate();
        return 0;
    }
    LS_LOG_MSG("\t\tDone.");
    
    SceneMesh& meshData = sceneData.meshes.front();
    meshData.drawParams.vaoId = renderData.vaos.front().gpu_id();
    meshData.vboId = vbo.gpu_id();
    meshData.iboId = ibo.gpu_id();

    vbo.unbind();
    ibo.unbind();
    
    LS_LOG_MSG(
        "\tSuccessfully sent a string to the GPU.",
        "\n\t\tCharacters:  ", totalMetaData.numSubmeshes,
        "\n\t\tVertices:    ", totalMetaData.totalVerts,
        "\n\t\tVert Size:   ", totalMetaData.calc_total_vertex_bytes(), " bytes"
        "\n\t\tIndices:     ", totalMetaData.totalIndices,
        "\n\t\tIndex Size:  ", totalMetaData.calc_total_index_bytes(), " bytes"
        "\n\t\tTotal Size:  ", totalMetaData.calc_total_bytes(), " bytes",
        '\n'
    );

    return totalMetaData.totalIndices;
}

/*-------------------------------------
 * Text/String loading
-------------------------------------*/
void TextMeshLoader::unload() {
    sceneData.terminate();

    lineSpacing = (float)DEFAULT_TEXT_LINE_SPACING;

    horizTabSpacing = (float)DEFAULT_TEXT_SPACES_PER_TAB;

    vertTabSpacing = (float)DEFAULT_TEXT_SPACES_PER_TAB;
}

} // end draw namespace
} // end ls namespace
