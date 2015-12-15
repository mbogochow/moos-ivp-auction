/**
 * @author Mike Bogochow
 * @version 2.4.0, Dec 8, 2015
 *
 * @file Auctioneer.cpp
 *
 * Auctioneer class implementation
 */

#include "AuctionMOOSApp.h"

AuctionMOOSApp::AuctionMOOSApp(void)
{
  m_iterations = 0;
}

AuctionMOOSApp::~AuctionMOOSApp(void)
{

}

bool
AuctionMOOSApp::Iterate(void)
{
  m_iterations += 1;
  dp.dprintf(LVL_MID_VERB, "Iterate() #%u\n", m_iterations);

  return true;
}

bool
AuctionMOOSApp::OnNewMail(MOOSMSG_LIST &NewMail)
{
  dp.dprintf(LVL_MAX_VERB, "OnNewMail()\n");
  return true;
}


bool
AuctionMOOSApp::OnConnectToServer(void)
{
  dp.dprintf(LVL_MAX_VERB, "OnConnectToServer()\n");
  return true;
}

bool
AuctionMOOSApp::OnStartUp(void)
{
  dp.dprintf(LVL_MAX_VERB, "OnStartup()\n");
  return true;
}
