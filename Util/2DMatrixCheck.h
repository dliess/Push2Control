#ifndef UTIL_2D_MATRIX_CHECK_H
#define UTIL_2D_MATRIX_CHECK_H

#include <tuple>
#include <vector>

namespace util
{
template<typename T>
[[nodiscard]] std::pair<int, int> dimOf(
   const std::vector<std::vector<T>>& matrix2D) noexcept
{
   return std::make_pair(matrix2D.size(),
                         matrix2D.size() > 0 ? matrix2D[0].size() : 0);
}

template<typename T>
[[nodiscard]] bool isConsistent(
   const std::vector<std::vector<T>>& matrix2D) noexcept
{
   static constexpr int UNSET = -1;
   int colCnt{UNSET};
   for (const auto& row : matrix2D)
   {
      if (UNSET == colCnt)
      {
         colCnt = row.size();
      }
      else
      {
         if (colCnt != row.size())
         {
            return false;
         }
      }
   }
   return true;
}

} // namespace util

#endif