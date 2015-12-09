/**
 * @author Mike Bogochow
 * @version 2.4.0, Dec 8, 2015
 *
 * @file Bidder.h
 *
 * Bidder class
 */

#ifndef MBOGO_BIDDER_H_
#define MBOGO_BIDDER_H_

#include "MOOS/libMOOS/App/MOOSApp.h"

class Bidder : public CMOOSApp {
  bool OnNewMail (MOOSMSG_LIST & Mail);
  bool OnConnectToServer ();
  bool Iterate ();
};

#endif /* MBOGO_BIDDER_H_ */
