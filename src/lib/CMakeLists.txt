#--------------------------------------------------------
# The CMakeLists.txt for:              lib
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
   graph.cpp)
ADD_LIBRARY(heap SHARED 
   heap.cpp)
    
TARGET_LINK_LIBRARIES(graph
   heap
   ${SYSTEM_LIBS} )