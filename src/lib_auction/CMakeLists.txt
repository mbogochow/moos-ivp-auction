#--------------------------------------------------------
# The CMakeLists.txt for:                     lib_auction
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
#                                                 Auction
#--------------------------------------------------------
ADD_LIBRARY(Auction SHARED 
   AuctionDefs.cpp
   DebugPrinter.cpp
   AuctionMOOSApp.cpp
   time_util.cpp)
TARGET_LINK_LIBRARIES(Auction
#   helmivp
#   behaviors 
#   ivpbuild 
#   logic 
#   ivpcore 
#   bhvutil 
#   mbutil 
#   geometry 
   ${SYSTEM_LIBS} )