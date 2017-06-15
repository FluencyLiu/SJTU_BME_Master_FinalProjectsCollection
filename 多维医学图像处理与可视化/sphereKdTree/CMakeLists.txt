cmake_minimum_required(VERSION 2.8)
 
PROJECT(sphereKdTree)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(sphereKdTree sphereKdTree.cpp)
 
if(VTK_LIBRARIES)
  target_link_libraries(sphereKdTree ${VTK_LIBRARIES})
else()
  target_link_libraries(sphereKdTree vtkHybrid vtkWidgets)
endif()