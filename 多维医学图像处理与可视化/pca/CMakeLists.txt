cmake_minimum_required(VERSION 2.8)
 
PROJECT(pca)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(pca pca.cpp)
 
if(VTK_LIBRARIES)
  target_link_libraries(pca ${VTK_LIBRARIES})
else()
  target_link_libraries(pca vtkHybrid vtkWidgets)
endif()