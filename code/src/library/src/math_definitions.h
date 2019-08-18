#ifndef INTEGRATION_LIBRARY_MATH_DEFINITIONS_H
#define INTEGRATION_LIBRARY_MATH_DEFINITIONS_H

#include <sstream>
#include <limits>

/**
 * Will check if a string is a number.
 * @tparam Numeric Template to check for.
 * @param s String that should be checked.
 * @return Return true is a numeric is present and false otherwise.
 */
template<typename Numeric>
bool is_number(const std::string& s)
{
  Numeric n;
  return((std::istringstream(s) >> n >> std::ws).eof());
}

template<typename Numeric>
Numeric to_number(const std::string& s)
{
  Numeric n;
  std::istringstream stream(s);
  stream >> n;
  return n;
}

#endif //INTEGRATION_LIBRARY_MATH_DEFINITIONS_H
