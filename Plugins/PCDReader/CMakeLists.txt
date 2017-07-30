if(ParaView_SOURCE_DIR)
  include_directories(${VTK_INCLUDE_DIRS})

else()
  find_package(ParaView REQUIRED)
  include(${PARAVIEW_USE_FILE})

endif()

ADD_PARAVIEW_PLUGIN(PCDReader "1.0" 
  SERVER_MANAGER_XML PCDReader.xml
  SERVER_MANAGER_SOURCES vtkPCDReader.cxx
  SOURCES lzf.cpp PCDReaderRaw.cpp
  REQUIRED_ON_SERVER)
