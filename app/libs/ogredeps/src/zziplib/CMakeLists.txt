project(zziplib)

set(zziplib_SOURCES
  zzip/autoconf.h
  zzip/conf.h
  zzip/_config.h
  zzip/__debug.h
  zzip/dir.c
  zzip/__dirent.h
  zzip/err.c
  zzip/fetch.c
  zzip/fetch.h
  zzip/file.c
  zzip/file.h
  zzip/__fnmatch.h
  zzip/format.h
  zzip/fseeko.c
  zzip/fseeko.h
  zzip/__hints.h
  zzip/info.c
  zzip/info.h
  zzip/lib.h
  zzip/memdisk.c
  zzip/memdisk.h
  zzip/__mmap.h
  zzip/mmapped.c
  zzip/mmapped.h
  zzip/_msvc.h
  zzip/plugin.c
  zzip/plugin.h
  zzip/stat.c
  zzip/stdint.h
  zzip/types.h
  zzip/write.c
  zzip/write.h
  zzip/zip.c
  zzip/zzip32.h
  zzip/zzip.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(zziplib STATIC ${zziplib_SOURCES})

set(ZZip_INCLUDE_DIR "${zziplib_SOURCE_DIR}" CACHE PATH "" FORCE)
set(ZZip_LIBRARY_DBG "zziplib" CACHE STRING "" FORCE)
set(ZZip_LIBRARY_REL "zziplib" CACHE STRING "" FORCE)
mark_as_advanced(ZZip_INCLUDE_DIR ZZip_LIBRARY_DBG ZZip_LIBRARY_REL)
