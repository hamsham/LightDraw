
# -------------------------------------
# Project Setup
# -------------------------------------
project(LightDraw CXX)
set(OUTPUT_NAME lsdraw)



# -------------------------------------
# Finders
# -------------------------------------
# Only using OpenGL for its libraries, not headers
include(FindOpenGL)

# FreeType requires special care
include(FindFreetype)



# -------------------------------------
# Include Paths
# -------------------------------------
include_directories("include")

include_directories(${FREETYPE_INCLUDE_DIRS})



# -------------------------------------
# Source Paths
# -------------------------------------
set(LS_DRAW_SOURCES
    "src/draw/Atlas.cpp"
    "src/draw/BlendObject.cpp"
    "src/draw/BoundingBox.cpp"
    "src/draw/BufferObject.cpp"
    "src/draw/Camera.cpp"
    "src/draw/Color.cpp"
    "src/draw/DepthObject.cpp"
    "src/draw/FontResource.cpp"
    "src/draw/FrameBuffer.cpp"
    "src/draw/GeometryUtils.cpp"
    "src/draw/ImageResource.cpp"
    "src/draw/IndexBuffer.cpp"
    "src/draw/MatrixStack.cpp"
    "src/draw/RenderBuffer.cpp"
    "src/draw/Setup.cpp"
    "src/draw/ShaderObject.cpp"
    "src/draw/ShaderProgram.cpp"
    "src/draw/Texture.cpp"
    "src/draw/Transform.cpp"
    "src/draw/VAOAssembly.cpp"
    "src/draw/VertexArray.cpp"
    "src/draw/VertexBuffer.cpp"
    "src/draw/VertexAttrib.cpp"
    "src/draw/VertexUtils.cpp"
)



# -------------------------------------
# Dependency Setup
# -------------------------------------
set(
    LS_DRAW_INTERNAL_DEPS
    lssetup
    lsutils
    lsmath
)

set(
    LS_DRAW_EXTERNAL_DEPS
    ${OPENGL_gl_LIBRARY}
    ${FREETYPE_LIBRARIES}
)



# -------------------------------------
# Library Setup
# -------------------------------------
add_library("${OUTPUT_NAME}" STATIC "${LS_DRAW_SOURCES}")

# Internal dependencies
add_dependencies(${LS_DRAW_INTERNAL_DEPS})

# Linking Setup
target_link_libraries("${OUTPUT_NAME}" ${LS_DRAW_INTERNAL_DEPS} ${LS_DRAW_EXTERNAL_DEPS})



# -------------------------------------
# Library Installation Setup
# -------------------------------------
install(TARGETS "${OUTPUT_NAME}" ARCHIVE DESTINATION lib)
install(DIRECTORY include/lightsky/draw DESTINATION include/lightsky)



# -------------------------------------
# LS Draw Testing Area
# -------------------------------------
set(LS_BUILD_DRAW_TESTS TRUE CACHE BOOL "Build tests for the LightDraw library.")

#if(LS_BUILD_DRAW_TESTS)
#    add_subdirectory("tests")
#endif(LS_BUILD_DRAW_TESTS)
