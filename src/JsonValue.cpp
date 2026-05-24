#include "include/JsonValue.hpp"

namespace sjson {

JsonValue::JsonValue() = default;

JsonValue::JsonValue(JsonPrimitiveType primitive) : value_(primitive) {}

JsonValue::JsonValue(JsonObject object) : value_(object) {}

JsonValue::JsonValue(JsonArray array) : value_(array) {}

bool JsonValue::is_null() const noexcept {
  if (!is_primitive()) {
    return false;
  }
  return std::holds_alternative<std::monostate>(
      std::get<JsonPrimitiveType>(value_));
}

bool JsonValue::is_string() const noexcept {
  if (!is_primitive()) {
    return false;
  }
  return std::holds_alternative<std::string>(
      std::get<JsonPrimitiveType>(value_));
}

bool JsonValue::is_number() const noexcept {
  if (!is_primitive()) {
    return false;
  }
  auto &prim = std::get<JsonPrimitiveType>(value_);
  return std::holds_alternative<long long>(prim) ||
         std::holds_alternative<double>(prim);
}

bool JsonValue::is_boolean() const noexcept {
  if (!is_primitive()) {
    return false;
  }
  return std::holds_alternative<bool>(std::get<JsonPrimitiveType>(value_));
}

std::size_t JsonValue::size() const {
  if (is_array()) {
    return std::get<JsonArray>(value_).size();
  } else if (is_object()) {
    return std::get<JsonObject>(value_).size();
  } else {
    throw std::runtime_error("JsonValue is not an array or object");
  }
}

bool JsonValue::is_object() const noexcept {
  return std::holds_alternative<JsonObject>(value_);
}

bool JsonValue::is_array() const noexcept {
  return std::holds_alternative<JsonArray>(value_);
}

bool JsonValue::is_primitive() const noexcept {
  return std::holds_alternative<JsonPrimitiveType>(value_);
}

JsonValue &JsonValue::operator[](int index) {
  if (!is_array()) {
    throw std::runtime_error("JsonValue is not an array");
  }
  return std::get<JsonArray>(value_)[index];
}

const JsonValue &JsonValue::operator[](int index) const {
  if (!is_array()) {
    throw std::runtime_error("JsonValue is not an array");
  }
  return std::get<JsonArray>(value_)[index];
}

JsonValue &JsonValue::operator[](const char *key) {
  if (!is_object()) {
    throw std::runtime_error("JsonValue is not an object");
  }
  return std::get<JsonObject>(value_)[key];
}

const JsonValue &JsonValue::operator[](const char *key) const {
  if (!is_object()) {
    throw std::runtime_error("JsonValue is not an object");
  }
  auto &obj = std::get<JsonObject>(value_);
  auto it = obj.find(key);
  if (it == obj.end()) {
    throw std::runtime_error("Key not found in JsonObject:" +
                             std::string(key));
  }
  return it->second;
}

JsonValue &JsonValue::operator[](const std::string &key) {
  if (!is_object()) {
    throw std::runtime_error("JsonValue is not an object");
  }
  return std::get<JsonObject>(value_)[key];
}

const JsonValue &JsonValue::operator[](const std::string &key) const {
  if (!is_object()) {
    throw std::runtime_error("JsonValue is not an object");
  }
  auto &obj = std::get<JsonObject>(value_);
  auto it = obj.find(key);
  if (it == obj.end()) {
    throw std::runtime_error("Key not found in JsonObject: " + key);
  }
  return it->second;
}

JsonValue::operator JsonPrimitiveType() const {
  return std::get<JsonPrimitiveType>(value_);
}

JsonValue::operator JsonObject() const {
  return std::get<JsonObject>(value_);
}

JsonValue::operator JsonArray() const {
  return std::get<JsonArray>(value_);
}

JsonValue::operator bool() const {
  if (is_primitive()) {
    auto &prim = std::get<JsonPrimitiveType>(value_);
    if (std::holds_alternative<bool>(prim)) {
      return std::get<bool>(prim);
    }
  }
  throw std::runtime_error("JsonValue is not a boolean primitive");
}

bool operator==(const JsonValue &lhs, const std::string &rhs) {
  auto &prim = std::get<JsonPrimitiveType>(lhs.value_);
  return std::holds_alternative<std::string>(prim) &&
         std::get<std::string>(prim) == rhs;
}

bool operator==(const JsonValue &lhs, const char *rhs) {
  return lhs == std::string(rhs);
}

} // namespace sjson
