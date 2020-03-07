#ifndef BUSINESS_LOGIC_H
#define BUSINESS_LOGIC_H

#include <vector>
#include <string>

namespace logic
{

struct Parameter
{
   std::string name;
   int         value;
};

struct Car
{
   std::string name;
   std::vector<Parameter> parameter;
};

struct Cars
{
   std::vector<Car> data;
};

} // namespace logic

#endif