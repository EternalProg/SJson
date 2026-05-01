#ifndef ETERNAL_JSON_VALUE_H
#define ETERNAL_JSON_VALUE_H

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace sjson {

// represent the json values
// struct Number;
struct JsonPrimitiveType;
struct JsonValue;
class JsonConverter;

using JsonObject = std::map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;
// using JsonVariant = std::variant<std::nullptr_t, bool, Number,
// std::string, JsonObject, JsonArray>;
using JsonVariant = std::variant<JsonPrimitiveType, JsonObject, JsonArray>;

enum class JsonToken : uint8_t { Null, Number, Boolean, String, Object, Array };

struct JsonPrimitiveType {
  std::variant<long long, double, std::nullptr_t, bool, std::string> value;

  JsonPrimitiveType() = default;
  explicit JsonPrimitiveType(long long val) : value(val) {}
  explicit JsonPrimitiveType(double val) : value(val) {}
  explicit JsonPrimitiveType(bool val) : value(val) {}
  explicit JsonPrimitiveType(std::string val) : value(val) {}
  explicit JsonPrimitiveType(std::nullptr_t) : value(nullptr) {}
};

struct Number {
  std::variant<long long, double> value;

  Number() = default;
  explicit Number(long long val) : value(val) {}
  explicit Number(double val) : value(val) {}
};

class JsonValue {
  friend class JsonConverter;

public:
  JsonValue()
      : m_values(
            std::make_shared<std::unordered_map<std::string, JsonVariant>>()) {}
  /*JsonValue(const std::string &t_json_str) :
  m_values(std::make_shared<std::unordered_map<std::string, JsonVariant>>()) {
    JsonValue(JsonConverter::deserialize(t_json_str));
  }*/

private:
  std::shared_ptr<std::unordered_map<std::string, JsonVariant>> m_values;
};

} // namespace sjson
#endif /* ifndef ETERNAL_JSON_VALUE_H */
