#ifndef TVM_STRING
#define TVM_STRING

#include <string>
#include <algorithm>
#include <vector>

namespace tvm {

const char* const white_space = " \t\n\r\f\v";
const char* const comment_sign = ";";


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


inline std::vector<std::string> tokenize(const std::string& s, const char* delim)
{
  std::vector<std::string> res;
  std::size_t start, end = 0;

  while ((start = s.find_first_not_of(delim, end)) != std::string::npos) {
    end = s.find_first_of(delim, start);
    res.push_back(s.substr(start, end - start));
  }

  return res;
}


inline bool is_number(std::string_view s)
{
  if (s.front() == '-') {
    s.remove_prefix(1);
    if (s.empty())
      return false;
  }
  auto d = std::count_if(s.begin(), s.end(),
             [](unsigned char c){ return std::isdigit(c);});
  return d == s.length();
}


}
#endif
