cmake_minimum_required(VERSION 2.8)

# -------------------------------------
# Project Setup
# -------------------------------------
project(LightMath CXX)
set(OUTPUT_NAME lsdraw)



# -------------------------------------
# Compiler Options
# -------------------------------------
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU"
    OR
    "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"
)
    set(warnings "-Wall -Werror -Wextra -pedantic -pedantic-errors")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 ${warnings}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${warnings}")

    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG} -std=c99 ${warnings}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 ${warnings}")

    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE} -std=c99 ${warnings}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 ${warnings}")
else()
    message(FATAL_ERROR "Your compiler is currently unsupported.")
endif()



# -------------------------------------
# Source Paths
# -------------------------------------
include_directories("include")



# -------------------------------------
# Library Setup
# -------------------------------------
add_library("${OUTPUT_NAME}" STATIC
            "src/draw/BlendObject.cpp"
            "src/draw/BoundingBox.cpp"
            "src/draw/BufferObject.cpp"
            "src/draw/Camera.cpp"
            "src/draw/Color.cpp"
            "src/draw/DepthObject.cpp"
            "src/draw/Framebuffer.cpp"
            "src/draw/GeometryUtils.cpp"
            "src/draw/LSGL.cpp"
            "src/draw/MatrixStack.cpp"
            "src/draw/RenderBuffer.cpp"
            "src/draw/Setup.cpp"
            "src/draw/ShaderObject.cpp"
            "src/draw/ShaderProgram.cpp"
            "src/draw/ShaderUniform.cpp"
            "src/draw/Texture.cpp"
            "src/draw/Transform.cpp"
            "src/draw/VertexArray.cpp"
            "src/draw/VertexAttrib.cpp"
            "src/draw/VertexUtils.cpp"
)

# Dependencies
target_link_libraries("${OUTPUT_NAME}" liblssetup liblsutils liblsmath)



# -------------------------------------
# Library Installation Setup
# -------------------------------------
install(TARGETS "${OUTPUT_NAME}" ARCHIVE DESTINATION lib)
install(DIRECTORY include/lightsky/draw DESTINATION include/lightsky)
