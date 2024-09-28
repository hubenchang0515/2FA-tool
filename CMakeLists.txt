cmake_minimum_required(VERSION 3.2)
project(2FA-tool)

find_package(Qt6 COMPONENTS Core Widgets QUIET)

if (NOT Qt6_FOUND)
  find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
endif (NOT Qt6_FOUND)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)
SET(CXX_STANDARD 11)

add_subdirectory(3rdparty EXCLUDE_FROM_ALL)

aux_source_directory(src SOURCE)

if (NOT Qt6_FOUND)
  add_executable(${PROJECT_NAME} ${SOURCE})
else()
  qt_add_executable(${PROJECT_NAME} ${SOURCE})
endif(NOT Qt6_FOUND)

target_include_directories(${PROJECT_NAME} PRIVATE quirc)
target_link_directories(${PROJECT_NAME} PRIVATE quirc)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Widgets quirc)