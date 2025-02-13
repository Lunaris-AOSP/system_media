/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <audio_utils/intrinsic_utils.h>

#include <gtest/gtest.h>
#include <random>
#include <vector>

static constexpr float kFloatTolerance = 1e-3;
static constexpr size_t kStandardSize = 8;
static constexpr float kRangeMin = -10.f;
static constexpr float kRangeMax = 10.f;

// see also std::seed_seq
static size_t seedCounter = 42;

// create uniform distribution
template <typename T, typename V>
static void initUniform(V& v, T rangeMin, T rangeMax) {
    std::minstd_rand gen(++seedCounter);
    std::uniform_real_distribution<T> dis(rangeMin, rangeMax);

    android::audio_utils::intrinsics::vapply([&]() { return dis(gen); }, v);
}

using android::audio_utils::intrinsics::veval;

// constexpr method tests can use static_assert.

static constexpr android::audio_utils::intrinsics::internal_array_t<float, 3> xyzzy =
        { 10, 10, 10 };

static_assert(android::audio_utils::intrinsics::internal_array_t<float, 3>(10) ==
        android::audio_utils::intrinsics::internal_array_t<float, 3>(
                { 10, 10, 10 }));

static_assert(android::audio_utils::intrinsics::internal_array_t<float, 3>(10) !=
        android::audio_utils::intrinsics::internal_array_t<float, 3>(
                { 10, 10, 20 }));

static_assert(android::audio_utils::intrinsics::internal_array_t<float, 3>(10) !=
        android::audio_utils::intrinsics::internal_array_t<float, 3>(
                { 10, 10 })); // implicit zero fill at end.

static_assert(android::audio_utils::intrinsics::internal_array_t<float, 3>( { 10, 10, 0 }) ==
        android::audio_utils::intrinsics::internal_array_t<float, 3>(
                { 10, 10 })); // implicit zero fill at end.


static_assert(android::audio_utils::intrinsics::internal_array_t<float, 3>(3) ==
    []() { android::audio_utils::intrinsics::internal_array_t<float, 3>  temp;
           vapply(3, temp);
           return temp; }());

TEST(IntrisicUtilsTest, vector_hw_ctor_compatibility) {
    const android::audio_utils::intrinsics::vector_hw_t<float, 3> a{ 1, 2, 3 };
    const android::audio_utils::intrinsics::vector_hw_t<float, 3> b(
        android::audio_utils::intrinsics::internal_array_t<float, 3>{ 1, 2, 3 });
    const android::audio_utils::intrinsics::vector_hw_t<float, 3> c(
        android::audio_utils::intrinsics::internal_array_t<float, 3>{ 1, 2, 2 });
    EXPECT_TRUE(android::audio_utils::intrinsics::veq(a, b));
    EXPECT_FALSE(android::audio_utils::intrinsics::veq(a, c));
}

TEST(IntrisicUtilsTest, veq_nan) {
    const android::audio_utils::intrinsics::vector_hw_t<float, 3> a(std::nanf(""));
    EXPECT_EQ(0, std::memcmp(&a, &a, sizeof(a)));  // bitwise equal.
    EXPECT_FALSE(android::audio_utils::intrinsics::veq(a, a));  // logically nan is not.
}

TEST(IntrisicUtilsTest, veq_zero) {
    int32_t neg = 0x8000'0000;
    int32_t pos = 0;
    float negzero, poszero;
    memcpy(&negzero, &neg, sizeof(neg));  // float negative zero.
    memcpy(&poszero, &pos, sizeof(pos));  // float positive zero.
    const android::audio_utils::intrinsics::vector_hw_t<float, 3> a(negzero);
    const android::audio_utils::intrinsics::vector_hw_t<float, 3> b(poszero);
    EXPECT_NE(0, std::memcmp(&a, &b, sizeof(a)));  // bitwise not-equal.
    EXPECT_TRUE(android::audio_utils::intrinsics::veq(a, b));  // logically equal.
}

template <typename D>
class IntrisicUtilsTest : public ::testing::Test {
};

// Basic intrinsic tests.
using FloatTypes = ::testing::Types<float, double,
        android::audio_utils::intrinsics::internal_array_t<float, kStandardSize>,
        android::audio_utils::intrinsics::internal_array_t<float, 1>,
        android::audio_utils::intrinsics::internal_array_t<double, kStandardSize>,
        android::audio_utils::intrinsics::vector_hw_t<float, kStandardSize>,
        android::audio_utils::intrinsics::vector_hw_t<float, 1>,
        android::audio_utils::intrinsics::vector_hw_t<float, 2>,
        android::audio_utils::intrinsics::vector_hw_t<float, 4>,
        android::audio_utils::intrinsics::vector_hw_t<float, 7>,
        android::audio_utils::intrinsics::vector_hw_t<float, 15>
        >;
TYPED_TEST_CASE(IntrisicUtilsTest, FloatTypes);

TYPED_TEST(IntrisicUtilsTest, vector_hw_ctor) {
    if constexpr (!std::is_arithmetic_v<TypeParam>) {
        if constexpr(std::is_same_v<float, typename TypeParam::element_t>) {
            android::audio_utils::intrinsics::vector_hw_t<float, TypeParam::size()>
                    a(TypeParam(0.5));
        }
    }
}

TYPED_TEST(IntrisicUtilsTest, vabs_constant) {
    const TypeParam value(-3.125f);
    const TypeParam result = veval([](auto v) { return std::abs(v); }, value);
    ASSERT_EQ(result, android::audio_utils::intrinsics::vabs(value));
}

TYPED_TEST(IntrisicUtilsTest, vabs_random) {
    TypeParam value;
    initUniform(value, kRangeMin, kRangeMax);
    const TypeParam result = veval([](auto v) { return std::abs(v); }, value);
    ASSERT_EQ(result, android::audio_utils::intrinsics::vabs(value));
}

TYPED_TEST(IntrisicUtilsTest, vadd_constant) {
    const TypeParam a(0.25f);
    const TypeParam b(0.5f);
    const TypeParam result = veval(
            [](auto x, auto y) { return x + y; }, a, b);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vadd(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vadd_random) {
    TypeParam a, b;
    initUniform(a, kRangeMin, kRangeMax);
    initUniform(b, kRangeMin, kRangeMax);
    const TypeParam result = veval(
            [](auto x, auto y) { return x + y; }, a, b);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vadd(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vaddv_random) {
    TypeParam a;
    initUniform(a, kRangeMin, kRangeMax);
    using element_t = decltype(android::audio_utils::intrinsics::first_element_of(a));
    element_t result{};
    android::audio_utils::intrinsics::vapply([&result] (element_t value) { result += value; }, a);
    EXPECT_NEAR(result, android::audio_utils::intrinsics::vaddv(a), kFloatTolerance);
}

TYPED_TEST(IntrisicUtilsTest, vdupn) {
    constexpr float ref = 1.f;
    const TypeParam value(ref);
    EXPECT_EQ(value, android::audio_utils::intrinsics::vdupn<TypeParam>(ref));
}

TYPED_TEST(IntrisicUtilsTest, vld1) {
    const TypeParam value(2.f);
    using element_t = decltype(android::audio_utils::intrinsics::first_element_of(value));
    EXPECT_EQ(value, android::audio_utils::intrinsics::vld1<TypeParam>(
            reinterpret_cast<const element_t*>(&value)));
}

TYPED_TEST(IntrisicUtilsTest, vmax_constant) {
    const TypeParam a(0.25f);
    const TypeParam b(0.5f);
    const TypeParam result = veval(
            [](auto x, auto y) { return std::max(x, y); }, a, b);
    ASSERT_EQ(result, android::audio_utils::intrinsics::vmax(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vmax_random) {
    TypeParam a, b;
    initUniform(a, kRangeMin, kRangeMax);
    initUniform(b, kRangeMin, kRangeMax);
    const TypeParam result = veval(
            [](auto x, auto y) { return std::max(x, y); }, a, b);
    ASSERT_EQ(result, android::audio_utils::intrinsics::vmax(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vmaxv_random) {
    TypeParam a;
    initUniform(a, kRangeMin, kRangeMax);
    using element_t = decltype(android::audio_utils::intrinsics::first_element_of(a));
    element_t result = android::audio_utils::intrinsics::first_element_of(a);
    android::audio_utils::intrinsics::vapply(
            [&result] (element_t value) { result = std::max(result, value); }, a);
    ASSERT_EQ(result, android::audio_utils::intrinsics::vmaxv(a));
}

TYPED_TEST(IntrisicUtilsTest, vmax_random_scalar) {
    TypeParam a;
    initUniform(a, kRangeMin, kRangeMax);
    using element_t = decltype(android::audio_utils::intrinsics::first_element_of(a));
    const element_t scalar = 3.f;
    TypeParam b(scalar);
    const TypeParam result = veval(
            [](auto x, auto y) { return std::max(x, y); }, a, b);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmax(a, scalar));
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmax(scalar, a));
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmax(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vmin_constant) {
    const TypeParam a(0.25f);
    const TypeParam b(0.5f);
    const TypeParam result = veval(
            [](auto x, auto y) { return std::min(x, y); }, a, b);
    ASSERT_EQ(result, android::audio_utils::intrinsics::vmin(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vmin_random) {
    TypeParam a, b;
    initUniform(a, kRangeMin, kRangeMax);
    initUniform(b, kRangeMin, kRangeMax);
    const TypeParam result = veval(
            [](auto x, auto y) { return std::min(x, y); }, a, b);
    ASSERT_EQ(result, android::audio_utils::intrinsics::vmin(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vminv_random) {
    TypeParam a;
    initUniform(a, kRangeMin, kRangeMax);
    using element_t = decltype(android::audio_utils::intrinsics::first_element_of(a));
    element_t result = android::audio_utils::intrinsics::first_element_of(a);
    android::audio_utils::intrinsics::vapply(
            [&result] (element_t value) { result = std::min(result, value); }, a);
    ASSERT_EQ(result, android::audio_utils::intrinsics::vminv(a));
}

TYPED_TEST(IntrisicUtilsTest, vmin_random_scalar) {
    TypeParam a;
    initUniform(a, kRangeMin, kRangeMax);
    using element_t = decltype(android::audio_utils::intrinsics::first_element_of(a));
    const element_t scalar = 3.f;
    TypeParam b(scalar);
    const TypeParam result = veval(
            [](auto x, auto y) { return std::min(x, y); }, a, b);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmin(a, scalar));
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmin(scalar, a));
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmin(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vmla_constant) {
    const TypeParam a(2.125f);
    const TypeParam b(2.25f);
    const TypeParam c(2.5f);
    const TypeParam result = veval(
            [](auto x, auto y, auto z) { return x + y * z; }, a, b, c);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmla(a, b, c));
}

TYPED_TEST(IntrisicUtilsTest, vmla_random) {
    TypeParam a, b, c;
    initUniform(a, kRangeMin, kRangeMax);
    initUniform(b, kRangeMin, kRangeMax);
    initUniform(c, kRangeMin, kRangeMax);
    const TypeParam result = veval(
            [](auto x, auto y, auto z) { return x + y * z; }, a, b, c);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmla(a, b, c));
}

TYPED_TEST(IntrisicUtilsTest, vmla_random_scalar) {
    TypeParam a, b;
    initUniform(a, kRangeMin, kRangeMax);
    initUniform(b, kRangeMin, kRangeMax);
    using element_t = decltype(android::audio_utils::intrinsics::first_element_of(a));
    const element_t scalar = 3.f;
    const TypeParam c(scalar);
    const TypeParam result = veval(
            [](auto x, auto y, auto z) { return x + y * z; }, a, b, c);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmla(a, scalar, b));
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmla(a, b, scalar));
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmla(a, b, c));
}

TYPED_TEST(IntrisicUtilsTest, vmul_constant) {
    const TypeParam a(2.25f);
    const TypeParam b(2.5f);
    const TypeParam result = veval(
            [](auto x, auto y) { return x * y; }, a, b);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmul(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vmul_random) {
    TypeParam a, b;
    initUniform(a, kRangeMin, kRangeMax);
    initUniform(b, kRangeMin, kRangeMax);
    const TypeParam result = veval(
            [](auto x, auto y) { return x * y; }, a, b);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmul(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vmul_random_scalar) {
    TypeParam a;
    initUniform(a, kRangeMin, kRangeMax);
    using element_t = decltype(android::audio_utils::intrinsics::first_element_of(a));
    const element_t scalar = 3.f;
    const TypeParam b(scalar);
    const TypeParam result = veval(
            [](auto x, auto y) { return x * y; }, a, b);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmul(a, scalar));
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmul(scalar, a));
    EXPECT_EQ(result, android::audio_utils::intrinsics::vmul(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vneg_constant) {
    const TypeParam value(3.125f);
    const TypeParam result = veval([](auto v) { return -v; }, value);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vneg(value));
}

TYPED_TEST(IntrisicUtilsTest, vneg_random) {
    TypeParam value;
    initUniform(value, kRangeMin, kRangeMax);
    const TypeParam result = veval([](auto v) { return -v; }, value);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vneg(value));
}

TYPED_TEST(IntrisicUtilsTest, vst1) {
    constexpr float ref = 2.f;
    const TypeParam value(ref);
    TypeParam destination(1.f);
    using element_t = decltype(android::audio_utils::intrinsics::first_element_of(value));
    android::audio_utils::intrinsics::vst1(
            reinterpret_cast<element_t*>(&destination),
            android::audio_utils::intrinsics::vdupn<TypeParam>(ref));
    EXPECT_EQ(value, destination);
}

TYPED_TEST(IntrisicUtilsTest, vsub_constant) {
    const TypeParam a(1.25f);
    const TypeParam b(1.5f);
    const TypeParam result = veval(
            [](auto x, auto y) { return x - y; }, a, b);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vsub(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vsub_random) {
    TypeParam a, b;
    initUniform(a, kRangeMin, kRangeMax);
    initUniform(b, kRangeMin, kRangeMax);
    const TypeParam result = veval(
            [](auto x, auto y) { return x - y; }, a, b);
    EXPECT_EQ(result, android::audio_utils::intrinsics::vsub(a, b));
}

TYPED_TEST(IntrisicUtilsTest, vclamp_constant) {
    const TypeParam a(0.25f);
    const TypeParam b(0.5f);
    const TypeParam c(1.f);
    const TypeParam result = veval(
            [](auto x, auto y, auto z) { if (y > z) {
                return std::min(std::max(x, y), z);  // undefined behavior, make defined.
            } else {
                return std::clamp(x, y, z);
            }
            }, a, b, c);
    ASSERT_EQ(result, android::audio_utils::intrinsics::vclamp(a, b, c));
}

TYPED_TEST(IntrisicUtilsTest, vclamp_random) {
    TypeParam a, b, c;
    initUniform(a, kRangeMin, kRangeMax);
    initUniform(b, kRangeMin, kRangeMax);
    initUniform(c, kRangeMin, kRangeMax);
    const TypeParam result = veval(
            [](auto x, auto y, auto z) { if (y > z) {
                return std::min(std::max(x, y), z);  // undefined behavior, make defined.
            } else {
                return std::clamp(x, y, z);
            }
            }, a, b, c);
    ASSERT_EQ(result, android::audio_utils::intrinsics::vclamp(a, b, c));
}
