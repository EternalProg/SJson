#include "include/JsonDeserializer.hpp"
#include <gtest/gtest.h>

TEST(SJsonDeserializerTest, DeserializeObjectFromString) {
  std::string json = R"JSON({
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
  )JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_EQ(obj["Image"]["Width"], 800ll);
  EXPECT_EQ(obj["Image"]["Height"], 600ll);
  EXPECT_EQ(obj["Image"]["Title"], "View from 15th Floor");
  EXPECT_EQ(obj["Image"]["Thumbnail"]["Url"],
            "http://www.example.com/image/481989943");
  EXPECT_EQ(obj["Image"]["Thumbnail"]["Height"], 125ll);
  EXPECT_EQ(obj["Image"]["Thumbnail"]["Width"], 100ll);
  EXPECT_FALSE(obj["Image"]["Animated"]);
  EXPECT_TRUE(obj["Image"]["IDs"].is_array());
  EXPECT_EQ(obj["Image"]["IDs"][0], 116ll);
  EXPECT_EQ(obj["Image"]["IDs"][1], 943ll);
  EXPECT_EQ(obj["Image"]["IDs"][2], 234ll);
  EXPECT_EQ(obj["Image"]["IDs"][3], 38793ll);
}

TEST(SJsonDeserializerTest, DeserializeArrayFromString) {
  std::string json = R"JSON([
        {
            "Name": "Alice",
            "Age": 30
        },
        {
            "Name": "Bob",
            "Age": 25
        }
    ])JSON";

  auto arr = sjson::deserialize(json);

  EXPECT_TRUE(arr.is_array());
  EXPECT_EQ(arr.size(), 2u);
  EXPECT_EQ(arr[0]["Name"], "Alice");
  EXPECT_EQ(arr[0]["Age"], 30ll);
  EXPECT_EQ(arr[1]["Name"], "Bob");
  EXPECT_EQ(arr[1]["Age"], 25ll);
}

TEST(SJsonDeserializerTest, DeserializePrimitiveTypes) {
  std::string json = R"JSON({
        "String": "Hello, World!",
        "Number": 42,
        "Boolean": true,
        "NullValue": null
    })JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_EQ(obj["String"], "Hello, World!");
  EXPECT_EQ(obj["Number"], 42ll);
  EXPECT_TRUE(obj["Boolean"]);
  EXPECT_TRUE(obj["NullValue"].is_null());
}

TEST(SJsonDeserializerTest, DeserializePrimitiveType) {
  std::string jsonString = R"JSON("Hello, World!")JSON";
  std::string jsonNumber = R"JSON(42)JSON";
  std::string jsonBoolean = R"JSON(true)JSON";
  std::string jsonNull = R"JSON(null)JSON";

  auto strValue = sjson::deserialize(jsonString);
  auto numValue = sjson::deserialize(jsonNumber);
  auto boolValue = sjson::deserialize(jsonBoolean);
  auto nullValue = sjson::deserialize(jsonNull);

  EXPECT_TRUE(strValue.is_string());
  EXPECT_EQ(strValue, "Hello, World!");
  EXPECT_TRUE(numValue.is_number());
  EXPECT_EQ(numValue, 42ll);
  EXPECT_TRUE(boolValue.is_boolean());
  EXPECT_TRUE(boolValue);
  EXPECT_TRUE(nullValue.is_null());
}

TEST(SJsonDeserializerTest, DeserializeNestedStructures) {
  std::string json = R"JSON({
        "Person": {
            "Name": "Charlie",
            "Age": 28,
            "Address": {
                "Street": "123 Main St",
                "City": "Anytown",
                "Country": "USA"
            },
            "Hobbies": ["Reading", "Traveling", "Cooking"]
        }
    })JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_EQ(obj["Person"]["Name"], "Charlie");
  EXPECT_EQ(obj["Person"]["Age"], 28ll);
  EXPECT_EQ(obj["Person"]["Address"]["Street"], "123 Main St");
  EXPECT_EQ(obj["Person"]["Address"]["City"], "Anytown");
  EXPECT_EQ(obj["Person"]["Address"]["Country"], "USA");
  EXPECT_TRUE(obj["Person"]["Hobbies"].is_array());
  EXPECT_EQ(obj["Person"]["Hobbies"][0], "Reading");
  EXPECT_EQ(obj["Person"]["Hobbies"][1], "Traveling");
  EXPECT_EQ(obj["Person"]["Hobbies"][2], "Cooking");
}

TEST(SJsonDeserializerTest, DeserializeEmptyObjectAndArray) {
  std::string json = R"JSON({
        "EmptyObject": {},
        "EmptyArray": []
    })JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_TRUE(obj["EmptyObject"].is_object());
  EXPECT_TRUE(obj["EmptyArray"].is_array());
  EXPECT_EQ(obj["EmptyArray"].size(), 0u);
}
TEST(SJsonDeserializerTest, DeserializeNumbers) {
  std::string json = R"JSON({
        "pi": 3.14,
        "neg": -42,
        "sci": 1.5e10
    })JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_TRUE(obj["pi"].is_number());
  EXPECT_EQ(obj["pi"], 3.14);
  EXPECT_TRUE(obj["neg"].is_number());
  EXPECT_EQ(obj["neg"], -42ll);
  EXPECT_TRUE(obj["sci"].is_number());
  EXPECT_EQ(obj["sci"], 1.5e10);
}

TEST(SJsonDeserializerTest, DeserializeStringWithEscapes) {
  std::string json = R"JSON({
        "newline": "line1\nline2",
        "tab": "col1\tcol2",
        "backslash": "path\\to\\file"
    })JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_EQ(obj["newline"], "line1\\nline2");
  EXPECT_EQ(obj["tab"], "col1\\tcol2");
  EXPECT_EQ(obj["backslash"], "path\\\\to\\\\file");
}

TEST(SJsonDeserializerTest, DeserializeUnicodeStrings) {
  std::string json = R"JSON({
        "cafe": "café",
        "japanese": "こんにちは",
        "emoji": "hello 😊",
        "chinese": "你好"
    })JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_EQ(obj["cafe"], "café");
  EXPECT_EQ(obj["japanese"], "こんにちは");
  EXPECT_EQ(obj["emoji"], "hello 😊");
  EXPECT_EQ(obj["chinese"], "你好");
}

TEST(SJsonDeserializerTest, DeserializeStringLookalike) {
  std::string json = R"JSON({
        "looks_like_object": "{}",
        "looks_like_array": "[]",
        "looks_like_number": "42",
        "looks_like_null": "null",
        "looks_like_bool": "true"
    })JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_TRUE(obj["looks_like_object"].is_string());
  EXPECT_EQ(obj["looks_like_object"], "{}");
  EXPECT_TRUE(obj["looks_like_array"].is_string());
  EXPECT_EQ(obj["looks_like_array"], "[]");
  EXPECT_TRUE(obj["looks_like_number"].is_string());
  EXPECT_EQ(obj["looks_like_number"], "42");
  EXPECT_TRUE(obj["looks_like_null"].is_string());
  EXPECT_EQ(obj["looks_like_null"], "null");
  EXPECT_TRUE(obj["looks_like_bool"].is_string());
  EXPECT_EQ(obj["looks_like_bool"], "true");
}

TEST(SJsonDeserializerTest, DeserializeWhitespaceVariants) {
  std::string json = "{\"a\" : 1,\t\"b\" : 2,\n\"c\" : 3}";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_EQ(obj["a"], 1ll);
  EXPECT_EQ(obj["b"], 2ll);
  EXPECT_EQ(obj["c"], 3ll);
}

TEST(SJsonDeserializerTest, DeserializeEdgeNumericValues) {
  std::string json = R"JSON({
        "zero": 0,
        "negative_zero": -0,
        "max_int": 9223372036854775807
    })JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_TRUE(obj["zero"].is_number());
  EXPECT_EQ(obj["zero"], 0ll);
  EXPECT_TRUE(obj["negative_zero"].is_number());
  EXPECT_EQ(obj["negative_zero"], 0ll);
  EXPECT_TRUE(obj["max_int"].is_number());
  EXPECT_EQ(obj["max_int"], 9223372036854775807ll);
}

TEST(SJsonDeserializerTest, DeserializeMixedTypeArray) {
  std::string json = R"JSON([1, "hello", true, null, {"nested": "obj"}, [1,2,3]])JSON";

  auto arr = sjson::deserialize(json);

  EXPECT_TRUE(arr.is_array());
  EXPECT_EQ(arr.size(), 6u);
  EXPECT_TRUE(arr[0].is_number());
  EXPECT_EQ(arr[0], 1ll);
  EXPECT_TRUE(arr[1].is_string());
  EXPECT_EQ(arr[1], "hello");
  EXPECT_TRUE(arr[2].is_boolean());
  EXPECT_TRUE(arr[2]);
  EXPECT_TRUE(arr[3].is_null());
  EXPECT_TRUE(arr[4].is_object());
  EXPECT_EQ(arr[4]["nested"], "obj");
  EXPECT_TRUE(arr[5].is_array());
  EXPECT_EQ(arr[5][0], 1ll);
  EXPECT_EQ(arr[5][1], 2ll);
  EXPECT_EQ(arr[5][2], 3ll);
}

TEST(SJsonDeserializerTest, DeserializeNestedEmptyContainers) {
  std::string json = R"JSON({
        "a": {},
        "b": [],
        "c": {"d": {}},
        "e": [[]]
    })JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_TRUE(obj["a"].is_object());
  EXPECT_EQ(obj["a"].size(), 0u);
  EXPECT_TRUE(obj["b"].is_array());
  EXPECT_EQ(obj["b"].size(), 0u);
  EXPECT_TRUE(obj["c"].is_object());
  EXPECT_TRUE(obj["c"]["d"].is_object());
  EXPECT_EQ(obj["c"]["d"].size(), 0u);
  EXPECT_TRUE(obj["e"].is_array());
  EXPECT_EQ(obj["e"].size(), 1u);
  EXPECT_TRUE(obj["e"][0].is_array());
  EXPECT_EQ(obj["e"][0].size(), 0u);
}

TEST(SJsonDeserializerTest, DeserializeDuplicateKeys) {
  std::string json = R"JSON({"a": 1, "a": 2})JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_TRUE(obj.is_object());
  EXPECT_EQ(obj["a"], 2ll);
}

TEST(SJsonDeserializerTest, ValidateTypeQueryMethods) {
  std::string json = R"JSON({
        "null_val": null,
        "str_val": "hello",
        "num_val": 42,
        "bool_val": true,
        "obj_val": {},
        "arr_val": []
    })JSON";

  auto obj = sjson::deserialize(json);

  auto &null_val = obj["null_val"];
  EXPECT_TRUE(null_val.is_null());
  EXPECT_FALSE(null_val.is_string());
  EXPECT_FALSE(null_val.is_number());
  EXPECT_FALSE(null_val.is_boolean());

  auto &str_val = obj["str_val"];
  EXPECT_FALSE(str_val.is_null());
  EXPECT_TRUE(str_val.is_string());
  EXPECT_FALSE(str_val.is_number());
  EXPECT_FALSE(str_val.is_boolean());

  auto &num_val = obj["num_val"];
  EXPECT_FALSE(num_val.is_null());
  EXPECT_FALSE(num_val.is_string());
  EXPECT_TRUE(num_val.is_number());
  EXPECT_FALSE(num_val.is_boolean());

  auto &bool_val = obj["bool_val"];
  EXPECT_FALSE(bool_val.is_null());
  EXPECT_FALSE(bool_val.is_string());
  EXPECT_FALSE(bool_val.is_number());
  EXPECT_TRUE(bool_val.is_boolean());

  EXPECT_TRUE(obj["obj_val"].is_object());
  EXPECT_TRUE(obj["arr_val"].is_array());
}

TEST(SJsonDeserializerTest, ValidateSizeMethod) {
  std::string json = R"JSON({
        "arr": [1, 2, 3],
        "obj": {"a": 1, "b": 2, "c": 3},
        "empty_arr": [],
        "empty_obj": {},
        "scalar": 42
    })JSON";

  auto obj = sjson::deserialize(json);

  EXPECT_EQ(obj["arr"].size(), 3u);
  EXPECT_EQ(obj["obj"].size(), 3u);
  EXPECT_EQ(obj["empty_arr"].size(), 0u);
  EXPECT_EQ(obj["empty_obj"].size(), 0u);
  EXPECT_THROW(obj["scalar"].size(), std::runtime_error);
}

TEST(SJsonDeserializerTest, DeserializeEmptyInput) {
  EXPECT_TRUE(sjson::deserialize("").is_null());
  EXPECT_TRUE(sjson::deserialize("   ").is_null());
}

TEST(SJsonDeserializerTest, DeserializeLargeArray) {
  std::string json = "[";
  for (int i = 0; i < 1000; ++i) {
    if (i > 0) json += ",";
    json += std::to_string(i);
  }
  json += "]";

  auto arr = sjson::deserialize(json);

  EXPECT_TRUE(arr.is_array());
  EXPECT_EQ(arr.size(), 1000u);
  EXPECT_EQ(arr[0], 0ll);
  EXPECT_EQ(arr[999], 999ll);
}
