# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Qtests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Qtests_autogen.dir\\ParseCache.txt"
  "Qtests_autogen"
  )
endif()
