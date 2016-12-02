#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# check the contents of a given source file. If they differ from the
# expected content, or the file does not exist, rewrite it with the
# provided content.
# This function is used in order to update Unity build files only when
# necessary. If we rewrote them unconditionally, it might trigger an
# unnecessary rebuild of the file.
function(check_and_update_file FILENAME CONTENT)
  # read current file contents
  if (EXISTS ${FILENAME})
    file(READ ${FILENAME} _CUR)
  else ()
    # create empty file
    file(WRITE ${FILENAME} "")
    set(_CUR "")
  endif ()
  if (NOT _CUR STREQUAL CONTENT)
    # rewrite file with new contents
    message(STATUS "  Updating ${FILENAME}...")
    file(WRITE ${FILENAME} ${CONTENT})
  else ()
    message(STATUS "  ${FILENAME} is up to date.")
  endif ()
endfunction()


# generate unity build files for the given target.
# If in the list of source files the key word SEPARATE is specified, then
# any source file after that will be compiled separately.
macro(create_unity_build_files TARGETNAME)
  # first step: build the primary and separate lists
  set(_PRIMARY "")
  set(_EXCLUDES "")
  set(_SEP FALSE)
  foreach(_FILE ${ARGN})
    if (_FILE STREQUAL "SEPARATE")
      set(_SEP TRUE)
    else ()
      if (_SEP)
        list(APPEND _EXCLUDES ${_FILE})
      else ()
        list(APPEND _PRIMARY ${_FILE})
      endif ()
    endif()
  endforeach()
  set(_SOURCES ${_PRIMARY} ${_EXCLUDES})
  list(REMOVE_DUPLICATES _SOURCES)
endmacro()


# add a new library target
# usage: ogre_add_library(TARGETNAME LIBTYPE SOURCE_FILES [SEPARATE SOURCE_FILES])
function(ogre_add_library TARGETNAME LIBTYPE)
  create_unity_build_files(${TARGETNAME} ${ARGN})
  add_library(${TARGETNAME} ${LIBTYPE} ${_SOURCES})
endfunction(ogre_add_library)