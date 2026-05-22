//
// Created by eternal on 01.02.25.
//

#ifndef SJSON_HEADERS_JSONCONVERTER_HPP_
#define SJSON_HEADERS_JSONCONVERTER_HPP_

#include <include/JsonValue.hpp>
#include <string>

namespace sjson {

JsonValue deserialize(const std::string &json_str) noexcept;
std::string serialize(const JsonValue &json_value) noexcept;


} // namespace sjson
#endif // SJSON_HEADERS_JSONCONVERTER_HPP_
