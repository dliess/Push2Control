#include <docopt.h>

#include <loguru.hpp>
#include <map>

#include "Base.h"

static const char USAGE[] = R"(
   Usage:
      BaseRun [CONFIGDIR]
)";

int main(int argc, char* argv[])
{
   loguru::init(argc, argv);

   std::map<std::string, docopt::value> args =
      docopt::docopt(USAGE, {argv + 1, argv + argc},
                     true,           // show help if requested
                     "BaseRun 0.1"); // version string

   const std::string configRoot(args["CONFIGDIR"] ? args["CONFIGDIR"].asString() : "");

   try
   {
      base::Base base(configRoot);
      base.start();
      base.waitForEnd();
   }
   catch (std::exception& e)
   {
      LOG_F(ERROR, "Exception caught: {}", e.what());
   }
   return 0;
}