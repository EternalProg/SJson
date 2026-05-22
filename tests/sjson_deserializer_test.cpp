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
