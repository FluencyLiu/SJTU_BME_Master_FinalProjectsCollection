cmake_minimum_required(VERSION 2.8.11)

project(medImgViewer)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


# Find the QtWidgets library
set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.5.0\\5.5\\msvc2013_64\\")
find_package(Qt5Widgets REQUIRED)

# Tell CMake to create the medImgViewer executable

file(GLOB SRC_FILES "./*.cpp") 
file(GLOB HEAD_FILES "./*.h")
file(GLOB UI_FILES "./*.ui")
qt5_wrap_ui(WRAP_FILES ${UI_FILES})
qt5_wrap_cpp(MOCSrcs ${QT_WRAP})

set(ITK_DIR "E:/ITK-Box/ITK-Build")
set(VTK_DIR "E:/VTK-Box/VTK-Build")
list(APPEND env{path} "E:/VTK-Box/VTK-Build/bin/Debug")
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
else()
  find_package(ItkVtkGlue REQUIRED)
  include(${ItkVtkGlue_USE_FILE})
  set(Glue ItkVtkGlue)
endif()

add_executable(medImgViewer WIN32 ${SRC_FILES} ${HEAD_FILES} ${WRAP_FILES} ${MOCSrcs})

# Use the ITK VTK and Widgets module from Qt 5.
target_link_libraries(medImgViewer Qt5::Widgets ${Glue} ${ITK_LIBRARIES} ${VTK_LIBRARIES})