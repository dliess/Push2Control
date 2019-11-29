#ifndef UTIL_OVERLOAD_H
#define UTIL_OVERLOAD_H

namespace util
{

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

} // namespace util

#endif