#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

#############################################################
# Install useful CMake modules.
# These are necessary to compile the samples from the install
# directory, but can also be used for custom projects.
#############################################################

set(OGRE_CMAKE_DIR "${OGRE_LIB_DIRECTORY}/OGRE/cmake")

set(INST_FILES
  Packages/FindOIS.cmake
  Packages/FindOGRE.cmake
  Utils/FindPkgMacros.cmake
  Utils/MacroLogFeature.cmake
  Utils/PreprocessorUtils.cmake
  Utils/PrecompiledHeader.cmake
  Utils/OgreAddTargets.cmake
  Utils/OgreConfigTargets.cmake
  Utils/OgreGetVersion.cmake
  Utils/OgreFindFrameworks.cmake
) 

set(STATIC_INST_FILES
  Packages/FindCg.cmake
  Packages/FindDirectX.cmake
  Packages/FindFreeImage.cmake
  Packages/FindFreetype.cmake
  Packages/FindOpenGLES.cmake
  Packages/FindOpenGLES2.cmake
  Packages/FindZLIB.cmake
  Packages/FindZZip.cmake
  Packages/FindSoftimage.cmake
)

install(FILES ${INST_FILES} DESTINATION ${OGRE_CMAKE_DIR})
if (OGRE_STATIC)
  install(FILES ${STATIC_INST_FILES} DESTINATION ${OGRE_CMAKE_DIR})
endif ()
