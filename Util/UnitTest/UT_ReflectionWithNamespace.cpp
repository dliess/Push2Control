#include <gtest/gtest.h>

#include <string>
#include "Meta.h"

#include "JsonCast.h"

struct TestStruct
{
   int a;
   float b;
   std::string c;
};

namespace base
{

struct TestStruct
{
   int d;
   float e;
   std::string f;
};

struct EmptyStruct
{
   int a;
   int b;
};

#include "JsonCastNamespaceFix.h"

} // namespace base

namespace meta
{

template<>
inline auto registerMembers<TestStruct>()
{
   return members(member("a", &TestStruct::a),
                  member("b", &TestStruct::b),
                  member("c", &TestStruct::c));
}

template<>
inline auto registerMembers<base::TestStruct>()
{
   return members(member("d", &base::TestStruct::d),
                  member("e", &base::TestStruct::e),
                  member("f", &base::TestStruct::f));
}

template<>
inline auto registerMembers<base::EmptyStruct>()
{
   return members();
   //return members(member("a", &base::EmptyStruct::a));
}

} // namespace meta


TEST(ReflectedToJsonTest, ReflectedToJson) {

   ASSERT_TRUE( meta::isRegistered<TestStruct>() );
   ASSERT_TRUE( meta::isRegistered<base::TestStruct>() );

   int i = 0;
   meta::doForAllMembers<TestStruct>([&i](const auto& member){
      switch(i++)
      {
         case 0: ASSERT_EQ("a", std::string(member.getName())); break;
         case 1: ASSERT_EQ("b", std::string(member.getName())); break;
         case 2: ASSERT_EQ("c", std::string(member.getName())); break;
      }
   });
   i = 0;
   meta::doForAllMembers<base::TestStruct>([&i](const auto& member){
      switch(i++)
      {
         case 0: ASSERT_EQ("d", std::string(member.getName())); break;
         case 1: ASSERT_EQ("e", std::string(member.getName())); break;
         case 2: ASSERT_EQ("f", std::string(member.getName())); break;
      }
   });

   {
      TestStruct t{3, 4.5, "Hello Test"};
      nlohmann::json j;
      nlohmann::to_json(j, t);
      ASSERT_EQ(
R"({
   "a": 3,
   "b": 4.5,
   "c": "Hello Test"
})", j.dump(3) );
   }
   {
      base::TestStruct t{3, 4.5, "Hello Test"};
      nlohmann::json j;
      nlohmann::to_json(j, t);
      ASSERT_EQ(
R"({
   "d": 3,
   "e": 4.5,
   "f": "Hello Test"
})", j.dump(3) );
   }

}

TEST(ReflectedToJsonTest, EmptyStruct) {
   base::EmptyStruct e;
   //ASSERT_TRUE( meta::isRegistered<base::EmptyStruct>() );


   /*
   nlohmann::json j;
   //TODO
   nlohmann::to_json(j, e);
      ASSERT_EQ(
R"({
   "d": 3,
   "e": 4.5,
   "f": "Hello Test"
})", j.dump(3) );*/
}