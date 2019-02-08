
# #####################################
# External build for FreeType
# #####################################
find_package(Freetype)

if (FREETYPE_INCLUDE_DIRS AND FREETYPE_LIBRARIES)
    include_directories(${FREETYPE_INCLUDE_DIRS})
else()
    message("-- Building Freetype from source")

    set(FREETYPE_BRANCH "master" CACHE STRING "Git branch or tag for checking out FreeType.")
    mark_as_advanced(FREETYPE_BRANCH)

    # Configure build options
    set(FREETYPE_BUILD_FLAGS
        -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
        -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
        -DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
        -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
        -DCMAKE_INSTALL_PREFIX:STRING=${EXTERNAL_PROJECT_PREFIX}
        -DBUILD_SHARED_LIBS:BOOL=FALSE
        -DCMAKE_DISABLE_FIND_PACKAGE_HarfBuzz:BOOL=TRUE)
    mark_as_advanced(FREETYPE_BUILD_FLAGS)

    # Build FreeType
    ExternalProject_Add(
        FreeType
        PREFIX
            ${EXTERNAL_PROJECT_PREFIX}
        GIT_REPOSITORY
            "https://git.savannah.nongnu.org/r/freetype/freetype2.git"
        GIT_SHALLOW
            TRUE
        GIT_PROGRESS
            TRUE
        GIT_TAG
            "${FREETYPE_BRANCH}"
        UPDATE_COMMAND
            ${GIT_EXECUTABLE} fetch
        CMAKE_COMMAND
            ${CMAKE_COMMAND}
        CMAKE_CACHE_ARGS
            ${FREETYPE_BUILD_FLAGS}
        BUILD_COMMAND
            ${CMAKE_COMMAND} -E chdir ${EXTERNAL_PROJECT_PREFIX}/src/FreeType-build ${CMAKE_COMMAND} --build . --config ${CMAKE_CFG_INTDIR}
        INSTALL_DIR
            ${EXTERNAL_PROJECT_PREFIX}
        INSTALL_COMMAND
        ${CMAKE_COMMAND} -E chdir ${EXTERNAL_PROJECT_PREFIX}/src/FreeType-build ${CMAKE_COMMAND} --build . --config ${CMAKE_CFG_INTDIR} --target install
    )

    # Add the imported library target
    add_library(freetype STATIC IMPORTED)
    set_target_properties(freetype PROPERTIES IMPORTED_LOCATION ${EXTERNAL_PROJECT_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}freetype${CMAKE_STATIC_LIBRARY_SUFFIX})
    add_dependencies(freetype FreeType)

    include_directories(BEFORE SYSTEM ${EXTERNAL_PROJECT_PREFIX}/include/freetype2)

    set(FREETYPE_LIBRARIES freetype)

endif()