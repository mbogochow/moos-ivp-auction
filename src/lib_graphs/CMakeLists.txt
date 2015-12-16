#--------------------------------------------------------
# The CMakeLists.txt for:                      lib_graphs
# Author(s):                                Mike Bogochow
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
#                                                  Graphs
#--------------------------------------------------------
ADD_LIBRARY(Graphs SHARED 
   Graph.cpp
   Subgraph.cpp
   SpanningTree.cpp
   Path.cpp)
TARGET_LINK_LIBRARIES(Graphs
#   helmivp
#   behaviors 
#   ivpbuild 
#   logic 
#   ivpcore 
#   bhvutil 
#   mbutil 
#   geometry 
   ${SYSTEM_LIBS} )