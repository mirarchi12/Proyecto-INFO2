cmake_minimum_required(VERSION 3.16)

project(programa_QT LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS SerialPort)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)


set(TS_FILES programa_QT_es_AR.ts)

add_executable(programa_QT
  main.cpp
  ${TS_FILES}
  Serial.h
  Serial.cpp
  sensor.h sensor.cpp
  player.h player.cpp
  looper.h looper.cpp


)
target_link_libraries(programa_QT PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(programa_QT PRIVATE Qt6::SerialPort)
target_link_libraries(programa_QT PRIVATE Qt6::Multimedia)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

include(GNUInstallDirs)
install(TARGETS programa_QT
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
