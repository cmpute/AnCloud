cmake_minimum_required(VERSION 3.1)
project(SelectionExtension VERSION 0.1)

if(ParaView_SOURCE_DIR)
  include_directories(${VTK_INCLUDE_DIRS})

else()
  find_package(ParaView REQUIRED)
  include(${PARAVIEW_USE_FILE})

endif()

ADD_PARAVIEW_PLUGIN(SelectionExtension "0.1" 
  SERVER_MANAGER_XML SelectionExtension.xml
  SERVER_MANAGER_SOURCES vtkSelectionOperationFilter.cxx
  REQUIRED_ON_SERVER)
