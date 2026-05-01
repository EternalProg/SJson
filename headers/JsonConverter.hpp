//
// Created by eternal on 01.02.25.
//

#ifndef SJSON_HEADERS_JSONCONVERTER_HPP_
#define SJSON_HEADERS_JSONCONVERTER_HPP_

#include <string>
#include "JsonValue.hpp"
#include <iostream>

namespace sjson {
class JsonValue;

class JsonConverter {
 public:
  // return a JsonValue object from json string
  static JsonValue deserialize(const std::string &t_json_str) noexcept;

  // return json string from the object of class T
  template<typename T>
  static std::string serialize(const T &t_obj) noexcept;

  static void parseObject(std::string_view t_json_str, JsonValue &t_res_obj) noexcept;
  static void parseArray(std::string_view t_json_str, JsonValue &t_res_obj) noexcept;
  static void parsePrimitiveValue(std::string_view t_json_str, JsonValue &t_res_obj) noexcept;
// private:
  using cstr_iterator = std::string::const_iterator;
  static cstr_iterator deserialize_helper(cstr_iterator it, JsonValue &t_res_obj) noexcept;

  static bool isNumber(std::string_view t_json_str) noexcept {
    return t_json_str.find_first_not_of("0123456789") == std::string::npos;
  }

  static bool isFloatNumber(std::string_view t_json_str) noexcept {
    return t_json_str.find_first_not_of("0123456789.") == std::string::npos;
  }

  static std::pair<std::string, std::string> retrieveKeyValuePair(std::string_view t_json_str) noexcept;

  static cstr_iterator skipWhitespaces(std::string_view t_json_str, cstr_iterator it) noexcept {
    while (*it == ' ') {
      ++it;
    }
    return it;
  }
};

}
#endif //SJSON_HEADERS_JSONCONVERTER_HPP_
