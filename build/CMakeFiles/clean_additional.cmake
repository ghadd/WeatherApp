# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/WeatherApp_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/WeatherApp_autogen.dir/ParseCache.txt"
  "WeatherApp_autogen"
  )
endif()
