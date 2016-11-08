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
add_library( NativeMain SHARED
             src/main/cpp/NativeMain.cpp )

target_include_directories(NativeMain PRIVATE
                           ${APP_GLUE_DIR})

target_link_libraries( NativeMain
                       appGlue
                       android
                       log
                       )
######################################################################  NativeMain


######################################################################  OgreDeps
add_subdirectory(libs/ogredeps)
######################################################################  OgreDeps


######################################################################  Ogre
add_subdirectory(libs/ogre)
######################################################################  Ogre