#--------------------------------------------------------
# The CMakeLists.txt for:              lib_behaviors-test
# Author(s): Mike Bogochow                               
#--------------------------------------------------------

# Set System Specific Libraries
if (${WIN32})
  # Windows Libraries
  SET(SYSTEM_LIBS
      )
else (${WIN32})
  # Linux and Apple Libraries
  SET(SYSTEM_LIBS
      m )
endif (${WIN32})

#--------------------------------------------------------
#                                                   graph
#--------------------------------------------------------
ADD_LIBRARY(graph SHARED 
   graph.cpp graph.cpp)
TARGET_LINK_LIBRARIES(graph
   ${SYSTEM_LIBS} )