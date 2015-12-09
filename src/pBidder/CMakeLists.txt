#--------------------------------------------------------
# The CMakeLists.txt for:                         pBidder
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
   Bidder.cpp
)

ADD_EXECUTABLE(pBidder ${SRC})

TARGET_LINK_LIBRARIES(pBidder
   ${MOOS_LIBRARIES}
   ${SYSTEM_LIBS} )