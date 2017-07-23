# Reference: http://qt-project.org/doc/qt-5.0/qtdoc/cmake-manual.html
#			 https://cmake.org/cmake/help/v3.9/manual/cmake-qt.7.html

cmake_minimum_required(VERSION 3.1)
project(AnCloud VERSION 0.1)

# -------------------------------------------------------------------------------------------------
# Basic information about project

if (NOT PROJECT_VERSION_PATCH)
  set(PROJECT_VERSION_PATCH 0)
endif()

if (NOT PROJECT_VERSION_TWEAK)
  set(PROJECT_VERSION_TWEAK 0)
endif()

# -------------------------------------------------------------------------------------------------
# Add Files

# Use the include path and library for Qt that is used by VTK.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC_FILES
  bootstrap.cxx
  Components/Toolbars/acMainToolbar.cxx
  Components/acMainWindow.cxx
  Components/acMenuBuilders.cxx
  Reactions/acLoadDataReaction.cxx
  Reactions/acReaction.cxx
  Reactions/acSaveDataReaction.cxx
  Reactions/acUndoRedoReaction.cxx
)

set(HDR_FILES
  macros.h
  Components/acMenuBuilders.h
)

set(MOC_HDRS_FILES
  Components/Toolbars/acMainToolbar.h
  Components/acMainWindow.h
  Reactions/acLoadDataReaction.h
  Reactions/acReaction.h
  Reactions/acSaveDataReaction.h
  Reactions/acUndoRedoReaction.h
)

set(UI_FILES
  Components/Views/acEditMenu.ui
  Components/Views/acFileMenu.ui
  Components/Views/acHelpMenu.ui
  Components/Views/acMainToolbar.ui
  Components/Views/acMainWindow.ui
  Components/Views/acViewMenu.ui
)

set(QRC_FILES
  Resources/Icons/icons.qrc
)

# -------------------------------------------------------------------------------------------------
# Include libraries

find_package(VTK 8.0 REQUIRED NO_MODULE QUIET)
include(${VTK_USE_FILE})

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK not found or was not built with Qt")
endif()

if(VTK_QT_VERSION VERSION_LESS "5")
  message(FATAL_ERROR "Building on Qt4 or older has not been tested")
endif()

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
qt5_wrap_ui(UI_Srcs ${UI_FILES})
qt5_add_resources(QRC_Srcs ${QRC_FILES} )

# -------------------------------------------------------------------------------------------------
# Setup source groups

source_group("Resources" FILES
  ${UI_FILES}
  ${QRC_FILES}
)

source_group("Generated" FILES
  ${UI_Srcs}
  ${QRC_Srcs}
)

# -------------------------------------------------------------------------------------------------
# Compiling

add_executable(AnCloud WIN32
  ${SRC_FILES} ${HDR_FILES} ${UI_Srcs} ${MOC_HDRS_FILES} ${QRC_Srcs})
qt5_use_modules(AnCloud Widgets)
target_link_libraries(AnCloud ${VTK_LIBRARIES})
