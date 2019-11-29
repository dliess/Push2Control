#include <gtest/gtest.h>
#include "Overload.h"
#include "mpark/variant.hpp"

class Base {};
class A : public Base {};
class B : public Base {};
class C {};

TEST(OverloadTest, Test) {
    mpark::variant<A, B, C> varA;
    mpark::visit(util::overload{
        [](const Base&){ GTEST_FAIL(); },
        [](const A&){ GTEST_SUCCEED(); },
        [](const B&){ GTEST_FAIL(); },
        [](const C&){ GTEST_FAIL(); }
    }, varA);

    mpark::visit(util::overload{
        [](const Base&){ GTEST_SUCCEED(); },
        [](const C&){ GTEST_FAIL(); }
    }, varA);

    mpark::visit(util::overload{
        [](const Base&){ GTEST_FAIL(); },
        [](const auto&){ GTEST_SUCCEED(); }
    }, varA);

    mpark::visit(util::overload{
        [](const A&){ GTEST_SUCCEED(); },
        [](const auto&){ GTEST_FAIL(); }
    }, varA);

    mpark::variant<A, B, C> varC;
    varC.emplace<C>();
    mpark::visit(util::overload{
        [](const A&){ GTEST_FAIL(); },
        [](const auto&){ GTEST_SUCCEED(); }
    }, varC);


}