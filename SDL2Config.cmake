
# #####################################
# External build for SDL2
# #####################################
set(SDL2_BRANCH "release-2.0.8" CACHE STRING "Git branch or tag for checking out SDL2.")
mark_as_advanced(SDL2_BRANCH)

# Configure build options
set(SDL2_BUILD_FLAGS
    -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCMAKE_INSTALL_PREFIX:STRING=${EXTERNAL_PROJECT_PREFIX}
    -DBUILD_SHARED_LIBS:BOOL=FALSE
    -DSNDIO:BOOL=OFF)
mark_as_advanced(SDL2_BUILD_FLAGS)

# Build SDL2
ExternalProject_Add(
    Sdl2
    PREFIX
        ${EXTERNAL_PROJECT_PREFIX}
    HG_REPOSITORY
        "https://hg.libsdl.org/SDL"
    HG_TAG
        "${SDL2_BRANCH}"
    CMAKE_COMMAND
        ${CMAKE_COMMAND}
    CMAKE_CACHE_ARGS
        ${SDL2_BUILD_FLAGS}
    BUILD_COMMAND
        ${CMAKE_COMMAND} -E chdir ${EXTERNAL_PROJECT_PREFIX}/src/Sdl2-build ${CMAKE_COMMAND} --build . --config ${CMAKE_CFG_INTDIR}
    INSTALL_DIR
        ${EXTERNAL_PROJECT_PREFIX}
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E chdir ${EXTERNAL_PROJECT_PREFIX}/src/Sdl2-build ${CMAKE_COMMAND} --build . --config ${CMAKE_CFG_INTDIR} --target install
)

# Add the imported library target
add_library(SDL2 STATIC IMPORTED)
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${EXTERNAL_PROJECT_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(SDL2 Sdl2)

set(SDL2_LIBS SDL2 pthread dl)
