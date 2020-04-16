#include <gtest/gtest.h>
#include <string>
#include <iostream>

#include "mpark/variant.hpp"
#include "Meta.h"
#include "JsonCast.h"

struct Person
{
   Person() = default;
   Person(std::string name, int age) : name(std::move(name)), age(age) {}
   bool operator==(const Person& rhs) const { return name == rhs.name && age == rhs.age; }
   std::string        name;
   int                age;
};

struct BreakDancer : public Person
{
   BreakDancer() = default;
   BreakDancer(std::string name, int age, int numHeadspins) : Person(std::move(name), age), numHeadspins(numHeadspins) {}
   bool operator==(const BreakDancer& rhs) const { return Person::operator==(rhs) && numHeadspins == rhs.numHeadspins; }
   int numHeadspins;
};

struct PopLocker : public Person
{
   PopLocker() = default;
   PopLocker(std::string name, int age, int locksPerSecond) : Person(std::move(name), age), locksPerSecond(locksPerSecond) {}
   bool operator==(const PopLocker& rhs) const { return Person::operator==(rhs) && locksPerSecond == rhs.locksPerSecond; }
   unsigned locksPerSecond;
};

struct PopLocker2 : public Person
{
   PopLocker2() = default;
   PopLocker2(std::string name, int age, int locksPerSecond) : Person(std::move(name), age), locksPerSecond(locksPerSecond) {}
   bool operator==(const PopLocker& rhs) const { return Person::operator==(rhs) && locksPerSecond == rhs.locksPerSecond; }
   unsigned locksPerSecond;
};


using PopLocker12 = mpark::variant<PopLocker, PopLocker2>;

struct MainData
{
   std::vector<std::vector<PopLocker12>> data;
};

using Dancer = mpark::variant<BreakDancer, PopLocker>;

namespace meta
{

template <>
inline auto registerMembers<BreakDancer>()
{
   return members(
      member("name", &BreakDancer::name),
      member("age", &BreakDancer::age),
      member("numHeadspins", &BreakDancer::numHeadspins)
   );
}

template <>
inline auto registerMembers<PopLocker>()
{
   return members(
      member("name", &PopLocker::name),
      member("age", &PopLocker::age),
      member("locksPerSecond", &PopLocker::locksPerSecond)
   );
}

template <>
inline auto registerMembers<PopLocker2>()
{
   return members(
      member("name", &PopLocker2::name),
      member("age", &PopLocker2::age),
      member("locksPerSecond", &PopLocker2::locksPerSecond)
   );
}

template <>
inline auto registerMembers<MainData>()
{
   return members(
      member("data", &MainData::data)
   );
}

template <>
auto getClassNameOrIndex<Person>(int i) noexcept { return "Person"; }

template <>
auto getClassNameOrIndex<BreakDancer>(int i) noexcept { return "BreakDancer"; }

template <>
auto getClassNameOrIndex<PopLocker>(int i) noexcept { return "PopLocker"; }

template <>
auto getClassNameOrIndex<PopLocker2>(int i) noexcept { return "PopLocker2"; }


} // namespace meta

//- -- This is just needed for the lambda overload collection
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;
//-----------------------------------------------------------

TEST(JsonParseVariantsTest, VariantToJson) {
   Dancer dancer;
   dancer.emplace<PopLocker>("MrWiggles", 20, 20);
   nlohmann::json jDancer(dancer);
   Dancer dancerReadBack;
   //std::cout << jDancer.dump() << std::endl;
   jDancer.get_to(dancerReadBack);
   ASSERT_EQ(dancerReadBack, dancer);
}

TEST(JsonParseVariantsTest, VectorOfVariantToJson) {
   std::vector<Dancer> dancers;
   dancers.emplace_back(BreakDancer("Darkness", 20, 20));
   dancers.emplace_back(PopLocker("MrT", 40, 20000));
   dancers.emplace_back(BreakDancer("BBoyBenny", 30, -123));

   nlohmann::json jDancers(dancers);
   std::cout << jDancers.dump(1) << std::endl;

   std::vector<Dancer> dancersReadBack;
   dancersReadBack = jDancers.get<std::vector<Dancer>>();
   ASSERT_EQ(3, dancersReadBack.size());
   ASSERT_EQ(0, dancersReadBack[0].index());
   ASSERT_EQ(1, dancersReadBack[1].index());
   ASSERT_EQ(0, dancersReadBack[2].index());

   #if 0
   for(auto& folks : dancers)
   {
      mpark::visit(overload{
         [](const BreakDancer& obj) { std::cout << obj.name << " numHeadspins " << obj.numHeadspins << std::endl; },
         [](const PopLocker& obj) { std::cout << obj.name << " locksPerSecond " << obj.locksPerSecond << std::endl; }
         /*, TODO: somehow always this gets activated even when one of the above would match
         [](auto&& other) { std::cerr << "INTERNAL ERROR, who is that?" << std::endl; }*/
      }, folks);
   }

   for(auto& folks : dancersReadBack)
   {
      mpark::visit(overload{
         [](const BreakDancer& obj) { std::cout << obj.name << " numHeadspins " << obj.numHeadspins << std::endl; },
         [](const PopLocker& obj) { std::cout << obj.name << " locksPerSecond " << obj.locksPerSecond << std::endl; }
         /*, TODO: somehow always this gets activated even when one of the above would match
         [](auto&& other) { std::cerr << "INTERNAL ERROR, who is that?" << std::endl; }*/
      }, folks);
   }
   #endif
}

TEST(JsonParseVariantsTest, NoMatchForVariant) {
   auto jMatchingContent = nlohmann::json::parse("{\"age\":20,\"locksPerSecond\":20,\"name\":\"MrWiggles\"}");
   auto jNonMatchingContent = nlohmann::json::parse("{\"iceage\":20,\"locksPerSecond\":20,\"name\":\"MrWiggles\"}");
   ASSERT_NO_THROW(auto popLocker = jMatchingContent.get<PopLocker>());
   ASSERT_NO_THROW(auto dancer =    jMatchingContent.get<Dancer>());
   ASSERT_THROW(auto popLocker =    jNonMatchingContent.get<PopLocker>(), std::runtime_error);
   ASSERT_THROW(auto dancer =       jNonMatchingContent.get<Dancer>(), std::runtime_error);
}

TEST(JsonParseVariantsTest, TestSubsetOrder1) {
   using ClassAndBaseClass = mpark::variant<BreakDancer, Person>;
   ClassAndBaseClass dancer;
   dancer.emplace<BreakDancer>("BboyBenny", 20, 300);
   nlohmann::json jDancer(dancer);
   ClassAndBaseClass dancerReadBack;
   jDancer.get_to(dancerReadBack);
   ASSERT_EQ(dancerReadBack, dancer);
}

TEST(JsonParseVariantsTest, TestSubsetOrder2) {
   using ClassAndSubClass = mpark::variant<Person, BreakDancer>;
   ClassAndSubClass dancer;
   dancer.emplace<BreakDancer>("BboyBenny", 20, 300);
   nlohmann::json jDancer(dancer);
   ClassAndSubClass dancerReadBack;
   jDancer.get_to(dancerReadBack);
   ASSERT_EQ(dancerReadBack, dancer);
}

TEST(JsonParseVariantsTest, TestType) {
   const std::string_view jsonStr = R"(
      {
         "data" :
         [ [ {".type" : "PopLocker", "age" : 20 , "locksPerSecond" : 20, "name" : "MrWiggles"},
             {".type" : "PopLocker2", "age" : 5 , "locksPerSecond" : 5, "name" : "BabyWiggles"}
           ] ]
      }
   )";

   nlohmann::json j;
   MainData mainData;
   ASSERT_NO_THROW( j = nlohmann::json::parse(jsonStr) );
   ASSERT_NO_THROW( mainData = j.get<MainData>() );
   ASSERT_EQ( 1, mainData.data.size() );
   ASSERT_EQ( 2, mainData.data[0].size() );
   const auto& var1 = mainData.data[0][0];
   const auto& var2 = mainData.data[0][1];
   ASSERT_TRUE( mpark::holds_alternative<PopLocker>(var1) );
   ASSERT_TRUE( mpark::holds_alternative<PopLocker2>(var2) );
}

TEST(JsonParseVariantsTest, BadType) {
   const std::string_view jsonStr = R"(
      {
         "data" :
         [ [ {".type" : "PopLocker", "age" : 20 , "locksPerSecond" : 20, "name" : "MrWiggles"},
             {".type" : "PopLocker2", "age" : 5 , "locksPerSecond" : 5, "name" : "BabyWiggles"},
             {".type" : "MessedUpType", "age" : 50 , "locksPerSecond" : 5, "name" : "OldWiggles"}
           ] ]
      }
   )";

   nlohmann::json j;
   MainData mainData;
   ASSERT_NO_THROW( j = nlohmann::json::parse(jsonStr) );
   ASSERT_THROW( mainData = j.get<MainData>(), std::runtime_error );
}