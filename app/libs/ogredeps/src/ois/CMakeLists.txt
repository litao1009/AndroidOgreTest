project(OIS)

set(OIS_HEADERS
  includes/OISConfig.h
  includes/OISEffect.h
  includes/OISEvents.h
  includes/OISException.h
  includes/OISFactoryCreator.h
  includes/OISForceFeedback.h
  includes/OIS.h
  includes/OISInputManager.h
  includes/OISInterface.h
  includes/OISJoyStick.h
  includes/OISKeyboard.h
  includes/OISMouse.h
  includes/OISMultiTouch.h
  includes/OISObject.h
  includes/OISPrereqs.h
)
set(OIS_SOURCES
  src/OISEffect.cpp
  src/OISException.cpp
  src/OISForceFeedback.cpp
  src/OISInputManager.cpp
  src/OISJoyStick.cpp
  src/OISKeyboard.cpp
  src/OISObject.cpp
  src/extras/LIRC/OISLIRC.cpp
  src/extras/LIRC/OISLIRCFactoryCreator.cpp
)
if (WIN32)
elseif (APPLE)
elseif (ANDROID)
  set(OIS_PLATFORM android)
elseif (EMSCRIPTEN)
  set(OIS_PLATFORM emscripten)
else()
  set(OIS_PLATFORM_HEADERS
    includes/linux/EventHelpers.h
    includes/linux/LinuxForceFeedback.h
    includes/linux/LinuxInputManager.h
    includes/linux/LinuxJoyStickEvents.h
    includes/linux/LinuxKeyboard.h
    includes/linux/LinuxMouse.h
    includes/linux/LinuxPrereqs.h
  )
  set(OIS_PLATFORM linux)
  set(OIS_PLATFORM_SOURCES
    src/linux/EventHelpers.cpp
    src/linux/LinuxForceFeedback.cpp
    src/linux/LinuxInputManager.cpp
    src/linux/LinuxJoyStickEvents.cpp
    src/linux/LinuxKeyboard.cpp
    src/linux/LinuxMouse.cpp
  )
endif ()

include_directories(includes)

if(NOT APPLE AND NOT ANDROID)
  add_definitions(-DOIS_NONCLIENT_BUILD -DOIS_DYNAMIC_LIB)
  add_library(OIS SHARED ${OIS_HEADERS} ${OIS_SOURCES} ${OIS_PLATFORM_HEADERS} ${OIS_PLATFORM_SOURCES})
else ()
  add_definitions(-DOIS_NONCLIENT_BUILD)
  add_library(OIS SHARED ${OIS_HEADERS} ${OIS_SOURCES} ${OIS_PLATFORM_HEADERS} ${OIS_PLATFORM_SOURCES})
endif ()
if (OGRE_PROJECT_FOLDERS)
	set_property(TARGET OIS PROPERTY FOLDER Dependencies)
endif ()

target_link_libraries(OIS ${OIS_PLATFORM_LIBS})

set(OIS_INCLUDE_DIR "${OIS_SOURCE_DIR}/includes" CACHE PATH "" FORCE)
set(OIS_LIBRARY_DBG "OIS" CACHE STRING "" FORCE)
set(OIS_LIBRARY_REL "OIS" CACHE STRING "" FORCE)
mark_as_advanced(OIS_INCLUDE_DIR OIS_LIBRARY_DBG OIS_LIBRARY_REL)