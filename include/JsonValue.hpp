#ifndef ETERNAL_JSON_VALUE_H
#define ETERNAL_JSON_VALUE_H

#include <cstddef>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace sjson {

class JsonValue;

using JsonObject = std::unordered_map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;

using JsonPrimitiveType =
    std::variant<std::monostate, long long, double, bool, std::string>;

class JsonValue {
public:
  JsonValue();
  JsonValue(JsonPrimitiveType primitive);
  JsonValue(JsonObject object);
  JsonValue(JsonArray array);

  bool is_object() const noexcept;
  bool is_array() const noexcept;
  bool is_primitive() const noexcept;

  JsonValue &operator[](int index);
  const JsonValue &operator[](int index) const;

  JsonValue &operator[](const char *key);
  const JsonValue &operator[](const char *key) const;

  JsonValue &operator[](const std::string &key);
  const JsonValue &operator[](const std::string &key) const;

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
  friend bool operator==(const JsonValue &lhs, T rhs) {
    auto &prim = std::get<JsonPrimitiveType>(lhs.value_);
    if constexpr (std::is_floating_point_v<T>) {
      return std::holds_alternative<double>(prim) &&
             std::get<double>(prim) == rhs;
    } else if constexpr (std::is_same_v<T, bool>) {
      return std::holds_alternative<bool>(prim) && std::get<bool>(prim) == rhs;
    } else {
      return std::holds_alternative<long long>(prim) &&
             std::get<long long>(prim) == rhs;
    }
  }
  friend bool operator==(const JsonValue &lhs, const std::string &rhs);
  friend bool operator==(const JsonValue &lhs, const char *rhs);

  operator JsonPrimitiveType() const;
  operator JsonObject() const;
  operator JsonArray() const;

  explicit operator bool() const;

private:
  std::variant<JsonPrimitiveType, JsonObject, JsonArray> value_;
};

} // namespace sjson
#endif // ETERNAL_JSON_VALUE_H
