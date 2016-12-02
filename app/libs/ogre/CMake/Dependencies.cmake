#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

#######################################################################
# Find all necessary and optional OGRE dependencies
#######################################################################

# OGRE_DEPENDENCIES_DIR can be used to specify a single base
# folder where the required dependencies may be found.
set(OGRE_DEPENDENCIES_DIR "" CACHE PATH "Path to prebuilt OGRE dependencies")
include(FindPkgMacros)
getenv_path(OGRE_DEPENDENCIES_DIR)

set(OGRE_DEP_SEARCH_PATH 
    ${OGRE_DEPENDENCIES_DIR}
    ${ENV_OGRE_DEPENDENCIES_DIR}
    "${OGRE_BINARY_DIR}/AndroidDependencies"
    "${OGRE_SOURCE_DIR}/AndroidDependencies"
    "${OGRE_BINARY_DIR}/../AndroidDependencies"
    "${OGRE_SOURCE_DIR}/../AndroidDependencies"
  )

message(STATUS "Search path: ${OGRE_DEP_SEARCH_PATH}")

# Set hardcoded path guesses for various platforms
if (UNIX AND NOT EMSCRIPTEN)
  set(OGRE_DEP_SEARCH_PATH ${OGRE_DEP_SEARCH_PATH} /usr/local)
  # Ubuntu 11.10 has an inconvenient path to OpenGL libraries
  set(OGRE_DEP_SEARCH_PATH ${OGRE_DEP_SEARCH_PATH} /usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-gnu)
endif ()

# give guesses as hints to the find_package calls
set(CMAKE_PREFIX_PATH ${OGRE_DEP_SEARCH_PATH} ${CMAKE_PREFIX_PATH})
set(CMAKE_FRAMEWORK_PATH ${OGRE_DEP_SEARCH_PATH} ${CMAKE_FRAMEWORK_PATH})

#######################################################################
# Core dependencies
#######################################################################

# Find zlib
find_package(ZLIB)
macro_log_feature(ZLIB_FOUND "zlib" "Simple data compression library" "http://www.zlib.net" FALSE "" "")

if (ZLIB_FOUND)
  # Find zziplib
  find_package(ZZip)
  macro_log_feature(ZZip_FOUND "zziplib" "Extract data from zip archives" "http://zziplib.sourceforge.net" FALSE "" "")
endif ()

# Find FreeImage
find_package(FreeImage)
macro_log_feature(FreeImage_FOUND "freeimage" "Support for commonly used graphics image formats" "http://freeimage.sourceforge.net" FALSE "" "")

# Find FreeType
find_package(Freetype)
macro_log_feature(FREETYPE_FOUND "freetype" "Portable font engine" "http://www.freetype.org" FALSE "" "")


#######################################################################
# RenderSystem dependencies
#######################################################################

# Find OpenGL ES 1.x
find_package(OpenGLES)
macro_log_feature(OPENGLES_FOUND "OpenGL ES 1.x" "Support for the OpenGL ES 1.x render system (DEPRECATED)" "http://www.khronos.org/opengles/" FALSE "" "")

# Find OpenGL ES 2.x
find_package(OpenGLES2)
macro_log_feature(OPENGLES2_FOUND "OpenGL ES 2.x" "Support for the OpenGL ES 2.x render system" "http://www.khronos.org/opengles/" FALSE "" "")

# Find OpenGL ES 3.x
find_package(OpenGLES3)
macro_log_feature(OPENGLES3_FOUND "OpenGL ES 3.x" "Support for the OpenGL ES 2.x render system with OpenGL ES 3 support" "http://www.khronos.org/opengles/" FALSE "" "")

#######################################################################
# Samples dependencies
#######################################################################

# Find OIS
find_package(OIS)
macro_log_feature(OIS_FOUND "OIS" "Input library needed for the samples" "http://sourceforge.net/projects/wgois" FALSE "" "")

# now see if we have a buildable Dependencies package in
# the source tree. If so, include that, and it will take care of
# setting everything up, including overriding any of the above
# findings.
set(OGREDEPS_RUNTIME_OUTPUT ${OGRE_RUNTIME_OUTPUT})
if (EXISTS "${OGRE_SOURCE_DIR}/Dependencies/CMakeLists.txt")
  add_subdirectory(Dependencies)
elseif (EXISTS "${OGRE_SOURCE_DIR}/ogredeps/CMakeLists.txt")
  add_subdirectory(ogredeps)
endif ()


# Display results, terminate if anything required is missing
MACRO_DISPLAY_FEATURE_LOG()

# Add library and include paths from the dependencies
include_directories(
  ${ZLIB_INCLUDE_DIRS}
  ${ZZip_INCLUDE_DIRS}
  ${FreeImage_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${OPENGLES_INCLUDE_DIRS}
  ${OPENGLES2_INCLUDE_DIRS}
  ${OPENGLES3_INCLUDE_DIRS}
  ${OIS_INCLUDE_DIRS}
)

link_directories(
  ${OPENGLES_LIBRARY_DIRS}
  ${OPENGLES2_LIBRARY_DIRS}
  ${OPENGLES3_LIBRARY_DIRS}
)
