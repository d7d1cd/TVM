#ifndef TVM_STRING
#define TVM_STRING

#include <string>
#include <algorithm>

namespace tvm {

const char* white_space = " \t\n\r\f\v";
const char* comment_sign = ";";


inline std::string& trim_comments(std::string& s, const char* c = comment_sign)
{
  if (auto i = s.find_first_of(c); i != s.npos)
	s.erase(i);
  return s;
}


inline std::string& right_trim(std::string& s, const char* t = white_space)
{
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}


inline std::string& left_trim(std::string& s, const char* t = white_space)
{
  s.erase(0, s.find_first_not_of(t));
  return s;
}


inline std::string& trim(std::string& s, const char* t = white_space)
{
  return left_trim(right_trim(s, t), t);
}


inline std::string& upper_case(std::string& s)
{
  std::transform(s.begin(), s.end(), s.begin(), toupper);
  return s;
}


}
#endif
