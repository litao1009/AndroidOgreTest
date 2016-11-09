cmake_minimum_required(VERSION 3.4.1)


######################################################################  appGlue
set(APP_GLUE_DIR
    "${ANDROID_NDK}/sources/android/native_app_glue")

add_library(appGlue
            STATIC
            ${APP_GLUE_DIR}/android_native_app_glue.c )
######################################################################  appGlue


######################################################################  cpufeatures
add_library(cpufeatures
            STATIC
            ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c )

target_include_directories(cpufeatures PRIVATE
                             ${ANDROID_NDK}/sources/android/cpufeatures/)
######################################################################  cpufeatures


######################################################################  NativeMain
set( NATIVE_SOURCE_DIR
     src/main/cpp
     )

set( NATIVE_SOURCE
    ${NATIVE_SOURCE_DIR}/NativeMain.cpp
    ${NATIVE_SOURCE_DIR}/AndroidBridge.h
    ${NATIVE_SOURCE_DIR}/AndroidBridge.cpp
    )

add_library( NativeMain SHARED
             ${NATIVE_SOURCE} )

target_include_directories(NativeMain PRIVATE
                           ${APP_GLUE_DIR})

target_link_libraries( NativeMain
                       appGlue
                       android
                       log
                       cpufeatures
                       )
######################################################################  NativeMain


######################################################################  OgreDeps
add_subdirectory(libs/ogredeps)
######################################################################  OgreDeps


######################################################################  Ogre
add_subdirectory(libs/ogre)

set(OGRE_NATIVE_INCLUDE
    ${OGRE_SOURCE_DIR}/OgreMain/include
    ${OGRE_SOURCE_DIR}/PlugIns/ParticleFX/include
    ${OGRE_SOURCE_DIR}/RenderSystems/GLES2/include
    ${OGRE_SOURCE_DIR}/RenderSystems/GLES2/include/EGL
    ${OGRE_BINARY_DIR}/include
    )

target_link_libraries( NativeMain
                       OgreMain
                       RenderSystem_GLES2
                       OIS
                        )

target_include_directories(NativeMain PRIVATE
                            ${OGRE_NATIVE_INCLUDE}
                            )
######################################################################  Ogre