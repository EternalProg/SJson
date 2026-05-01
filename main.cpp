#include "headers/JsonValue.hpp"
#include "headers/JsonConverter.hpp"
#include <iostream>

#include <string>

int main() {
  std::string json = "\"name\": \"pudge\"";
  auto pair = sjson::JsonConverter::retrieveKeyValuePair(json);
  std::unordered_map<std::string, std::string> mp;
  mp[pair.first] = pair.second;
  std::cout << "key: " << pair.first << "\nvalue :" << pair.second << '\n'
    << mp[pair.first] << '\n';
  /*
  JsonValue onvalue1 = JsonValue::parse(json);
  std::cout << jsonvalue1["name"] << jsonvalue1["age"] << jsonvalue1["salary"]
            << '\n';
            */
  return 0;
}


