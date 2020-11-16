cmake_minimum_required(VERSION 3.5)

project(WeatherApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(WeatherApp
  main.cpp
  app.cpp
  app.h
  app.ui

  src/weather.cpp
  src/weather.h
  src/place.cpp
  src/place.h
  src/weatherinfo.cpp
  src/weatherinfo.h
  src/config.cpp
  src/config.h
  src/logic.h

  assets.qrc
  )

target_link_libraries(WeatherApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(WeatherApp PRIVATE cpr)
