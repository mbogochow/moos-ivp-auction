#--------------------------------------------------------
# The CMakeLists.txt for:                     pAuctioneer
# Author(s):                                Mike Bogochow
#--------------------------------------------------------

# Set System Specific Libraries
if (${WIN32})
  # Windows Libraries
  SET(SYSTEM_LIBS
     wsock32 )
else (${WIN32})
  # Linux and Apple Libraries
  SET(SYSTEM_LIBS
      m
      pthread )
endif (${WIN32})


SET(SRC
   main.cpp
   Auctioneer.cpp
   Auctioneer_Info.cpp
)

ADD_EXECUTABLE(pAuctioneer ${SRC})

TARGET_LINK_LIBRARIES(pAuctioneer
   ${MOOS_LIBRARIES}
   Graphs
   Auction
   mbutil
   ${SYSTEM_LIBS} )
