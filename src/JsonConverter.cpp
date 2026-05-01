//
// Created by eternal on 01.02.25.
//
#include "JsonConverter.hpp"
namespace sjson {

// TODO -- РЕалізувати deserialize. Взагалом можна все реалізувати,
//  адже основна проблема уже зараз, це просо спарсити весь стрінг і перетворити його у потрібні типи.
//  Також думаю додати можилвість із файла перетворювати. І реалізувати seriazlize також.
//  З готового ою'єкта буде створюватися json у  виді стрінга.
//  Тут відразу можна зрозуміти, що потрібно буде знати ім'я полів, як у boost::pfr,
//  тому з цим розібратися і самому реаізувати на темлпейтах

JsonValue JsonConverter::deserialize(const std::string &t_json_str) noexcept {
  JsonValue json_value;
  cstr_iterator iterator = skipWhitespaces(t_json_str, t_json_str.cbegin());
  cstr_iterator end_iterator = t_json_str.cend();

  while (iterator != end_iterator) {
    iterator = deserialize_helper(iterator, json_value);
  }

  return json_value;
}

void JsonConverter::parseObject(std::string_view t_json_str, JsonValue &t_res_obj) noexcept {
  auto start = t_json_str.begin();
}

void JsonConverter::parseArray(std::string_view t_json_str, JsonValue &t_res_obj) noexcept {

}

void JsonConverter::parsePrimitiveValue(std::string_view t_json_str, JsonValue &t_res_obj) noexcept {
  const auto [key, value] = retrieveKeyValuePair(t_json_str);
  JsonPrimitiveType resValue;
  if (value == "true") {
    resValue = JsonPrimitiveType(true);
  } else if (value == "false") {
    resValue = JsonPrimitiveType(false);
  } else if (value == "null") {
    resValue = JsonPrimitiveType(nullptr);
  } else if (isNumber(value)) {
    resValue = JsonPrimitiveType(std::stoll(value));
  } else if (isFloatNumber(value)) {
    resValue = JsonPrimitiveType(std::stod(value));
  } else {
    resValue = JsonPrimitiveType(value);
  }
  (*t_res_obj.m_values)[key] = resValue;
}

std::pair<std::string, std::string> JsonConverter::retrieveKeyValuePair(std::string_view t_json_str) noexcept {
  std::string key, value;
  auto start = skipWhitespaces(t_json_str, t_json_str.begin());
  auto end = start;

  if (*start == '"') {
    end = ++start;
    while (*end != '"') {
      ++end;
    }
    key = t_json_str.substr(start - t_json_str.begin(), end - start);
    start = 1 + end;
  }

  while (*start != '"') {
    ++start;
  }
  end = 1 + start;

  while (*end != '"') {
    ++end;
  }
  ++end;

  value = t_json_str.substr(start - t_json_str.begin(), end - start);
  return std::pair<std::string, std::string>({std::move(key), std::move(value)});
}

JsonConverter::cstr_iterator JsonConverter::deserialize_helper(cstr_iterator it, JsonValue &t_res_obj) noexcept {
  if (*it == '{') {
    return parseObject(++it, t_res_obj);
  } else if (*it =='[') {
    return parseArray(++it, t_res_obj);
  }
  return parsePrimitiveValue(it, t_res_obj);
}

}
