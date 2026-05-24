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
// {"pi": 3.14, "neg": -42, "sci": 1.5e10}
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
