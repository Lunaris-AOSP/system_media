/*
 * Copyright (C) 2024 The Android Open Source Project
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

#include <audio_utils/elementwise_op.h>
#include <gtest/gtest.h>
#include <log/log.h>

#include <string>

using android::audio_utils::elementwise_clamp;
using android::audio_utils::elementwise_max;
using android::audio_utils::elementwise_min;
using android::audio_utils::kMaxStructMember;
using android::audio_utils::op_tuple_elements;

enum class OpTestEnum { E1, E2, E3 };

struct OpTestSSS {
  double a;
  bool b;
};

struct OpTestSS {
  OpTestSSS sss;
  int c;
  std::vector<float> d;
  OpTestEnum e;
};

struct OpTestS {
  OpTestSS ss;
  int f;
  bool g;
  std::string h;
};

std::ostream& operator<<(std::ostream& os, const OpTestEnum& e) {
  switch (e) {
    case OpTestEnum::E1: {
      os << "E1";
      break;
    }
    case OpTestEnum::E2: {
      os << "E2";
      break;
    }
    case OpTestEnum::E3: {
      os << "E3";
      break;
    }
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const OpTestSSS& sss) {
  os << "a: " << sss.a << ", b: " << sss.b;
  return os;
}

std::ostream& operator<<(std::ostream& os, const OpTestSS& ss) {
  os << ss.sss << ", c: " << ss.c << ", d: [";
  for (const auto& itor : ss.d) {
    os << itor << " ";
  }
  os << "], e: " << ss.e;
  return os;
}

std::ostream& operator<<(std::ostream& os, const OpTestS& s) {
  os << s.ss << ", f: " << s.f << ", g: " << s.g << ", h" << s.h;
  return os;
}

constexpr bool operator==(const OpTestSSS& lhs, const OpTestSSS& rhs) {
  return lhs.a == rhs.a && lhs.b == rhs.b;
}

constexpr bool operator==(const OpTestSS& lhs, const OpTestSS& rhs) {
  return lhs.sss == rhs.sss && lhs.c == rhs.c && lhs.d == rhs.d &&
         lhs.e == rhs.e;
}

constexpr bool operator==(const OpTestS& lhs, const OpTestS& rhs) {
  return lhs.ss == rhs.ss && lhs.f == rhs.f && lhs.g == rhs.g && lhs.h == rhs.h;
}

const OpTestSSS sss1{.a = 1, .b = false};
const OpTestSSS sss2{.a = sss1.a + 1, .b = true};
const OpTestSSS sss3{.a = sss2.a + 1, .b = true};
const OpTestSSS sss_mixed{.a = sss1.a - 1, .b = true};
const OpTestSSS sss_clamped_1_3{.a = sss1.a, .b = true};
const OpTestSSS sss_clamped_2_3{.a = sss2.a, .b = true};

const OpTestSS ss1{.sss = sss1, .c = 1, .d = {1.f}, .e = OpTestEnum::E1};
const OpTestSS ss2{
    .sss = sss2, .c = ss1.c + 1, .d = {ss1.d[0] + 1}, .e = OpTestEnum::E2};
const OpTestSS ss3{
    .sss = sss3, .c = ss2.c + 1, .d = {ss2.d[0] + 1}, .e = OpTestEnum::E3};
const OpTestSS ss_mixed{
    .sss = sss_mixed, .c = ss1.c - 1, .d = {ss3.d[0] + 1}, .e = OpTestEnum::E3};
const OpTestSS ss_clamped_1_3{
    .sss = sss_clamped_1_3, .c = ss1.c, .d = {ss3.d[0]}, .e = OpTestEnum::E3};
const OpTestSS ss_clamped_2_3{
    .sss = sss_clamped_2_3, .c = ss2.c, .d = {ss3.d[0]}, .e = OpTestEnum::E3};

const OpTestS s1{.ss = ss1, .f = 1, .g = false, .h = "s1"};
const OpTestS s2{.ss = ss2, .f = s1.f + 1, .g = false, .h = "s2"};
const OpTestS s3{.ss = ss3, .f = s2.f + 1, .g = true, .h = "s3"};
const OpTestS s_mixed{.ss = ss_mixed, .f = s1.f - 1, .g = true, .h = "mixed"};
const OpTestS s_clamped_1_3{
    .ss = ss_clamped_1_3, .f = s1.f, .g = true, .h = "s1"};
const OpTestS s_clamped_2_3{
    .ss = ss_clamped_2_3, .f = s2.f, .g = true, .h = "s2"};

// clamp a structure with range of min == max
TEST(ClampOpTest, elementwise_clamp) {
  std::optional<OpTestS> clamped;

  clamped = elementwise_clamp(s2, s1, s3);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s2);

  clamped = elementwise_clamp(s1, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s2);

  clamped = elementwise_clamp(s3, s1, s2);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s2);
}

// clamp a structure with range of min == max
TEST(ClampOpTest, clamp_same_min_max) {
  std::optional<OpTestS> clamped;

  clamped = elementwise_clamp(s1, s1, s1);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s1);

  clamped = elementwise_clamp(s2, s1, s1);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s1);

  clamped = elementwise_clamp(s3, s1, s1);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s1);

  clamped = elementwise_clamp(s1, s2, s2);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s2);

  clamped = elementwise_clamp(s2, s2, s2);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s2);

  clamped = elementwise_clamp(s3, s2, s2);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s2);

  clamped = elementwise_clamp(s1, s3, s3);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s3);

  clamped = elementwise_clamp(s2, s3, s3);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s3);

  clamped = elementwise_clamp(s3, s3, s3);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s3);
}

// clamp a structure with invalid range (min > max)
TEST(ClampOpTest, clamp_invalid_range) {
  EXPECT_EQ(std::nullopt, elementwise_clamp(s1, s2, s1));
  EXPECT_EQ(std::nullopt, elementwise_clamp(s2, s3, s2));
  EXPECT_EQ(std::nullopt, elementwise_clamp(s3, s3, s1));
}

// all members in p3 clamped to s2 but p3.ss.sss.a
TEST(ClampOpTest, clamp_to_max_a) {
  OpTestS p3 = s3;
  std::optional<OpTestS> clamped;

  p3.ss.sss.a = s1.ss.sss.a;
  clamped = elementwise_clamp(p3, s1, s2);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p3.ss.sss.a is not clamped
  EXPECT_EQ(clamped->ss.sss.a, s1.ss.sss.a);
  // ensure all other members correctly clamped to max
  clamped->ss.sss.a = s2.ss.sss.a;
  EXPECT_EQ(*clamped, s2);
}

// all members in p3 clamped to s2 but p3.ss.sss.b
TEST(ClampOpTest, clamp_to_max_b) {
  OpTestS p3 = s3;
  std::optional<OpTestS> clamped;

  p3.ss.sss.b = s1.ss.sss.b;
  clamped = elementwise_clamp(p3, s1, s2);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p3.ss.sss.b is not clamped
  EXPECT_EQ(clamped->ss.sss.b, s1.ss.sss.b);
  // ensure all other members correctly clamped to max
  clamped->ss.sss.b = s2.ss.sss.b;
  EXPECT_EQ(*clamped, s2);
}

// all members in p3 clamped to s2 but p3.ss.c
TEST(ClampOpTest, clamp_to_max_c) {
  OpTestS p3 = s3;
  std::optional<OpTestS> clamped;

  p3.ss.c = s1.ss.c;
  clamped = elementwise_clamp(p3, s1, s2);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(p3.ss.c, s1.ss.c);
  // ensure p3.ss.c is not clamped
  EXPECT_EQ(clamped->ss.c, s1.ss.c);
  // ensure all other members correctly clamped to max
  clamped->ss.c = s2.ss.c;
  EXPECT_EQ(*clamped, s2);
}

// all members in p3 clamped to s2 but p3.ss.d
TEST(ClampOpTest, clamp_to_max_d) {
  OpTestS p3 = s3;
  std::optional<OpTestS> clamped;

  p3.ss.d = s1.ss.d;
  clamped = elementwise_clamp(p3, s1, s2);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p3.ss.d is not clamped
  EXPECT_EQ(clamped->ss.d, s1.ss.d);
  // ensure all other members correctly clamped to max
  clamped->ss.d = s2.ss.d;
  EXPECT_EQ(*clamped, s2);
}

// all members in p3 clamped to s2 but p3.ss.e
TEST(ClampOpTest, clamp_to_max_e) {
  OpTestS p3 = s3;
  std::optional<OpTestS> clamped;

  p3.ss.e = s1.ss.e;
  clamped = elementwise_clamp(p3, s1, s2);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p3.ss.e is not clamped
  EXPECT_EQ(clamped->ss.e, s1.ss.e);
  // ensure all other members correctly clamped to max
  clamped->ss.e = s2.ss.e;
  EXPECT_EQ(*clamped, s2);
}

// all members in p3 clamped to s2 but p3.f
TEST(ClampOpTest, clamp_to_max_f) {
  OpTestS p3 = s3;
  std::optional<OpTestS> clamped;

  p3.f = s1.f;
  clamped = elementwise_clamp(p3, s1, s2);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p3.f is not clamped
  EXPECT_EQ(clamped->f, s1.f);
  // ensure all other members correctly clamped to max
  clamped->f = s2.f;
  EXPECT_EQ(*clamped, s2);
}

// all members in p3 clamped to s2 but p3.g
TEST(ClampOpTest, clamp_to_max_g) {
  OpTestS p3 = s3;
  std::optional<OpTestS> clamped;

  p3.g = s1.g;
  clamped = elementwise_clamp(p3, s1, s2);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p3.g is not clamped
  EXPECT_EQ(clamped->g, s1.g);
  // ensure all other members correctly clamped to max
  clamped->g = s2.g;
  EXPECT_EQ(*clamped, s2);
}

// all members in p3 clamped to s2 but p3.h
TEST(ClampOpTest, clamp_to_max_h) {
  OpTestS p3 = s3;
  std::optional<OpTestS> clamped;

  p3.h = s1.h;
  clamped = elementwise_clamp(p3, s1, s2);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p3.g is not clamped
  EXPECT_EQ(clamped->h, s1.h);
  // ensure all other members correctly clamped to max
  clamped->h = s2.h;
  EXPECT_EQ(*clamped, s2);
}

// all members in p1 clamped to s2 except p1.ss.sss.a
TEST(ClampOpTest, clamp_to_min_a) {
  OpTestS p1 = s1;
  p1.ss.sss.a = s3.ss.sss.a;
  std::optional<OpTestS> clamped = elementwise_clamp(p1, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p1.ss.sss.a is not clamped
  EXPECT_EQ(clamped->ss.sss.a, s3.ss.sss.a);
  // ensure all other members correctly clamped to max
  clamped->ss.sss.a = s2.ss.sss.a;
  EXPECT_EQ(*clamped, s2);
}

// all members in p1 clamped to s2 but p1.ss.sss.b
TEST(ClampOpTest, clamp_to_min_b) {
  OpTestS p1 = s1;
  p1.ss.sss.b = s3.ss.sss.b;
  std::optional<OpTestS> clamped = elementwise_clamp(p1, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p1.ss.sss.b is not clamped
  EXPECT_EQ(clamped->ss.sss.b, s3.ss.sss.b);
  // ensure all other members correctly clamped to max
  clamped->ss.sss.b = s2.ss.sss.b;
  EXPECT_EQ(*clamped, s2);
}

TEST(ClampOpTest, clamp_to_min_c) {
  OpTestS p1 = s1;
  p1.ss.c = s3.ss.c;
  std::optional<OpTestS> clamped = elementwise_clamp(p1, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(p1.ss.c, s3.ss.c);
  // ensure p1.ss.c is not clamped
  EXPECT_EQ(clamped->ss.c, s3.ss.c);
  // ensure all other members correctly clamped to max
  clamped->ss.c = s2.ss.c;
  EXPECT_EQ(*clamped, s2);
}

TEST(ClampOpTest, clamp_to_min_d) {
  OpTestS p1 = s1;
  p1.ss.d = s3.ss.d;
  std::optional<OpTestS> clamped = elementwise_clamp(p1, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p1.ss.d is not clamped
  EXPECT_EQ(clamped->ss.d, s3.ss.d);
  // ensure all other members correctly clamped to max
  clamped->ss.d = s2.ss.d;
  EXPECT_EQ(*clamped, s2);
}

TEST(ClampOpTest, clamp_to_min_e) {
  OpTestS p1 = s1;
  p1.ss.e = s3.ss.e;
  std::optional<OpTestS> clamped = elementwise_clamp(p1, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p1.ss.e is not clamped
  EXPECT_EQ(clamped->ss.e, s3.ss.e);
  // ensure all other members correctly clamped to max
  clamped->ss.e = s2.ss.e;
  EXPECT_EQ(*clamped, s2);
}

TEST(ClampOpTest, clamp_to_min_f) {
  OpTestS p1 = s1;
  p1.f = s3.f;
  std::optional<OpTestS> clamped = elementwise_clamp(p1, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p1.f is not clamped
  EXPECT_EQ(clamped->f, s3.f);
  // ensure all other members correctly clamped to max
  clamped->f = s2.f;
  EXPECT_EQ(*clamped, s2);
}

TEST(ClampOpTest, clamp_to_min_g) {
  OpTestS p1 = s1;
  p1.g = s3.g;
  std::optional<OpTestS> clamped = elementwise_clamp(p1, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p1.g is not clamped
  EXPECT_EQ(clamped->g, s3.g);
  // ensure all other members correctly clamped to max
  clamped->g = s2.g;
  EXPECT_EQ(*clamped, s2);
}

TEST(ClampOpTest, clamp_to_min_h) {
  OpTestS p1 = s1;
  p1.h = s3.h;
  std::optional<OpTestS> clamped = elementwise_clamp(p1, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  // ensure p1.g is not clamped
  EXPECT_EQ(clamped->h, s3.h);
  // ensure all other members correctly clamped to max
  clamped->h = s2.h;
  EXPECT_EQ(*clamped, s2);
}

// test vector clamp with same size target and min/max
TEST(ClampOpTest, clamp_vector_same_size) {
  std::optional<OpTestS> clamped;
  OpTestS target = s2, min = s1, max = s3;

  min.ss.d = {1, 11, 21};
  max.ss.d = {10, 20, 30};
  target.ss.d = {0, 30, 21};
  std::vector<float> expect = {1, 20, 21};
  clamped = elementwise_clamp(target, min, max);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(clamped->ss.d, expect);

  min.ss.d = {10, 11, 1};
  max.ss.d = {10, 20, 30};
  target.ss.d = {20, 20, 20};
  expect = {10, 20, 20};
  clamped = elementwise_clamp(target, min, max);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(clamped->ss.d, expect);

  clamped = elementwise_clamp(target, min, min);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, min);

  clamped = elementwise_clamp(target, max, max);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, max);
}

// test vector clamp with one element min and max
TEST(ClampOpTest, clamp_vector_one_member_min_max) {
  std::optional<OpTestS> clamped;
  OpTestS target = s2, min = s1, max = s3;

  min.ss.d = {10};
  max.ss.d = {20};
  target.ss.d = {0, 30, 20};
  std::vector<float> expect = {10, 20, 20};

  clamped = elementwise_clamp(target, min, max);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(clamped->ss.d, expect);
}

TEST(ClampOpTest, clamp_vector_one_min) {
  std::optional<OpTestS> clamped;
  OpTestS target = s2, min = s1, max = s3;

  min.ss.d = {0};
  max.ss.d = {20, 10, 30};
  target.ss.d = {-1, 30, 20};
  std::vector<float> expect = {0, 10, 20};

  clamped = elementwise_clamp(target, min, max);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(clamped->ss.d, expect);
}

TEST(ClampOpTest, clamp_vector_one_max) {
  std::optional<OpTestS> clamped;
  OpTestS target = s2, min = s1, max = s3;

  min.ss.d = {0, 10, 20};
  max.ss.d = {20};
  target.ss.d = {-1, 30, 20};
  std::vector<float> expect = {0, 20, 20};

  clamped = elementwise_clamp(target, min, max);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(clamped->ss.d, expect);
}

TEST(ClampOpTest, clamp_vector_invalid_range) {
  std::optional<OpTestS> clamped;
  OpTestS target = s2, min = s1, max = s3;

  target.ss.d = {-1, 30, 20};
  std::vector<float> expect = {0, 20, 20};

  min.ss.d = {0, 10};
  max.ss.d = {20};
  clamped = elementwise_clamp(target, min, max);
  EXPECT_EQ(clamped, std::nullopt);

  min.ss.d = {0, 10, 20};
  max.ss.d = {};
  clamped = elementwise_clamp(target, min, max);
  EXPECT_EQ(clamped, std::nullopt);

  min.ss.d = {};
  max.ss.d = {0, 10, 20};
  clamped = elementwise_clamp(target, min, max);
  EXPECT_EQ(clamped, std::nullopt);

  min.ss.d = {0, 10, 20};
  max.ss.d = {0, 10, 10};
  clamped = elementwise_clamp(target, min, max);
  EXPECT_EQ(clamped, std::nullopt);

  min.ss.d = {0, 10, 5, 10};
  max.ss.d = {0, 10, 10};
  clamped = elementwise_clamp(target, min, max);
  EXPECT_EQ(clamped, std::nullopt);

  min.ss.d = {};
  max.ss.d = {};
  target.ss.d = {};
  clamped = elementwise_clamp(target, min, max);
  EXPECT_EQ(clamped, std::nullopt);
}

TEST(ClampOpTest, clamp_string) {
  std::optional<OpTestS> clamped;
  OpTestS target = s2, min = s1, max = s3;

  min.h = "";
  max.h = "";
  target.h = "";
  clamped = elementwise_clamp(target, min, max);
  EXPECT_EQ(*clamped, target);

  min.h = "apple";
  max.h = "pear";
  target.h = "orange";
  clamped = elementwise_clamp(target, min, max);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(clamped->h, std::clamp(target.h, min.h, max.h));
  EXPECT_EQ(*clamped, target);

  target.h = "aardvark";
  clamped = elementwise_clamp(target, min, max);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(clamped->h, std::clamp(target.h, min.h, max.h));
  target.h = clamped->h;
  EXPECT_EQ(*clamped, target);

  target.h = "zebra";
  clamped = elementwise_clamp(target, min, max);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(clamped->h, std::clamp(target.h, min.h, max.h));
  target.h = clamped->h;
  EXPECT_EQ(*clamped, target);
}

// clamp a mixed structure in range
TEST(ClampOpTest, clamp_mixed) {
  std::optional<OpTestS> clamped;

  clamped = elementwise_clamp(s_mixed, s1, s3);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s_clamped_1_3);

  clamped = elementwise_clamp(s_mixed, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s_clamped_2_3);
}

// clamp a mixed structure in range
TEST(ClampOpTest, clamp_primitive_type) {
  std::optional<OpTestS> clamped;

  clamped = elementwise_clamp(s_mixed, s1, s3);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s_clamped_1_3);

  clamped = elementwise_clamp(s_mixed, s2, s3);
  ASSERT_NE(clamped, std::nullopt);
  EXPECT_EQ(*clamped, s_clamped_2_3);
}

// Template function to return an array of size N
template <size_t N>
auto getArrayN() {
  return std::array<int, N>{};
}

// Recursive function to make a tuple of arrays up to size N
template <std::size_t N>
auto makeTupleOfArrays() {
  if constexpr (N == 1) {
    return std::make_tuple(getArrayN<1>());
  } else {
    return std::tuple_cat(makeTupleOfArrays<N - 1>(),
                          std::make_tuple(getArrayN<N>()));
  }
}

// test the clamp utility can handle structures with up to
// `android::audio_utils::kMaxStructMember` members
TEST(ClampOpTest, clamp_different_struct_members) {
  auto clampVerifyOp = [](auto&& arr) {
    auto m1(arr), m2(arr), m3(arr);
    m1.fill(1);
    m2.fill(2);
    m3.fill(3);

    auto clamped = elementwise_clamp(m2, m1, m3);
    ASSERT_NE(clamped, std::nullopt);
    EXPECT_EQ(*clamped, m2);

    clamped = elementwise_clamp(m1, m2, m3);
    ASSERT_NE(clamped, std::nullopt);
    EXPECT_EQ(*clamped, m2);

    clamped = elementwise_clamp(m3, m1, m2);
    ASSERT_NE(clamped, std::nullopt);
    EXPECT_EQ(*clamped, m2);

    // invalid range
    EXPECT_EQ(elementwise_clamp(m3, m2, m1), std::nullopt);
    EXPECT_EQ(elementwise_clamp(m3, m3, m1), std::nullopt);
    EXPECT_EQ(elementwise_clamp(m3, m3, m2), std::nullopt);
  };

  auto arrays = makeTupleOfArrays<kMaxStructMember>();
  for (size_t i = 0; i < kMaxStructMember; i++) {
    op_tuple_elements(arrays, i, clampVerifyOp);
  }
}

template <typename T>
void MinMaxOpTestHelper(const T& a, const T& b, const T& expectedLower,
                        const T& expectedUpper,
                        const std::optional<T>& unexpected = std::nullopt) {
  // lower
  auto result = elementwise_min(a, b);
  ASSERT_NE(unexpected, *result);
  EXPECT_EQ(expectedLower, *result);

  result = elementwise_min(b, a);
  ASSERT_NE(unexpected, *result);
  EXPECT_EQ(expectedLower, *result);

  result = elementwise_min(a, a);
  EXPECT_EQ(a, elementwise_min(a, a));
  EXPECT_EQ(b, elementwise_min(b, b));

  // upper
  result = elementwise_max(a, b);
  ASSERT_NE(unexpected, result);
  EXPECT_EQ(expectedUpper, *result);

  result = elementwise_max(b, a);
  ASSERT_NE(unexpected, result);
  EXPECT_EQ(expectedUpper, *result);

  EXPECT_EQ(a, elementwise_max(a, a));
  EXPECT_EQ(b, elementwise_max(b, b));
}

TEST(MinMaxOpTest, primitive_type_int) {
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(1, 2, 1, 2));
}

TEST(MinMaxOpTest, primitive_type_float) {
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(.1f, .2f, .1f, .2f));
}

TEST(MinMaxOpTest, primitive_type_string) {
  std::string a = "ab", b = "ba";
  EXPECT_NO_FATAL_FAILURE(
      MinMaxOpTestHelper(a, b, std::min(a, b), std::max(a, b)));
  a = "", b = "0";
  EXPECT_NO_FATAL_FAILURE(
      MinMaxOpTestHelper(a, b, std::min(a, b), std::max(a, b)));
  a = "abc", b = "1234";
  EXPECT_NO_FATAL_FAILURE(
      MinMaxOpTestHelper(a, b, std::min(a, b), std::max(a, b)));
}

TEST(MinMaxOpTest, primitive_type_enum) {
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(OpTestEnum::E1, OpTestEnum::E2,
                                             OpTestEnum::E1, OpTestEnum::E2));
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(OpTestEnum::E3, OpTestEnum::E2,
                                             OpTestEnum::E2, OpTestEnum::E3));
}

TEST(MinMaxOpTest, vector_same_size) {
  std::vector<int> v1, v2, expected_lower, expected_upper;
  EXPECT_NO_FATAL_FAILURE(
      MinMaxOpTestHelper(v1, v2, expected_lower, expected_upper));

  v1 = {1}, v2 = {2}, expected_lower = {1}, expected_upper = {2};
  EXPECT_NO_FATAL_FAILURE(
      MinMaxOpTestHelper(v1, v2, expected_lower, expected_upper));

  v1 = {3, 2, 3}, v2 = {2, 2, 2}, expected_lower = v2, expected_upper = v1;
  EXPECT_NO_FATAL_FAILURE(
      MinMaxOpTestHelper(v1, v2, expected_lower, expected_upper));

  v1 = {3, 2, 3}, v2 = {1, 4, 1}, expected_lower = {1, 2, 1},
  expected_upper = {3, 4, 3};
  EXPECT_NO_FATAL_FAILURE(
      MinMaxOpTestHelper(v1, v2, expected_lower, expected_upper));
}

TEST(MinMaxOpTest, vector_different_size_valid) {
  std::vector<int> v1, v2({1}), expected_lower, expected_upper({1});
  EXPECT_NO_FATAL_FAILURE(
      MinMaxOpTestHelper(v1, v2, expected_lower, expected_upper));

  v1 = {1, 2, 3, 1, 0, 5}, v2 = {2}, expected_lower = {1, 2, 2, 1, 0, 2},
  expected_upper = {2, 2, 3, 2, 2, 5};
  EXPECT_NO_FATAL_FAILURE(
      MinMaxOpTestHelper(v1, v2, expected_lower, expected_upper));
}

// invalid vector size combination, expect std::nullopt
TEST(MinMaxOpTest, invalid_vector_size) {
  std::vector<int> v1 = {3, 2}, v2 = {2, 2, 2};
  EXPECT_EQ(std::nullopt, elementwise_min(v1, v2));
  EXPECT_EQ(std::nullopt, elementwise_min(v2, v1));
  EXPECT_EQ(std::nullopt, elementwise_max(v1, v2));
  EXPECT_EQ(std::nullopt, elementwise_max(v2, v1));
}

TEST(MinMaxOpTest, aggregate_type) {
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(sss1, sss2, sss1, sss2));
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(sss2, sss3, sss2, sss3));
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(sss1, sss3, sss1, sss3));

  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(ss1, ss2, ss1, ss2));
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(ss2, ss3, ss2, ss3));
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(ss1, ss3, ss1, ss3));

  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(s1, s2, s1, s2));
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(s2, s3, s2, s3));
  EXPECT_NO_FATAL_FAILURE(MinMaxOpTestHelper(s1, s3, s1, s3));
}

// invalid vector size combination in nested structure
TEST(MinMaxOpTest, invalid_vector_in_structure) {
  auto tt1 = ss1, tt2 = ss2;
  tt1.d = {.1f, .2f, .3f};
  tt2.d = {.1f, .2f, .3f, .4f, .5f};

  EXPECT_EQ(std::nullopt, elementwise_min(tt1, tt2));
  EXPECT_EQ(std::nullopt, elementwise_min(tt2, tt1));
  EXPECT_EQ(std::nullopt, elementwise_max(tt1, tt2));
  EXPECT_EQ(std::nullopt, elementwise_max(tt2, tt1));

  auto t1 = s1, t2 = s2;
  t1.ss = tt1, t2.ss = tt2;
  EXPECT_EQ(std::nullopt, elementwise_min(t1, t2));
  EXPECT_EQ(std::nullopt, elementwise_min(t2, t1));
  EXPECT_EQ(std::nullopt, elementwise_max(t1, t2));
  EXPECT_EQ(std::nullopt, elementwise_max(t2, t1));
}

TEST(MinMaxOpTest, aggregate_different_members) {
  auto boundaryVerifyOp = [](auto&& arr) {
    auto m1(arr), m2(arr);
    m1.fill(1);
    m2.fill(2);

    auto lower = elementwise_min(m1, m2);
    ASSERT_NE(lower, std::nullopt);
    EXPECT_EQ(*lower, m1);

    auto upper = elementwise_max(m1, m2);
    ASSERT_NE(upper, std::nullopt);
    EXPECT_EQ(*upper, m2);
  };

  auto arrays = makeTupleOfArrays<kMaxStructMember>();
  for (size_t i = 0; i < kMaxStructMember; i++) {
    op_tuple_elements(arrays, i, boundaryVerifyOp);
  }
}