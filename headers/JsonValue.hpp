#ifndef ETERNAL_JSON_VALUE_H
#define ETERNAL_JSON_VALUE_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace sjson {

class JsonValue {
private:
  using JsonObject = std::unordered_map<std::string, JsonValue>;
  using JsonArray = std::vector<JsonValue>;

  struct JsonPrimitiveType {
    // monostate represents null value
    std::variant<std::monostate, long long, double, bool, std::string> value;

    JsonPrimitiveType() = default;
    explicit JsonPrimitiveType(long long val) : value(val) {}
    explicit JsonPrimitiveType(double val) : value(val) {}
    explicit JsonPrimitiveType(bool val) : value(val) {}
    explicit JsonPrimitiveType(std::string val) : value(val) {}
    explicit JsonPrimitiveType(std::monostate) : value() {}
  };

public:
  std::variant<JsonPrimitiveType, JsonObject, JsonArray> value;
};

} // namespace sjson
#endif /* ifndef ETERNAL_JSON_VALUE_H */
