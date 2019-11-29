#include "fmt/format.h"
#include "fmt/printf.h" // fmt::printf
#include <string>

struct date {
  int year, month, day;
};

template <>
struct fmt::formatter<date> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const date &d, FormatContext &ctx) {
    return format_to(ctx.out(), "{}-{}-{}", d.year, d.month, d.day);
  }
};

int main()
{
    fmt::print("Hello, {}!\n", "world");  // Python-like format string syntax
    fmt::printf("Hello, %s!\n", "world"); // printf format string syntax

    std::string s = fmt::format("I'd rather be {1} than {0}.", "right", "happy");
    fmt::print("{}\n", s);

    fmt::memory_buffer buf;
    format_to(buf, "{}", 42);    // replaces itoa(42, buffer, 10)
    format_to(buf, "{:x}", 42);  // replaces itoa(42, buffer, 16)
    // access the string with to_string(buf) or buf.data()
    fmt::print("{}\n", buf.data());
    fmt::print("{}\n", to_string(buf));

    fmt::print("The date is {}\n", date{2012, 12, 9});

    return 0;
}