cmake_minimum_required(VERSION 2.8.10)

project(OGREDEPS)

set(CMAKE_MODULE_PATH "${OGREDEPS_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

if (ANDROID)
  SET(TargetPlatform "Android")
  SET(OGRE_PLATFORM OGRE_PLATFORM_ANDROID)
endif()

add_subdirectory(src)
