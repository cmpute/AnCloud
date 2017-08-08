# Reference: http://qt-project.org/doc/qt-5.0/qtdoc/cmake-manual.html
#			 https://cmake.org/cmake/help/v3.9/manual/cmake-qt.7.html

cmake_minimum_required(VERSION 3.1)
project(AnCloud VERSION 0.1)

# Disallow in-source build
if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL_ERROR
    "AnCloud requires an out of source Build. "
    "Please create a separate binary directory (recommend ./Build) and run CMake there.")
endif()

# -------------------------------------------------------------------------------------------------
# Basic information about project

if (NOT PROJECT_VERSION_PATCH)
  set(PROJECT_VERSION_PATCH 0)
endif()

if (NOT PROJECT_VERSION_TWEAK)
  set(PROJECT_VERSION_TWEAK 0)
endif()

set(AC_INSTALL_RUNTIME_DIR bin)
if (WIN32)
  set(AC_INSTALL_LIBRARY_DIR bin)
else()
  set(AC_INSTALL_LIBRARY_DIR
    lib/ancloud-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})
endif()

# -------------------------------------------------------------------------------------------------
# Add Files

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC_FILES
  Components/Toolbars/acCameraToolbar.cxx
  Components/Toolbars/acMainToolbar.cxx
  Components/Toolbars/acSelectionToolbar.cxx
  Components/Widgets/acLabelViewWidget.cxx
  Components/Widgets/acSelectionBrowserWidget.cxx
  Components/Widgets/acSelectionInfoWidget.cxx
  Components/Widgets/acSelectionModel.cxx
  Components/Widgets/acSelectionViewWidget.cxx
  Components/acApplicationCore.cxx
  Components/acComponentsInit.cxx
  Components/acMainWindow.cxx
  Components/acMenuBuilders.cxx
  Components/acSelectionAdaptor.cxx
  Core/acSelectionManager.cxx
  Reactions/acDeleteReaction.cxx
  Reactions/acLoadDataReaction.cxx
  Reactions/acReaction.cxx
  Reactions/acSaveDataReaction.cxx
  Reactions/acUndoRedoReaction.cxx
)

set(HDR_FILES
  Components/acComponentsInit.h
  Components/acMenuBuilders.h
  Core/acSelectionManager.h
)

set(MOC_HDRS_FILES
  Components/Toolbars/acCameraToolbar.h
  Components/Toolbars/acMainToolbar.h
  Components/Toolbars/acSelectionToolbar.h
  Components/Widgets/acLabelViewWidget.h
  Components/Widgets/acSelectionBrowserWidget.h
  Components/Widgets/acSelectionInfoWidget.h
  Components/Widgets/acSelectionModel.h
  Components/Widgets/acSelectionViewWidget.h
  Components/acApplicationCore.h
  Components/acMainWindow.h
  Components/acSelectionAdaptor.h
  Reactions/acDeleteReaction.h
  Reactions/acLoadDataReaction.h
  Reactions/acReaction.h
  Reactions/acSaveDataReaction.h
  Reactions/acUndoRedoReaction.h
)

set(UI_FILES
  Components/Views/acCameraToolbar.ui
  Components/Views/acEditMenu.ui
  Components/Views/acFileMenu.ui
  Components/Views/acHelpMenu.ui
  Components/Views/acMainToolbar.ui
  Components/Views/acMainWindow.ui
  Components/Views/acSelectionInfoWidget.ui
  Components/Views/acSelectionToolbar.ui
  Components/Views/acSelectionViewWidget.ui
)

set(QRC_FILES
  Resources/acIcons.qrc
)

set(EXT_Srcs)

# -------------------------------------------------------------------------------------------------
# Options

option(BUILD_SHARED_LIBS "Build shared libs" ON)
# ParaView 5.x does not support build development installing currently
# option(USE_EXTERNAL_VTK "Using external VTK installation" OFF)
option(USE_EXTERNAL_ParaView "Using external ParaView installation" ON)
option(USE_EXTERNAL_Qt "Using external Qt installation" ON)

set (ANCLOUD_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

# -------------------------------------------------------------------------------------------------
# Include libraries

# find_package(VTK 8.0 REQUIRED NO_MODULE QUIET)
# include(${VTK_USE_FILE})

# if("${VTK_QT_VERSION}" STREQUAL "")
#   message(FATAL_ERROR "VTK not found or was not built with Qt")
# endif()

# if(VTK_QT_VERSION VERSION_LESS "5")
#   message(FATAL_ERROR "Building on Qt4 or older has not been tested")
# endif()

if(USE_EXTERNAL_ParaView)
  find_package(ParaView REQUIRED)

  include(${PARAVIEW_USE_FILE})
else()
  # TODO: This part is not completed, need to figure out how to build ParaView in place.

  # Set CMake module path
  set (PARAVIEW_CMAKE_DIR ${AnCloud_SOURCE_DIR}/ParaView/CMake)

  # Set default ParaView build options
  set (PARAVIEW_BUILD_QT_GUI ON CACHE INTERNAL "")
  set (PARAVIEW_ENABLE_CATALYST OFF CACHE BOOL "" FORCE)
  set (PARAVIEW_ENABLE_PYTHON OFF CACHE BOOL "" FORCE)
  set (PARAVIEW_ENABLE_XDMF2 OFF CACHE BOOL "" FORCE)
  mark_as_advanced(
    PARAVIEW_ENABLE_CATALYST
    PARAVIEW_ENABLE_PYTHON
    PARAVIEW_ENABLE_XDMF2
  )
  set (PARAVIEW_BUILD_PLUGIN_AdiosReader OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_AnalyzeNIfTIIO OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_ArrowGlyph OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_CoProcessingScriptGenerator OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_EyeDomeLighting OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_ForceTime OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_GMVReader OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_MantaView OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_MobileRemoteControl OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_Moments OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_NonOrthogonalSource OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_PacMan OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_PointSprite OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_PrismPlugin OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_pvblot OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_QuadView OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_SLACTools OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_SciberQuestToolKit OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_SierraPlotTools OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_SurfaceLIC OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_UncertaintyRendering OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_VRPlugin OFF CACHE INTERNAL "")
  set (PARAVIEW_BUILD_PLUGIN_VaporPlugin OFF CACHE INTERNAL "")
  set (VTK_USE_VIDEO_FOR_WINDOWS OFF CACHE INTERNAL "")

  include (${PARAVIEW_CMAKE_DIR}/UseParaView.cmake)
endif()
# Update CMAKE_MODULE_PATH to include Paraview's cmake files.
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PARAVIEW_CMAKE_DIR})

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

if(USE_EXTERNAL_Qt)
  find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
endif()
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

source_group("ThirdParty" FILES
  ${EXT_Srcs}
)

# -------------------------------------------------------------------------------------------------
# Import Plugin
add_subdirectory(Plugins/PCDReader)
include_directories(Plugins/PCDReader)
add_subdirectory(Plugins/SelectionExtension)
include_directories(Plugins/SelectionExtension)
add_subdirectory(Plugins/Debug)
include_directories(Plugins/Debug)

# -------------------------------------------------------------------------------------------------
# Compiling

build_paraview_client(AnCloud
  TITLE "AnCloud"
  ORGANIZATION "Tsinghua University"
  VERSION_MAJOR ${PROJECT_VERSION_MAJOR}
  VERSION_MINOR ${PROJECT_VERSION_MINOR}
  VERSION_PATCH ${PROJECT_VERSION_PATCH}
  PVMAIN_WINDOW acMainWindow
  PVMAIN_WINDOW_INCLUDE Components/acMainWindow.h
  SPLASH_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Splash.jpg"
  BUNDLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Resources/AppIcon.icns"
  APPLICATION_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Resources/AppIcon.ico"
  SOURCES
    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_HDRS_FILES}
    ${UI_Srcs}
    ${QRC_Srcs}
    ${EXT_Srcs}
  INSTALL_BIN_DIR ${AC_INSTALL_RUNTIME_DIR}
  INSTALL_LIB_DIR ${AC_INSTALL_LIBRARY_DIR}
)

# add_executable(AnCloud WIN32
#   ${SRC_FILES} ${HDR_FILES} ${UI_Srcs} ${MOC_HDRS_FILES} ${QRC_Srcs})
# qt5_use_modules(AnCloud Widgets)
# target_link_libraries(AnCloud ${VTK_LIBRARIES})

target_link_libraries(AnCloud
  LINK_PRIVATE vtkPVClientServerCoreRendering ${VTK_LIBRARIES} LINK_PUBLIC PCDReader) #${PARAVIEW_DEFAULT_VTK_MODULES}
