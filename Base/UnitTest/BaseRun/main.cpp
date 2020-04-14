#include <loguru.hpp>

#include "Base.h"

int main(int argc, char* argv[])
{
   loguru::init(argc, argv);
   try
   {
      base::Base base;
      base.start();
      base.waitForEnd();
   }
   catch (std::exception& e)
   {
      LOG_F(ERROR, "Exception caught: {}", e.what());
   }
   return 0;
}