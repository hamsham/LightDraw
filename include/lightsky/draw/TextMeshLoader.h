/*
 * File:   draw/GeometryLoader.h
 * Author: miles
 *
 * Created on August 3, 2015, 9:50 PM
 */

#ifndef __LS_DRAW_GEOMETRY_LOADER_H__
#define __LS_DRAW_GEOMETRY_LOADER_H__

#include <string>

#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/VertexUtils.h"
#include "lightsky/draw/BoundingBox.h"
#include "lightsky/draw/SceneGraph.h"
#include "lightsky/draw/SceneMesh.h"

/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
namespace ls
{

/*-----------------------------------------------------------------------------
 * Text Loader Functions
-----------------------------------------------------------------------------*/
namespace draw
{

enum common_vertex_t : unsigned;

class Atlas;

struct AtlasGlyph;

class IndexBuffer;

class VertexBuffer;

class VertexArray;



/**------------------------------------
 * @brief Mesh Properties
 *
 * These enumerations contains properties for different types of geometry
 * objects. These constants used within contain vital information that's used
 * within the geometry object's implementation.
-------------------------------------*/
enum text_property_t
{
    TEXT_VERTS_PER_GLYPH = 4,

    TEXT_INDICES_PER_GLYPH = 6,

    // TODO: Make this number editable as a member variable.
    DEFAULT_TEXT_SPACES_PER_TAB = 4,

    // TODO: Make this number editable as a member variable.
    DEFAULT_TEXT_LINE_SPACING = 1,

    SUPPORTED_TEXT_VERTEX_TYPES = (0
                                   | common_vertex_t::POSITION_VERTEX
                                   | common_vertex_t::TEXTURE_VERTEX
                                   | common_vertex_t::NORMAL_VERTEX
                                   | common_vertex_t::TANGENT_VERTEX
                                   | common_vertex_t::BITANGENT_VERTEX
                                   | common_vertex_t::INDEX_VERTEX
                                   | 0)
};



/*-----------------------------------------------------------------------------
 * Text Loader Class
-----------------------------------------------------------------------------*/
class TextMeshLoader
{
  public:
    static constexpr buffer_map_t DEFAULT_VBO_MAP_FLAGS = (buffer_map_t)(0
                                                                         | VBO_MAP_BIT_INVALIDATE_RANGE
                                                                         | VBO_MAP_BIT_UNSYNCHRONIZED
                                                                         | VBO_MAP_BIT_INVALIDATE_BUFFER
                                                                         | VBO_MAP_BIT_WRITE
                                                                         | 0);

    /**
     * @brief Retrieve a count of the number of characters which can be rendered
     * from OpenGL.
     *
     * This function attempts to ignore all whitespace characters that cannot be
     * rendered through OpenGL.
     *
     * @param str
     * A constant reference to a string of text whose characters will be analyzed
     * for renderability.
     *
     * @return An unsigned integral type, containing the number of renderable
     * characters in the input string.
     */
    static unsigned get_num_drawable_chars(const std::string& str) noexcept;

    /**
     * @brief Generate information about what it takes to store text geometry on
     * the GPU with certain vertex data.
     *
     * @param str
     * A constant reference to a string of text. The number of vertices, indices,
     * and bytes required for both will be derived from this string.
     *
     * @param vertexTypes
     * A bitmask, containing a set of vertex types which will be taken into account
     * when determining the number of vertices and indices required to render a
     * string.
     */
    void generate_meta_data(const std::string& str, const common_vertex_t vertexTypes) noexcept;

  private:
    MeshMetaData totalMetaData;

    SceneGraph sceneData;

    float lineSpacing;

    float horizTabSpacing;

    float vertTabSpacing;

    template <typename data_t>
    char* set_text_vertex_data(char* const pVert, const data_t& data) noexcept;

    char* set_text_index_data(char* pIndices, const unsigned indexOffset) noexcept;

    unsigned calc_text_geometry_pos(const AtlasGlyph& rGlyph, char* pVert, const math::vec2& posOffset, const unsigned charIndex) noexcept;

    unsigned calc_text_geometry_uvs(const AtlasGlyph& rGlyph, char* pVert) noexcept;

    unsigned calc_text_geometry_norms(char* pVert, const math::vec3& normDir) noexcept;

    unsigned calc_text_geometry_indices(char* pVert, const unsigned indexId) noexcept;

    char* gen_text_geometry_vert(const AtlasGlyph& rGlyph, char* const pData, const math::vec2& posOffset, const unsigned currChar) noexcept;

    template <typename data_t>
    char* fill_geometry_indices(void* const pIndices, const unsigned indexOffset) noexcept;

    bool gen_text_geometry(const std::string& str, const Atlas& atlas) noexcept;

    void update_buffer_attribs(VertexBuffer& vbo, IndexBuffer& ibo) noexcept;

    size_t allocate_cpu_data(const std::string& str, const common_vertex_t vertexTypes, const bool loadBounds) noexcept;

    size_t allocate_gpu_data(const Atlas& atlas) noexcept;

    bool assemble_vao() noexcept;

  public:
    /**
     * Destructor
     *
     * Clears all CPU-side data from *this. A manual call to "unload()" is
     * required to free GPU-side data.
     */
    ~TextMeshLoader() noexcept;

    /**
     * Constructor
     *
     * Initializes all internal members to their default states.
     */
    TextMeshLoader() noexcept;

    /**
     * Copy Constructor
     *
     * Copies data from the input parameter into *this.
     *
     * @param t
     * An l-value reference to another TextMeshLoader object.
     */
    TextMeshLoader(const TextMeshLoader& t) noexcept;

    /**
     * Move Constructor
     *
     * Moves data from the input parameter into *this.
     *
     * @param t
     * An r-value reference to another TextMeshLoader object.
     */
    TextMeshLoader(TextMeshLoader&& t) noexcept;

    /**
     * Copy Operator
     *
     * Copies data from the input parameter into *this.
     *
     * @param t
     * An l-value reference to another TextMeshLoader object.
     *
     * @return A reference to *this.
     */
    TextMeshLoader& operator=(const TextMeshLoader& t) noexcept;

    /**
     * Move Operator
     *
     * Moves data from the input parameter into *this.
     *
     * @param t
     * An r-value reference to another TextMeshLoader object.
     *
     * @return A reference to *this.
     */
    TextMeshLoader& operator=(TextMeshLoader&& t) noexcept;

    /**
     * @brief Initialize, generate, and emplace a set of textual geometry into an
     * OpenGL VBO and IBO.
     *
     * The winding/index order for all text rendering follows this basic format:
     *
     * 0--------2,3
     * |     /  |
     * |   /    |
     * | /      |
     * 1,4------5
     *
     * @param str
     * A constant reference to a string of text, containing the characters which
     * will be represented by the geometry contained within the vbo and ibo
     * parameters.
     *
     * @param vertexTypes
     * A bitmask, containing a set of vertex types which should be generated by
     * this function.
     *
     * @param atlas
     * A constant reference to an atlas object which contains glyph size and text
     * bitmaps which will be represented by the resulting VBO+IBO objects.
     *
     * @param loadBounds
     * Load the bounding boxes of all glyphs into memory. This only loads
     * bounding boxes on the CPU, not as vertex data on the GPU.
     *
     * @return An unsigned integral type, containing the number of indices which
     * were used to generate the vertex data in the "vbo" parameter.
     */
    unsigned load(const std::string& str, const common_vertex_t vertexTypes, const Atlas& atlas, const bool loadBounds = false) noexcept;

    /**
     * Clear all CPU and GPU data from *this. Reset all internal members to
     * their defaults.
     */
    void unload();

    const SceneGraph& get_mesh() const noexcept;

    SceneGraph& get_mesh() noexcept;

    void set_spaces_per_horiz_tab(const unsigned numSpaces = text_property_t::DEFAULT_TEXT_SPACES_PER_TAB) noexcept;

    unsigned get_spaces_per_horiz_tab() const noexcept;

    void set_spaces_per_vert_tab(const unsigned numSpaces = text_property_t::DEFAULT_TEXT_SPACES_PER_TAB) noexcept;

    unsigned get_spaces_per_vert_tab() const noexcept;

    void set_line_spacing(const float numSpaces = (float)text_property_t::DEFAULT_TEXT_LINE_SPACING) noexcept;

    float get_line_spacing() const noexcept;
};

/*-------------------------------------
 * Calculate a portion of vertex data that a glyph should contain.
-------------------------------------*/
template <typename data_t>
inline char* TextMeshLoader::set_text_vertex_data(char* const pVert, const data_t& data) noexcept
{
    *reinterpret_cast<data_t* const> (pVert) = data;
    return pVert + totalMetaData.calc_vertex_stride();
}

/*-------------------------------------
 * Set the index data required by geometry text (helper function).
-------------------------------------*/
template <typename data_t>
char* TextMeshLoader::fill_geometry_indices(void* const pIndices, const unsigned indexOffset) noexcept
{
    data_t* pData = reinterpret_cast<data_t*> (pIndices);

    *(pData++) = indexOffset + 0;
    *(pData++) = indexOffset + 1;
    *(pData++) = indexOffset + 2;
    *(pData++) = indexOffset + 2;
    *(pData++) = indexOffset + 1;
    *(pData++) = indexOffset + 3;

    return reinterpret_cast<char*> (pData);
}

/*-------------------------------------
 * Retrieve the currently loaded mesh (const)
-------------------------------------*/
inline const SceneGraph& TextMeshLoader::get_mesh() const noexcept
{
    return sceneData;
}

/*-------------------------------------
 * Retrieve the currently loaded mesh
-------------------------------------*/
inline SceneGraph& TextMeshLoader::get_mesh() noexcept
{
    return sceneData;
}

/*-------------------------------------
-------------------------------------*/
inline void TextMeshLoader::set_spaces_per_horiz_tab(const unsigned numSpaces) noexcept
{
    horizTabSpacing = (float)numSpaces;
}

/*-------------------------------------
-------------------------------------*/
inline unsigned TextMeshLoader::get_spaces_per_horiz_tab() const noexcept
{
    return (unsigned)math::floor(horizTabSpacing + 0.5f);
}

/*-------------------------------------
-------------------------------------*/
inline void TextMeshLoader::set_spaces_per_vert_tab(const unsigned numSpaces) noexcept
{
    vertTabSpacing = (float)numSpaces;
}

/*-------------------------------------
-------------------------------------*/
inline unsigned TextMeshLoader::get_spaces_per_vert_tab() const noexcept
{
    return (unsigned)math::floor(vertTabSpacing + 0.5f);
}

/*-------------------------------------
-------------------------------------*/
inline void TextMeshLoader::set_line_spacing(const float numSpaces) noexcept
{
    lineSpacing = numSpaces;
}

/*-------------------------------------
-------------------------------------*/
inline float TextMeshLoader::get_line_spacing() const noexcept
{
    return lineSpacing;
}
} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_GEOMETRY_LOADER_H__ */
