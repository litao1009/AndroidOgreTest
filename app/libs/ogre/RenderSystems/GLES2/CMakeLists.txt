#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Configure OpenGL ES 2.0 RenderSystem build

file(GLOB HEADER_FILES "include/*.h")
file(GLOB SOURCE_FILES "src/*.cpp")

file(GLOB GLSLES_FILES "src/GLSLES/include/*.h" "src/GLSLES/src/*.cpp")

list(REMOVE_ITEM GLSLES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/GLSLES/src/OgreGLSLESCgProgram.cpp")
list(REMOVE_ITEM GLSLES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/GLSLES/src/OgreGLSLESCgProgramFactory.cpp")

if(OGRE_CONFIG_ENABLE_GLES3_SUPPORT)
  set(GLESW_HEADER_INSTALL "GLES3")
  file(GLOB GLESW_HEADERS "include/GLES3/*.h")
  list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/gles2w.cpp")
  list(APPEND HEADER_FILES
  )
  if (OGRE_BUILD_PLATFORM_APPLE_IOS)
    set_source_files_properties(src/gles3w.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
  endif()
else()
  set(GLESW_HEADER_INSTALL "GLES2")
  file(GLOB GLESW_HEADERS "include/GLES2/*.h")
  list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/gles3w.cpp")
endif()

file(GLOB KHR_HEADERS "include/KHR/*.h")

include_directories(src/StateCacheManager)
if(OGRE_CONFIG_ENABLE_GL_STATE_CACHE_SUPPORT)
  list(APPEND STATECACHE_HEADERS
      ${CMAKE_CURRENT_SOURCE_DIR}/src/StateCacheManager/OgreGLES2StateCacheManagerImp.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/StateCacheManager/OgreGLES2UniformCacheImp.h
  )
  list(APPEND SOURCE_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/src/StateCacheManager/OgreGLES2StateCacheManagerImp.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/StateCacheManager/OgreGLES2UniformCacheImp.cpp
  )
else()
  list(APPEND STATECACHE_HEADERS
      ${CMAKE_CURRENT_SOURCE_DIR}/src/StateCacheManager/OgreGLES2NullStateCacheManagerImp.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/StateCacheManager/OgreGLES2NullUniformCacheImp.h
  )
  list(APPEND SOURCE_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/src/StateCacheManager/OgreGLES2NullStateCacheManagerImp.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/StateCacheManager/OgreGLES2NullUniformCacheImp.cpp
  )
endif()

if(NOT OPENGLES3_FOUND)
  set(OPENGLES3_INCLUDE_DIR "")
endif()


file(GLOB PLATFORM_HEADERS "include/EGL/Android/*.h" "include/EGL/*.h")
file(GLOB PLATFORM_SOURCES "src/EGL/Android/*.cpp" "src/EGL/*.cpp")

include_directories(src/EGL/Android include/EGL/Android)
if(OGRE_CONFIG_ENABLE_GLES3_SUPPORT)
link_directories(${OPENGLES3_LIBRARY_PATH})
else()
link_directories(${OPENGLES2_LIBRARY_PATH})
endif()
set(PLATFORM_HEADER_INSTALL "Android")

# Add support for the RT Shader System
add_definitions(-DINCLUDE_RTSHADER_SYSTEM)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/EGL
  ${CMAKE_CURRENT_SOURCE_DIR}/include/EAGL
  ${CMAKE_CURRENT_SOURCE_DIR}/src/GLSLES/include
  ${CMAKE_SOURCE_DIR}/Components/RTShaderSystem/include
  ${OPENGLES3_INCLUDE_DIR} ${OPENGLES2_INCLUDE_DIR} ${EGL_INCLUDE_DIR}
)

ogre_add_library(RenderSystem_GLES2 ${OGRE_LIB_TYPE} ${HEADER_FILES} ${STATECACHE_HEADERS} ${GLESW_HEADERS} ${KHR_HEADERS} ${PLATFORM_HEADERS} ${SOURCE_FILES} ${PLATFORM_SOURCES} SEPARATE ${GLSLES_FILES})
target_link_libraries(RenderSystem_GLES2 OgreMain ${OPENGLES3_LIBRARIES} ${OPENGLES2_LIBRARIES} ${EGL_LIBRARIES} ${PLATFORM_LIBS})

if (NOT OGRE_STATIC)
  set_target_properties(RenderSystem_GLES2 PROPERTIES
    COMPILE_DEFINITIONS OGRE_GLES2PLUGIN_EXPORTS
  )
endif ()