//
// Created by eternal on 01.02.25.
//
#include "include/JsonDeserializer.hpp"
#include <cctype>
#include <unordered_map>
#include <variant>
#include <vector>

namespace sjson {

/*
{
  "Image": {
      "Width":  800,
      "Height": 600,
      "Title":  "View from 15th Floor",
      "Thumbnail": {
          "Url":    "http://www.example.com/image/481989943",
          "Height": 125,
          "Width":  100
      },
      "Animated" : false,
      "IDs": [116, 943, 234, 38793]
    }
}
*/

namespace {

char next(std::string_view sv, std::size_t &pos) {
  while (std::isspace(sv[pos])) {
    ++pos;
  }
  return sv[pos];
}

bool is_object(std::string_view json_sv, std::size_t &pos) {
  char ch = next(json_sv, pos);
  return ch == '{';
}

bool is_array(std::string_view json_sv, std::size_t &pos) {
  char ch = next(json_sv, pos);
  return ch == '[';
}

bool is_primitive(std::string_view json_sv, std::size_t &pos) {
  char ch = next(json_sv, pos);
  return ch != '{' && ch != '[';
}

JsonValue deserialize_value(std::string_view json_sv, std::size_t &pos);

// "key": value
JsonValue deserialize_object(std::string_view json_sv, std::size_t &pos) {
  JsonObject obj;
  ++pos; // skip opening '{'

  for (char ch = next(json_sv, pos); ch != '}'; ch = next(json_sv, pos)) {
    auto key_start = json_sv.find('\"', pos);
    auto key_end = json_sv.find('\"', key_start + 1);
    std::string key =
        std::string(json_sv.substr(key_start + 1, key_end - key_start - 1));

    auto colomn = json_sv.find(':', key_end);
    pos = colomn + 1;

    obj[key] = deserialize_value(json_sv, pos);

    if (json_sv[pos] == ',')
      ++pos; // skip comma
  }

  ++pos; // skip closing '}'
  return obj;
}

// [value, value]
JsonArray deserialize_array(std::string_view json_sv, std::size_t &pos) {
  JsonArray array;
  ++pos; // skip opening '['

  for (char ch = next(json_sv, pos); ch != ']'; ch = next(json_sv, pos)) {
    array.push_back(deserialize_value(json_sv, pos));

    if (json_sv[pos] == ',')
      ++pos; // skip comma
  }

  ++pos; // skip closing ']'
  return array;
}

// primitive: string, number, boolean, null
JsonPrimitiveType deserialize_primitive(std::string_view json_sv,
                                        std::size_t &pos) {
  JsonPrimitiveType value;
  char ch = next(json_sv, pos);
  if (ch == '\"') {
    auto value_end = json_sv.find('\"', pos + 1);
    value = std::string(json_sv.substr(pos + 1, value_end - pos - 1));
    pos = value_end + 1;
  } else if (std::isdigit(ch) || ch == '-') {
    auto value_end = json_sv.find_first_not_of("-0123456789", pos);
    // floating point number
    if (json_sv[value_end] == '.' || json_sv[value_end] == 'e' ||
        json_sv[value_end] == 'E') {
      value_end = json_sv.find_first_not_of("0123456789.eE", pos);
      value = std::stod(std::string(json_sv.substr(pos, value_end - pos)));
    } else /* integer */ {
      value = std::stoll(std::string(json_sv.substr(pos, value_end - pos)));
    }
    pos = value_end;
  } else if (ch == 't' || ch == 'f') {
    value = (ch == 't');
    pos += (ch == 't') ? 4 : 5; // skip "true" or "false"
  } else if (ch == 'n') {
    value = std::monostate();
    pos += 4; // skip "null"
  }

  return value;
}

JsonValue deserialize_value(std::string_view json_sv, std::size_t &pos) {
  if (is_object(json_sv, pos)) {
    return JsonValue(deserialize_object(json_sv, pos));
  } else if (is_array(json_sv, pos)) {
    return JsonValue(deserialize_array(json_sv, pos));
  } else /* primitive */ {
    return JsonValue(deserialize_primitive(json_sv, pos));
  }
}

}; // namespace

JsonValue deserialize(const std::string &json_str) noexcept {
  if (json_str.empty()) {
    // If the input string is empty, we can consider it as a null JSON value.
    // TODO: verify in specs
    return JsonValue(JsonPrimitiveType(std::monostate()));
  }

  std::size_t pos = 0;
  return deserialize_value(json_str, pos);
}

std::string serialize(const JsonValue &json_value) noexcept {}

} // namespace sjson
