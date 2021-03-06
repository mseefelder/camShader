cmake_minimum_required(VERSION 2.8.12)
set(PROJECT_NAME "camShader")
project(${PROJECT_NAME} C CXX)

#
# Tucano
#

# Tucano directories.
set(TUCANO_DIR "/PATH/TO/TUCANO/ROOT")
set(TUCANO_BINARY_DIR "${TUCANO_DIR}/bin")
set(TUCANO_DOCUMENTATION_DIR "${TUCANO_DIR}/doc")
set(TUCANO_EFFECTS_DIR "${TUCANO_DIR}/effects")
set(TUCANO_SAMPLES_DIR "${TUCANO_DIR}/samples")
set(TUCANO_SRC_DIR "${TUCANO_DIR}/src")

# Build options.
option(DOC "build documentation" ON)
option(SAMPLES "build samples" ON)

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
  message(SEND_ERROR "GLEW not found on your system.")
endif()

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
  message(SEND_ERROR "OpenGL not found on your system.")
endif()

# Qt5.
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)
if (Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Eigen 3.
# pkg-config is used so we don't hard-code the location of eigen.
find_package(PkgConfig REQUIRED)
if(NOT PKG_CONFIG_FOUND)
  message(SEND_ERROR "pkg-config not found on your system")
endif()
pkg_check_modules(EIGEN3 REQUIRED eigen3)

include_directories(
  ${EIGEN3_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5OpenGL_INCLUDE_DIRS}
  ${TUCANO_EFFECTS_DIR}
  ${TUCANO_SRC_DIR}
  )

set(
  TUCANO_SOURCES
  ${TUCANO_SRC_DIR}/bufferobject.hpp
  ${TUCANO_SRC_DIR}/camera.hpp
  ${TUCANO_SRC_DIR}/effect.hpp
  ${TUCANO_SRC_DIR}/framebuffer.hpp
  ${TUCANO_SRC_DIR}/mesh.hpp
  ${TUCANO_SRC_DIR}/model.hpp
  ${TUCANO_SRC_DIR}/shader.hpp
  ${TUCANO_SRC_DIR}/texture.hpp
  ${TUCANO_SRC_DIR}/texturemanager.hpp
  ${TUCANO_SRC_DIR}/tucano.hpp
  )

set(
  TUCANO_SOURCES_UTILS
  ${TUCANO_SRC_DIR}/utils/misc.hpp
  ${TUCANO_SRC_DIR}/utils/trackball.hpp
  ${TUCANO_SRC_DIR}/utils/objimporter.hpp
  ${TUCANO_SRC_DIR}/utils/plyimporter.hpp
  ${TUCANO_SRC_DIR}/utils/qtplainwidget.hpp
  ${TUCANO_SRC_DIR}/utils/qttrackballwidget.hpp
  ${TUCANO_SRC_DIR}/utils/rply.hpp
  )

add_library(
  TUCANO_LIBRARY
  OBJECT
  ${TUCANO_SOURCES}
  ${TUCANO_SOURCES_UTILS}
  )

#
# camShader
#
set(CAMSHADER_DIR "${PROJECT_SOURCE_DIR}")
include_directories(${CAMSHADER_DIR})

#find_package(X11 REQUIRED)
#if(NOT X11_FOUND)
#  message(SEND_ERROR "X11 not found on your system.")
#endif()

#find_package(LibAV REQUIRED)
find_package( OpenCV REQUIRED )
  
set(
  CAMSHADER_SOURCES
  ${CAMSHADER_DIR}/main.cpp
  ${CAMSHADER_DIR}/mainwindow.cpp
  ${CAMSHADER_DIR}/glwidget.cpp
  )

set(
  VOLUME_HEADERS
  ${CAMSHADER_DIR}/mainwindow.h
  ${CAMSHADER_DIR}/glwidget.hpp
  ${CAMSHADER_DIR}/cameraexception.hpp
  ${CAMSHADER_DIR}/freeze.hpp
  )

qt5_wrap_ui(
  CAMSHADER_UIS
  ${CAMSHADER_DIR}/mainwindow.ui
  )

include_directories( 
  ${OpenCV_INCLUDE_DIRS} 
  )

add_executable(
  camShader

  $<TARGET_OBJECTS:TUCANO_LIBRARY>
  ${CAMSHADER_SOURCES}
  ${CAMSHADER_HEADERS}
  ${CAMSHADER_UIS}
  )

add_dependencies(
  camShader

  TUCANO_LIBRARY
  )

target_link_libraries(
  camShader

  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  #pthread
  Qt5::OpenGL
  Qt5::Widgets
  #${X11_LIBRARIES}
  ${OpenCV_LIBS}
  )
