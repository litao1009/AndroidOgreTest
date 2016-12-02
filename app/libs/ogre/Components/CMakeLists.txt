#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Configure Components build

# Use same build type for components as main build
set(OGRE_COMP_LIB_TYPE ${OGRE_LIB_TYPE})

add_subdirectory(MeshLodGenerator)

add_subdirectory(Overlay)
