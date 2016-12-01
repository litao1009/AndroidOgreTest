cmake_minimum_required(VERSION 3.4.1)


######################################################################  NativeMain
set( NATIVE_SOURCE_DIR
     src/main/cpp
     )

set( NATIVE_SOURCE
    ${NATIVE_SOURCE_DIR}/JNIBridge.cpp
    ${NATIVE_SOURCE_DIR}/IEvent.h
    ${NATIVE_SOURCE_DIR}/EventQueue.h
    ${NATIVE_SOURCE_DIR}/EventQueue.cpp
    ${NATIVE_SOURCE_DIR}/NativeBridge.h
    ${NATIVE_SOURCE_DIR}/NativeBridge.cpp
    )

list(APPEND
    NATIVE_SOURCE

    ${NATIVE_SOURCE_DIR}/Camera/MayaCamera.cpp
    ${NATIVE_SOURCE_DIR}/Camera/MayaCamera.h
    ${NATIVE_SOURCE_DIR}/OgreEnv.cpp
    ${NATIVE_SOURCE_DIR}/OgreEnv.h
    ${NATIVE_SOURCE_DIR}/InputEvent.cpp
    ${NATIVE_SOURCE_DIR}/InputEvent.h
    ${NATIVE_SOURCE_DIR}/InputEventFwd.h
    ${NATIVE_SOURCE_DIR}/IFrameListener.cpp
    ${NATIVE_SOURCE_DIR}/IFrameListener.h
    ${NATIVE_SOURCE_DIR}/IFrameListenerFwd.h
    ${NATIVE_SOURCE_DIR}/ICameraFrameListener.cpp
    ${NATIVE_SOURCE_DIR}/ICameraFrameListener.h
    ${NATIVE_SOURCE_DIR}/SceneListener/DummySceneListener.cpp
    ${NATIVE_SOURCE_DIR}/SceneListener/DummySceneListener.h
    )

add_library( NativeMain SHARED
             ${NATIVE_SOURCE} )

target_include_directories(NativeMain PRIVATE
                           ${NATIVE_SOURCE_DIR}
                           )

target_link_libraries( NativeMain
                       android
                       log
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
    ${OIS_INCLUDE_DIR}
    )

target_link_libraries( NativeMain
                       OgreMain
                       RenderSystem_GLES2
                       Plugin_ParticleFX
                       OIS
                        )

target_include_directories(NativeMain PRIVATE
                            ${OGRE_NATIVE_INCLUDE}
                            )
######################################################################  Ogre