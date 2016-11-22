include(ExternalProject)

# determine if we are compiling for a 32bit or 64bit system
include(CheckTypeSize)
CHECK_TYPE_SIZE("void*" OGREDEPS_PTR_SIZE BUILTIN_TYPES_ONLY)
if (OGREDEPS_PTR_SIZE EQUAL 8)
  set(OGREDEPS_PLATFORM_X64 TRUE)
else ()
  set(OGREDEPS_PLATFORM_X64 FALSE)
endif ()


add_subdirectory(FreeImage)

add_subdirectory(freetype)

add_subdirectory(ois)

add_subdirectory(zziplib)

