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

#define LOG_TAG "ElementWiseAidlUnionTest"

#include <aidl/android/hardware/audio/effect/DynamicsProcessing.h>
#include <gtest/gtest.h>
#include <log/log.h>
#include <system/elementwise_op.h>

using ::aidl::android::hardware::audio::effect::DynamicsProcessing;
using ::android::audio_utils::elementwise_clamp;
using ::android::audio_utils::elementwise_max;
using ::android::audio_utils::elementwise_min;

static DynamicsProcessing dynamicsProcessing(int v = 0) {
  const DynamicsProcessing::EngineArchitecture engine{
      .preferredProcessingDurationMs = static_cast<float>(v),
      .preEqStage = DynamicsProcessing::StageEnablement{.bandCount = v},
      .postEqStage = DynamicsProcessing::StageEnablement{.bandCount = v},
      .mbcStage = DynamicsProcessing::StageEnablement{.bandCount = v},
  };

  return DynamicsProcessing::make<DynamicsProcessing::engineArchitecture>(engine);
}

static DynamicsProcessing dynamicsProcessing(int v1, int v2) {
  const DynamicsProcessing::EngineArchitecture engine{
      .preferredProcessingDurationMs = static_cast<float>(v1),
      .preEqStage = DynamicsProcessing::StageEnablement{.bandCount = v2},
      .postEqStage = DynamicsProcessing::StageEnablement{.bandCount = v1},
      .mbcStage = DynamicsProcessing::StageEnablement{.bandCount = v2},
  };

  return DynamicsProcessing::make<DynamicsProcessing::engineArchitecture>(engine);
}

static const DynamicsProcessing kDpDifferentTag;

class ElementWiseAidlUnionTest : public ::testing::TestWithParam<int> {
 public:
  ElementWiseAidlUnionTest() : value(GetParam()) {}

 protected:
  const int value;
};

// min/max/clamp op on same AIDL unions should get same value as result
TEST_P(ElementWiseAidlUnionTest, aidl_union_op_self) {
  const DynamicsProcessing dp = dynamicsProcessing(value);
  auto min = elementwise_min(dp, dp);
  ASSERT_NE(std::nullopt, min);
  EXPECT_EQ(dp, min.value());
  auto max = elementwise_max(dp, dp);
  ASSERT_NE(std::nullopt, max);
  EXPECT_EQ(dp, max.value());
  auto clamp = elementwise_clamp(dp, dp, dp);
  ASSERT_NE(std::nullopt, clamp);
  EXPECT_EQ(dp, clamp.value());
}

// min/max/clamp op on AIDL unions with ascending order
TEST_P(ElementWiseAidlUnionTest, aidl_union_op_ascending) {
  const DynamicsProcessing dp1 = dynamicsProcessing(value);
  const DynamicsProcessing dp2 = dynamicsProcessing(value + 1);
  const DynamicsProcessing dp3 = dynamicsProcessing(value + 2);
  auto min = elementwise_min(dp1, dp2);
  ASSERT_NE(std::nullopt, min);
  EXPECT_EQ(dp1, min.value());

  auto max = elementwise_max(dp1, dp2);
  ASSERT_NE(std::nullopt, max);
  EXPECT_EQ(dp2, max.value());

  auto clamped = elementwise_clamp(dp1, dp1, dp3);
  ASSERT_NE(std::nullopt, clamped);
  EXPECT_EQ(dp1, clamped.value());

  clamped = elementwise_clamp(dp2, dp1, dp3);
  ASSERT_NE(std::nullopt, clamped);
  EXPECT_EQ(dp2, clamped.value());

  clamped = elementwise_clamp(dp3, dp1, dp3);
  ASSERT_NE(std::nullopt, clamped);
  EXPECT_EQ(dp3, clamped.value());

  clamped = elementwise_clamp(dp1, dp2, dp3);
  ASSERT_NE(std::nullopt, clamped);
  EXPECT_EQ(dp2, clamped.value());
}

// min/max/clamp op on AIDL unions with descending order
TEST_P(ElementWiseAidlUnionTest, aidl_union_op_descending) {
  const DynamicsProcessing dp1 = dynamicsProcessing(value);
  const DynamicsProcessing dp2 = dynamicsProcessing(value + 1);
  const DynamicsProcessing dp3 = dynamicsProcessing(value + 2);
  auto min = elementwise_min(dp2, dp1);
  ASSERT_NE(std::nullopt, min);
  EXPECT_EQ(dp1, min.value());

  auto max = elementwise_max(dp2, dp1);
  ASSERT_NE(std::nullopt, max);
  EXPECT_EQ(dp2, max.value());

  auto clamped = elementwise_clamp(dp3, dp2, dp1);
  ASSERT_EQ(std::nullopt, clamped);

  clamped = elementwise_clamp(dp1, dp3, dp1);
  ASSERT_EQ(std::nullopt, clamped);

  clamped = elementwise_clamp(dp2, dp3, dp1);
  ASSERT_EQ(std::nullopt, clamped);

  clamped = elementwise_clamp(dp3, dp3, dp1);
  ASSERT_EQ(std::nullopt, clamped);

  clamped = elementwise_clamp(dp1, dp3, dp2);
  ASSERT_EQ(std::nullopt, clamped);
}

constexpr int kTestParamValues[] = {0, 1, 10};

INSTANTIATE_TEST_SUITE_P(AidlUtilsTest, ElementWiseAidlUnionTest,
                         testing::ValuesIn(kTestParamValues));

// expect `std::nullopt` when comparing two AIDL unions with different tags
TEST(ElementWiseAidlUnionTest, aidl_union_op_mismatch_tag) {
  const DynamicsProcessing dp = dynamicsProcessing();

  EXPECT_EQ(std::nullopt, elementwise_min(dp, kDpDifferentTag));
  EXPECT_EQ(std::nullopt, elementwise_min(kDpDifferentTag, dp));
  EXPECT_EQ(std::nullopt, elementwise_max(dp, kDpDifferentTag));
  EXPECT_EQ(std::nullopt, elementwise_max(kDpDifferentTag, dp));
  EXPECT_EQ(std::nullopt, elementwise_clamp(dp, dp, kDpDifferentTag));
  EXPECT_EQ(std::nullopt, elementwise_clamp(dp, kDpDifferentTag, dp));
}

// min/max op on AIDL unions with mixed parameter values
TEST(ElementWiseAidlUnionTest, aidl_union_op_compare_mix) {
  const auto dp12 = dynamicsProcessing(1, 2);
  const auto dp21 = dynamicsProcessing(2, 1);
  const auto dp34 = dynamicsProcessing(3, 4);
  const auto dp43 = dynamicsProcessing(4, 3);

  auto min = elementwise_min(dp12, dp21);
  ASSERT_NE(std::nullopt, min);
  EXPECT_EQ(dynamicsProcessing(1), min.value());
  auto max = elementwise_max(dp12, dp21);
  ASSERT_NE(std::nullopt, max);
  EXPECT_EQ(dynamicsProcessing(2), max.value());

  min = elementwise_min(dp34, dp43);
  ASSERT_NE(std::nullopt, min);
  EXPECT_EQ(dynamicsProcessing(3), min.value());
  max = elementwise_max(dp34, dp43);
  ASSERT_NE(std::nullopt, max);
  EXPECT_EQ(dynamicsProcessing(4), max.value());
}

// clamp op on AIDL unions with mixed parameter values
TEST(ElementWiseAidlUnionTest, aidl_union_op_clamp_mix) {
  const auto dp3 = dynamicsProcessing(3);
  const auto dp4 = dynamicsProcessing(4);
  const auto dp34 = dynamicsProcessing(3, 4);
  const auto dp43 = dynamicsProcessing(4, 3);
  const auto dp33 = dynamicsProcessing(3, 3);
  const auto dp44 = dynamicsProcessing(4, 4);

  auto clamped = elementwise_clamp(dp34, dp3, dp4);
  ASSERT_NE(std::nullopt, clamped);
  EXPECT_EQ(dp34, clamped.value());
  clamped = elementwise_clamp(dp43, dp33, dp44);
  ASSERT_NE(std::nullopt, clamped);
  EXPECT_EQ(dp43, clamped.value());
  clamped = elementwise_clamp(dp34, dp3, dp3);
  ASSERT_NE(std::nullopt, clamped);
  EXPECT_EQ(dp3, clamped.value());
  clamped = elementwise_clamp(dp43, dp4, dp4);
  ASSERT_NE(std::nullopt, clamped);
  EXPECT_EQ(dp4, clamped.value());
}