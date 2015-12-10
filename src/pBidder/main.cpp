/**
 * @author Mike Bogochow
 * @version 2.4.0, Dec 8, 2015
 *
 * @file main.cpp
 *
 * pBidder main
 */

#include "Bidder.h"
#include "../lib_auction/Info.h"

#include "MBUtils.h"
#include "ColorParse.h"

#include <string>

int main(int argc, char **argv)
{
  int ret = EXIT_SUCCESS;

  std::string mission_file;
  std::string run_command = argv[0];

  for(int i=1; i<argc; i++) {
    std::string argi = argv[i];
    if((argi=="-v") || (argi=="--version") || (argi=="-version"))
      showReleaseInfoAndExit();
    else if((argi=="-e") || (argi=="--example") || (argi=="-example"))
      showExampleConfigAndExit();
    else if((argi == "-h") || (argi == "--help") || (argi=="-help"))
      showHelpAndExit();
    else if((argi == "-i") || (argi == "--interface"))
      showInterfaceAndExit();
    else if(strEnds(argi, ".moos") || strEnds(argi, ".moos++"))
      mission_file = argv[i];
    else if(strBegins(argi, "--alias="))
      run_command = argi.substr(8);
    else if(i==2)
      run_command = argi;
  }

  if(mission_file == "")
    showHelpAndExit();

  std::cout << termColor("green");
  std::cout << "pBidder launching as " << run_command << std::endl;
  std::cout << termColor() << std::endl;

  Bidder bidder;

  bidder.Run(run_command.c_str(), mission_file.c_str());

  return ret;
}
