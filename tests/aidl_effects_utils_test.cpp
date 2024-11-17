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

#include <array>

#define LOG_TAG "AidlEffectsUtilsTest"

#include <aidl/android/hardware/audio/effect/IEffect.h>
#include <gtest/gtest.h>
#include <log/log.h>
#include <system/audio_effects/aidl_effects_utils.h>

using ::aidl::android::hardware::audio::effect::Capability;
using ::aidl::android::hardware::audio::effect::Downmix;
using ::aidl::android::hardware::audio::effect::DynamicsProcessing;
using ::aidl::android::hardware::audio::effect::Parameter;
using ::aidl::android::hardware::audio::effect::Range;

// Helper function to create a DynamicsProcessing parameter with custom tag
template <typename DynamicsProcessing::Tag TAG = DynamicsProcessing::engineArchitecture>
static DynamicsProcessing dynamicsProcessing(int v, int n = 0) {
  if constexpr (TAG == DynamicsProcessing::engineArchitecture) {
    const DynamicsProcessing::EngineArchitecture engine{
        .preferredProcessingDurationMs = static_cast<float>(v),
        .preEqStage = DynamicsProcessing::StageEnablement{.bandCount = v},
        .postEqStage = DynamicsProcessing::StageEnablement{.bandCount = v},
        .mbcStage = DynamicsProcessing::StageEnablement{.bandCount = v},
    };
    return DynamicsProcessing::make<DynamicsProcessing::engineArchitecture>(engine);
  } else if constexpr (TAG == DynamicsProcessing::inputGain) {
    std::vector<DynamicsProcessing::InputGain> gain;
    for (int i = 0; i < n; i++) {
      gain.emplace_back(DynamicsProcessing::InputGain{
          .channel = i, .gainDb = static_cast<float>(v)});
    }
    return DynamicsProcessing::make<DynamicsProcessing::inputGain>(gain);
  } else {
    static_assert(false, "tag not supported");
  }
}

static Parameter parameter(int v) {
  return Parameter::make<Parameter::specific>(
      Parameter::Specific::make<Parameter::Specific::dynamicsProcessing>(dynamicsProcessing(v)));
}

static Capability capability(int min, int max) {
  return Capability{
      .range =
          Range::make<Range::dynamicsProcessing>({Range::DynamicsProcessingRange{
              .min = dynamicsProcessing(min), .max = dynamicsProcessing(max),
          }}),
  };
}

static Capability multiCapability(int min, int max) {
  return Capability{
      .range = Range::make<Range::dynamicsProcessing>({
          Range::DynamicsProcessingRange{
              .min = dynamicsProcessing(min), .max = dynamicsProcessing(max),
          },
          Range::DynamicsProcessingRange{
              .min = dynamicsProcessing<DynamicsProcessing::inputGain>(min),
              .max = dynamicsProcessing<DynamicsProcessing::inputGain>(max),
          },
      }),
  };
}

// construct an invalid capability with different vector size
static Capability capabilityWithDifferentVecSize(int min, int minVecSize, int max, int maxVecSize) {
  return Capability{
      .range = Range::make<Range::dynamicsProcessing>({
          Range::DynamicsProcessingRange{
              .min = dynamicsProcessing<DynamicsProcessing::inputGain>(min, minVecSize),
              .max = dynamicsProcessing<DynamicsProcessing::inputGain>(max, maxVecSize),
          },
      }),
  };
}

static Capability downmixCapability() {
  return Capability{.range = Range::make<Range::downmix>({Range::DownmixRange{}})};
}

// static Range::DynamicsProcessingRange createMultiRange(int min, int max) {
//   return Range::DynamicsProcessingRange{.min = min, .max = max};
// }

using FindSharedCapabilityTestParam =
    std::tuple<int /* a_min */, int /* a_max */, int /*b_min*/, int /*b_max*/>;
class FindSharedCapabilityTest
    : public ::testing::TestWithParam<FindSharedCapabilityTestParam> {
 public:
  FindSharedCapabilityTest()
      : a_min(std::get<0>(GetParam())),
        a_max(std::get<1>(GetParam())),
        b_min(std::get<2>(GetParam())),
        b_max(std::get<3>(GetParam())) {}

 protected:
  const int a_min, a_max, b_min, b_max;
};

/**
 * Find shared capability with all elements in the predefined capability array `kCapArray`.
 */
TEST_P(FindSharedCapabilityTest, basic) {
  std::optional<Capability> cap =
      findSharedCapability(capability(a_min, a_max), capability(b_min, b_max));
  ASSERT_NE(std::nullopt, cap);
  EXPECT_EQ(capability(std::max(a_min, b_min), std::min(a_max, b_max)).range, cap->range);
}

TEST_P(FindSharedCapabilityTest, multi_tags) {
  std::optional<Capability> cap = findSharedCapability(
      multiCapability(a_min, a_max), multiCapability(b_min, b_max));
  ASSERT_NE(std::nullopt, cap);
  EXPECT_EQ(multiCapability(std::max(a_min, b_min), std::min(a_max, b_max)).range, cap->range);
}

TEST(FindSharedCapabilityTest, diff_effects) {
  EXPECT_EQ(std::nullopt, findSharedCapability(capability(0, 1), downmixCapability()));
}

TEST(FindSharedCapabilityTest, capability_with_diff_vec) {
  auto target = capabilityWithDifferentVecSize(1, 5, 2, 6);
  auto shared = findSharedCapability(
      capabilityWithDifferentVecSize(0 /*min*/, 5 /*minVacSize*/, 3 /*max*/, 6 /*maxVacSize*/),
      capabilityWithDifferentVecSize(1 /*min*/, 5 /*minVacSize*/, 2 /*max*/, 6 /*maxVacSize*/));
  ASSERT_NE(std::nullopt, shared);
  EXPECT_EQ(target.range, shared->range);

  // the shared min is invalid because the vector size is different
  target = capabilityWithDifferentVecSize(0, 0, 1, 3);
  shared = findSharedCapability(
      capabilityWithDifferentVecSize(0 /*min*/, 2 /*minVacSize*/, 1 /*max*/, 3 /*maxVacSize*/),
      capabilityWithDifferentVecSize(0 /*min*/, 3 /*minVacSize*/, 1 /*max*/, 3 /*maxVacSize*/));
  ASSERT_NE(std::nullopt, shared);
  ASSERT_EQ(Range::dynamicsProcessing, shared->range.getTag());
  auto dpRanges = shared->range.get<Range::dynamicsProcessing>();
  ASSERT_EQ(1ul, dpRanges.size());
  EXPECT_EQ(DynamicsProcessing::vendor, dpRanges[0].min.getTag());
  const auto targetRanges = target.range.get<Range::dynamicsProcessing>();
  EXPECT_EQ(targetRanges[0].max, dpRanges[0].max);

  // the shared min and max both invalid because the vector size is different
  target = capabilityWithDifferentVecSize(0, 0, 1, 3);
  shared = findSharedCapability(
      capabilityWithDifferentVecSize(0 /*min*/, 2 /*minVacSize*/, 1 /*max*/, 5 /*maxVacSize*/),
      capabilityWithDifferentVecSize(0 /*min*/, 3 /*minVacSize*/, 1 /*max*/, 3 /*maxVacSize*/));
  EXPECT_EQ(std::nullopt, shared);
}

using ClampParameterTestParam = std::tuple<int /* a */, int /* b */>;
class ClampParameterTest
    : public ::testing::TestWithParam<ClampParameterTestParam> {
 public:
  ClampParameterTest()
      : a(std::get<0>(GetParam())), b(std::get<1>(GetParam())) {}

 protected:
  const int a, b;
};

TEST_P(ClampParameterTest, basic) {
  const std::optional<Parameter> clamped =
      clampParameter<Range::dynamicsProcessing, Parameter::Specific::dynamicsProcessing>(
          parameter(a), capability(a, b));
  if (a <= b) {
    ASSERT_NE(std::nullopt, clamped);
    EXPECT_EQ(parameter(a), clamped.value());
  } else {
    EXPECT_EQ(std::nullopt, clamped);
  }
}

TEST_P(ClampParameterTest, clamp_to_min) {
  const std::optional<Parameter> clamped =
      clampParameter<Range::dynamicsProcessing, Parameter::Specific::dynamicsProcessing>(
          parameter(a - 1), capability(a, b));
  if (a <= b) {
    ASSERT_NE(std::nullopt, clamped);
    EXPECT_EQ(parameter(a), clamped.value());
  } else {
    EXPECT_EQ(std::nullopt, clamped);
  }
}

TEST_P(ClampParameterTest, clamp_to_max) {
  const std::optional<Parameter> clamped =
      clampParameter<Range::dynamicsProcessing, Parameter::Specific::dynamicsProcessing>(
          parameter(b + 1), capability(a, b));
  if (a <= b) {
    ASSERT_NE(std::nullopt, clamped);
    EXPECT_EQ(parameter(b), clamped.value());
  } else {
    EXPECT_EQ(std::nullopt, clamped);
  }
}

// minimum and maximum value used to initialize effect parameters for comparison
static constexpr int kParameterStartValue = 1;
static constexpr int kParameterEndValue = 4; // end will not included in the generated values

INSTANTIATE_TEST_SUITE_P(
    AidlEffectsUtilsTest, FindSharedCapabilityTest,
    ::testing::Combine(testing::Range(kParameterStartValue, kParameterEndValue),
                       testing::Range(kParameterStartValue, kParameterEndValue),
                       testing::Range(kParameterStartValue, kParameterEndValue),
                       testing::Range(kParameterStartValue, kParameterEndValue)),
    [](const testing::TestParamInfo<FindSharedCapabilityTest::ParamType>& info) {
      return std::to_string(std::get<0>(info.param)) + "_" +
             std::to_string(std::get<1>(info.param)) + "_" +
             std::to_string(std::get<2>(info.param)) + "_" +
             std::to_string(std::get<3>(info.param));
    });

INSTANTIATE_TEST_SUITE_P(
    AidlEffectsUtilsTest, ClampParameterTest,
    ::testing::Combine(testing::Range(kParameterStartValue, kParameterEndValue),
                       testing::Range(kParameterStartValue, kParameterEndValue)),
    [](const testing::TestParamInfo<ClampParameterTest::ParamType>& info) {
      return std::to_string(std::get<0>(info.param)) + "_" +
             std::to_string(std::get<1>(info.param));
    });