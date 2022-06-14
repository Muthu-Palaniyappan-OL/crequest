#include "util.hpp"

#include <regex>
#include <string>
using namespace std;

size_t hash_string(string input) {
  hash<string> h;
  size_t hash_result = h(input);
  return hash_result;
}

/*
string binary_hash(string input) {
  string binary;
  hash<string> h;
  size_t hash_result = h(input);
  size_t size = sizeof(size_t);
  for (int i = 0; i < size * 8; ++i) {
    binary.push_back(char(48 + ((hash_result >> size - i - 1) & 1)));
  }
  binary.shrink_to_fit();
  return binary;
}
*/

void read_json_input() {
  string s;
  ifstream file;
  file.open("/etc/crequest/config.json");
  string buffer;
  if (file.is_open())
    while (!file.eof()) {
      getline(file, buffer);
      s += buffer;
    }

  auto p = json::parse(s);
}