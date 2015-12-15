/**
 * @author Mike Bogochow
 * @version 2.4.0, Dec 8, 2015
 *
 * @file Auctioneer.h
 *
 * Auctioneer class
 */

#ifndef MBOGO_AUCTIONMOOSAPP_H_
#define MBOGO_AUCTIONMOOSAPP_H_

#include "MOOS/libMOOS/App/MOOSApp.h"

#include "../lib_auction/AuctionDefs.h"
#include "../lib_auction/DebugPrinter.h"

#include <boost/lexical_cast.hpp>

#include <string>

class AuctionMOOSApp : public virtual CMOOSApp
{
public:
  AuctionMOOSApp(void);
  ~AuctionMOOSApp(void);

  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool OnStartUp(void);
  bool Iterate(void);
  bool OnConnectToServer(void);

protected:
  unsigned long int m_iterations;
  DebugPrinter dp;

  template<typename T>
  bool doNotify(std::string key, T val)
  {
    bool ret;
    std::string sval = boost::lexical_cast<std::string>(val);
    dp.dprintf(LVL_MID_VERB, "Writing %s=%s\n to MOOSDB\n", key.c_str(),
        sval.c_str());
    if (!(ret = Notify(key, sval)))
    {
      MOOSTrace("ERROR: Failed to write %s=%s to MOOSDB\n", key.c_str(),
          sval.c_str());
    }
    return ret;
  }
};

#endif /* MBOGO_AUCTIONMOOSAPP_H_ */
