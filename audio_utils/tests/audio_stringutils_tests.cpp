/*
 * Copyright 2024 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "audio_stringutils_tests"

#include <audio_utils/StringUtils.h>
#include <gtest/gtest.h>

TEST(audio_utils_string, parseVector) {
    {
        std::vector<int32_t> values;
        EXPECT_EQ(true, android::audio_utils::stringutils::parseVector(
                "0{4,300,0,-112343,350}9", &values));
        EXPECT_EQ(values, std::vector<int32_t>({0, 4, 300, 0, -112343, 350, 9}));
    }
    {
        std::vector<int32_t> values;
        EXPECT_EQ(true, android::audio_utils::stringutils::parseVector("53", &values));
        EXPECT_EQ(values, std::vector<int32_t>({53}));
    }
    {
        std::vector<int32_t> values;
        EXPECT_EQ(false, android::audio_utils::stringutils::parseVector("5{3,6*3}3", &values));
        EXPECT_EQ(values, std::vector<int32_t>({}));
    }
    {
        std::vector<int32_t> values = {1}; // should still be this when parsing fails
        std::vector<int32_t> expected = {1};
        EXPECT_EQ(false, android::audio_utils::stringutils::parseVector("51342abcd,1232", &values));
        EXPECT_EQ(values, std::vector<int32_t>({1}));
    }
    {
        std::vector<int32_t> values = {2}; // should still be this when parsing fails
        EXPECT_EQ(false, android::audio_utils::stringutils::parseVector(
                "12345678901234,12345678901234", &values));
        EXPECT_EQ(values, std::vector<int32_t>({2}));
    }
}


TEST(audio_utils_string, device_parsing) {
    auto devaddr = android::audio_utils::stringutils::getDeviceAddressPairs("(DEVICE, )");
    ASSERT_EQ((size_t)1, devaddr.size());
    ASSERT_EQ("DEVICE", devaddr[0].first);
    ASSERT_EQ("", devaddr[0].second);

    devaddr = android::audio_utils::stringutils::getDeviceAddressPairs(
            "(DEVICE1, A)|(D, ADDRB)");
    ASSERT_EQ((size_t)2, devaddr.size());
    ASSERT_EQ("DEVICE1", devaddr[0].first);
    ASSERT_EQ("A", devaddr[0].second);
    ASSERT_EQ("D", devaddr[1].first);
    ASSERT_EQ("ADDRB", devaddr[1].second);

    devaddr = android::audio_utils::stringutils::getDeviceAddressPairs(
            "(A,B)|(C,D)");
    ASSERT_EQ((size_t)2, devaddr.size());
    ASSERT_EQ("A", devaddr[0].first);
    ASSERT_EQ("B", devaddr[0].second);
    ASSERT_EQ("C", devaddr[1].first);
    ASSERT_EQ("D", devaddr[1].second);

    devaddr = android::audio_utils::stringutils::getDeviceAddressPairs(
            "  ( A1 , B )  | ( C , D2 )  ");
    ASSERT_EQ((size_t)2, devaddr.size());
    ASSERT_EQ("A1", devaddr[0].first);
    ASSERT_EQ("B", devaddr[0].second);
    ASSERT_EQ("C", devaddr[1].first);
    ASSERT_EQ("D2", devaddr[1].second);

    devaddr = android::audio_utils::stringutils::getDeviceAddressPairs(
            " Z  ");
    ASSERT_EQ((size_t)1, devaddr.size());
    ASSERT_EQ("Z", devaddr[0].first);

    devaddr = android::audio_utils::stringutils::getDeviceAddressPairs(
            "  A | B|C  ");
    ASSERT_EQ((size_t)3, devaddr.size());
    ASSERT_EQ("A", devaddr[0].first);
    ASSERT_EQ("", devaddr[0].second);
    ASSERT_EQ("B", devaddr[1].first);
    ASSERT_EQ("", devaddr[1].second);
    ASSERT_EQ("C", devaddr[2].first);
    ASSERT_EQ("", devaddr[2].second);

    devaddr = android::audio_utils::stringutils::getDeviceAddressPairs(
            "  A | (B1, 10) |C  ");
    ASSERT_EQ((size_t)3, devaddr.size());
    ASSERT_EQ("A", devaddr[0].first);
    ASSERT_EQ("", devaddr[0].second);
    ASSERT_EQ("B1", devaddr[1].first);
    ASSERT_EQ("10", devaddr[1].second);
    ASSERT_EQ("C", devaddr[2].first);
    ASSERT_EQ("", devaddr[2].second);
}

TEST(audio_utils_string, ConvertToLowerCamelCase) {
    EXPECT_EQ("camelCase.andSnakeCase.4Fun.2Funny.look4It",
            android::audio_utils::stringutils::toLowerCamelCase(
                    "camel_case.AndSnake_Case.4Fun.2FUNNY.Look_4__it"));

    EXPECT_EQ("abc.abc1_10_100$def #!g",
            android::audio_utils::stringutils::toLowerCamelCase(
                    "ABC.abc_1_10_100$def #!g"));
}

TEST(audio_utils_string, ConvertToUpperCamelCase) {
    EXPECT_EQ("CamelCase.AndSnakeCase.4Fun.2Funny.Look4It",
            android::audio_utils::stringutils::toUpperCamelCase(
                    "camel_case.AndSnake_Case.4Fun.2FUNNY.Look_4__it"));

    EXPECT_EQ("Abc.Abc1_10_100$Def #!G",
            android::audio_utils::stringutils::toUpperCamelCase(
                    "ABC.abc_1_10_100$def #!g"));
}

TEST(audio_utils_string, ConvertToLowerSnakeCase) {
    EXPECT_EQ("camel_case.and_snake_case.4fun.2funny.look_4_it",
            android::audio_utils::stringutils::toLowerSnakeCase(
                    "camel_case.AndSnake_Case.4Fun.2FUNNY.Look_4__it"));

    EXPECT_EQ("abc.abc_1_10_100$def #!g",
            android::audio_utils::stringutils::toLowerSnakeCase(
                    "ABC.abc_1_10_100$def #!g"));
}

TEST(audio_utils_string, ConvertToUpperSnakeCase) {
    EXPECT_EQ("CAMEL_CASE.AND_SNAKE_CASE.4FUN.2FUNNY.LOOK_4_IT",
            android::audio_utils::stringutils::toUpperSnakeCase(
                    "camel_case.AndSnake_Case.4Fun.2FUNNY.Look_4__it"));

    EXPECT_EQ("ABC.ABC_1_10_100$DEF #!G",
            android::audio_utils::stringutils::toUpperSnakeCase(
                    "ABC.abc_1_10_100$def #!g"));
}

TEST(audio_utils_string, PreserveDigitSequence) {
    EXPECT_EQ("CamelCase10_100",
            android::audio_utils::stringutils::toUpperCamelCase("camel_case10_100"));
    EXPECT_EQ("camelCase10_100",
            android::audio_utils::stringutils::toLowerCamelCase("camel_case10_100"));
}

TEST(audio_utils_string, appendWithReplacement_empty) {
    std::string s("");
    android::audio_utils::stringutils::appendWithReplacement(s, "", '|', '+');
    EXPECT_EQ("", s);
}

TEST(audio_utils_string, appendWithReplacement_basic) {
    std::string s("hello");
    android::audio_utils::stringutils::appendWithReplacement(s, "+||", '|', '+');
    EXPECT_EQ("hello+++", s);
}

TEST(audio_utils_string, appendWithReplacement_copy) {
    std::string s("hello");
    android::audio_utils::stringutils::appendWithReplacement(s, " world", '|', '+');
    EXPECT_EQ("hello world", s);
}