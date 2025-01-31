/*
 * Copyright 2025 The Android Open Source Project
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

#include <functional>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

/*
Pixel 6 Pro Android 14
------------------------------------------------------------------------------
Benchmark                                    Time             CPU   Iterations
------------------------------------------------------------------------------
BM_VectorTestLoopFloat/1                  1199 ns         1195 ns       527528
BM_VectorTestLoopFloat/2                  2201 ns         2194 ns       319117
BM_VectorTestLoopFloat/3                  3226 ns         3215 ns       217631
BM_VectorTestLoopFloat/4                  4295 ns         4280 ns       163563
BM_VectorTestLoopFloat/5                  5392 ns         5375 ns       130251
BM_VectorTestLoopFloat/6                  6446 ns         6424 ns       109544
BM_VectorTestLoopFloat/7                  7571 ns         7545 ns        93144
BM_VectorTestLoopFloat/8                  8715 ns         8685 ns        81006
BM_VectorTestLoopFloat/9                  9868 ns         9834 ns        72006
BM_VectorTestLoopFloat/10                10992 ns        10954 ns        63695
BM_VectorTestLoopFloat/11                12186 ns        12145 ns        57963
BM_VectorTestLoopFloat/12                13329 ns        13282 ns        52824
BM_VectorTestLoopFloat/13                14467 ns        14417 ns        49040
BM_VectorTestLoopFloat/14                15699 ns        15645 ns        44942
BM_VectorTestLoopFloat/15                16820 ns        16762 ns        42023
BM_VectorTestLoopFloat/16                18007 ns        17946 ns        38813
BM_VectorTestLoopFloat/17                19243 ns        19177 ns        36480
BM_VectorTestLoopFloat/18                20470 ns        20398 ns        34367
BM_VectorTestLoopFloat/19                21599 ns        21523 ns        32726
BM_VectorTestLoopFloat/20                22830 ns        22750 ns        30758
BM_VectorTestLoopFloat/21                24006 ns        23921 ns        29306
BM_VectorTestLoopFloat/22                25181 ns        25091 ns        27997
BM_VectorTestLoopFloat/23                26458 ns        26362 ns        26585
BM_VectorTestLoopFloat/24                27729 ns        27630 ns        25197
BM_VectorTestLoopFloat/25                28706 ns        28604 ns        24326
BM_VectorTestLoopFloat/26                30003 ns        29895 ns        23408
BM_VectorTestLoopFloat/27                31374 ns        31259 ns        22552
BM_VectorTestLoopFloat/28                32364 ns        32251 ns        21548
BM_VectorTestLoopFloat/29                33556 ns        33438 ns        20977
BM_VectorTestLoopFloat/30                34976 ns        34847 ns        20169
BM_VectorTestLoopFloat/31                35903 ns        35771 ns        19584
BM_VectorTestLoopFloat/32                37303 ns        37156 ns        18962
BM_VectorTestConstArraySizeFloat/1         178 ns          177 ns      3986625
BM_VectorTestConstArraySizeFloat/2         632 ns          630 ns      1119563
BM_VectorTestConstArraySizeFloat/3        2067 ns         2060 ns       335407
BM_VectorTestConstArraySizeFloat/4        3704 ns         3692 ns       188037
BM_VectorTestConstArraySizeFloat/5        1796 ns         1789 ns       394081
BM_VectorTestConstArraySizeFloat/6        1905 ns         1898 ns       364662
BM_VectorTestConstArraySizeFloat/7        2722 ns         2712 ns       257824
BM_VectorTestConstArraySizeFloat/8        2018 ns         2011 ns       348922
BM_VectorTestConstArraySizeFloat/9        2981 ns         2970 ns       235582
BM_VectorTestConstArraySizeFloat/10       3087 ns         3076 ns       227236
BM_VectorTestConstArraySizeFloat/11       3972 ns         3959 ns       177225
BM_VectorTestConstArraySizeFloat/12       3045 ns         3034 ns       231600
BM_VectorTestConstArraySizeFloat/13       4386 ns         4370 ns       160592
BM_VectorTestConstArraySizeFloat/14       4203 ns         4188 ns       167829
BM_VectorTestConstArraySizeFloat/15       5230 ns         5212 ns       136365
BM_VectorTestConstArraySizeFloat/16       3999 ns         3985 ns       175834
BM_VectorTestConstArraySizeFloat/17       5563 ns         5543 ns       126600
BM_VectorTestConstArraySizeFloat/18       5262 ns         5243 ns       137721
BM_VectorTestConstArraySizeFloat/19       6341 ns         6319 ns       110219
BM_VectorTestConstArraySizeFloat/20       5029 ns         5011 ns       100000
BM_VectorTestConstArraySizeFloat/21       6510 ns         6491 ns       106033
BM_VectorTestConstArraySizeFloat/22       6428 ns         6409 ns       107897
BM_VectorTestConstArraySizeFloat/23       7544 ns         7521 ns        93162
BM_VectorTestConstArraySizeFloat/24       5878 ns         5861 ns       120695
BM_VectorTestConstArraySizeFloat/25       7983 ns         7956 ns        89747
BM_VectorTestConstArraySizeFloat/26       7554 ns         7530 ns        94650
BM_VectorTestConstArraySizeFloat/27       8810 ns         8782 ns        79411
BM_VectorTestConstArraySizeFloat/28       7020 ns         6993 ns       101845
BM_VectorTestConstArraySizeFloat/29       9208 ns         9168 ns        76725
BM_VectorTestConstArraySizeFloat/30       8756 ns         8717 ns        81178
BM_VectorTestConstArraySizeFloat/31      10075 ns        10021 ns        70355
BM_VectorTestConstArraySizeFloat/32       8014 ns         7975 ns        88308
BM_VectorTestForcedIntrinsics/1            187 ns          187 ns      3693366
BM_VectorTestForcedIntrinsics/2           1164 ns         1160 ns       590960
BM_VectorTestForcedIntrinsics/3           1637 ns         1631 ns       429460
BM_VectorTestForcedIntrinsics/4           1199 ns         1195 ns       588012
BM_VectorTestForcedIntrinsics/5           1874 ns         1867 ns       375261
BM_VectorTestForcedIntrinsics/6           1961 ns         1954 ns       355218
BM_VectorTestForcedIntrinsics/7           2756 ns         2746 ns       254439
BM_VectorTestForcedIntrinsics/8           2062 ns         2054 ns       342008
BM_VectorTestForcedIntrinsics/9           3154 ns         3142 ns       222200
BM_VectorTestForcedIntrinsics/10          3113 ns         3102 ns       225867
BM_VectorTestForcedIntrinsics/11          3986 ns         3972 ns       176529
BM_VectorTestForcedIntrinsics/12          3089 ns         3078 ns       227638
BM_VectorTestForcedIntrinsics/13          4434 ns         4418 ns       159927
BM_VectorTestForcedIntrinsics/14          4222 ns         4207 ns       166875
BM_VectorTestForcedIntrinsics/15          5165 ns         5146 ns       100000
BM_VectorTestForcedIntrinsics/16          4040 ns         4025 ns       170974
BM_VectorTestForcedIntrinsics/17          5581 ns         5561 ns       120296
BM_VectorTestForcedIntrinsics/18          5324 ns         5304 ns       126241
BM_VectorTestForcedIntrinsics/19          6398 ns         6375 ns       112255
BM_VectorTestForcedIntrinsics/20          5053 ns         5034 ns       100000
BM_VectorTestForcedIntrinsics/21          6826 ns         6801 ns       104789
BM_VectorTestForcedIntrinsics/22          6473 ns         6450 ns       108229
BM_VectorTestForcedIntrinsics/23          7596 ns         7569 ns        94341
BM_VectorTestForcedIntrinsics/24          6017 ns         5995 ns       117525
BM_VectorTestForcedIntrinsics/25          7955 ns         7927 ns        88942
BM_VectorTestForcedIntrinsics/26          7634 ns         7607 ns        93720
BM_VectorTestForcedIntrinsics/27          8809 ns         8778 ns        80646
BM_VectorTestForcedIntrinsics/28          7065 ns         7042 ns       100188
BM_VectorTestForcedIntrinsics/29          8922 ns         8888 ns        79557
BM_VectorTestForcedIntrinsics/30          8885 ns         8852 ns        81284
BM_VectorTestForcedIntrinsics/31          9986 ns         9943 ns        71552
BM_VectorTestForcedIntrinsics/32          8039 ns         8006 ns        88152

Pixel 6 Pro (1/29/2025)
------------------------------------------------------------------------------
Benchmark                                    Time             CPU   Iterations
------------------------------------------------------------------------------
BM_VectorTestLoopFloat/1                  1502 ns         1497 ns       459325
BM_VectorTestLoopFloat/2                  2388 ns         2380 ns       294160
BM_VectorTestLoopFloat/3                  3396 ns         3384 ns       208241
BM_VectorTestLoopFloat/4                  4514 ns         4498 ns       156121
BM_VectorTestLoopFloat/5                  5655 ns         5635 ns       124000
BM_VectorTestLoopFloat/6                  6800 ns         6776 ns       102712
BM_VectorTestLoopFloat/7                  7940 ns         7913 ns        88857
BM_VectorTestLoopFloat/8                  9058 ns         9028 ns        77529
BM_VectorTestLoopFloat/9                 10134 ns        10099 ns        69007
BM_VectorTestLoopFloat/10                11333 ns        11294 ns        62029
BM_VectorTestLoopFloat/11                12591 ns        12547 ns        56158
BM_VectorTestLoopFloat/12                13722 ns        13675 ns        51178
BM_VectorTestLoopFloat/13                14841 ns        14789 ns        47634
BM_VectorTestLoopFloat/14                16006 ns        15949 ns        44030
BM_VectorTestLoopFloat/15                17215 ns        17153 ns        41298
BM_VectorTestLoopFloat/16                18856 ns        18788 ns        35997
BM_VectorTestLoopFloat/17                20116 ns        20053 ns        35029
BM_VectorTestLoopFloat/18                21270 ns        21164 ns        32944
BM_VectorTestLoopFloat/19                22445 ns        22367 ns        31383
BM_VectorTestLoopFloat/20                23789 ns        23710 ns        29843
BM_VectorTestLoopFloat/21                25588 ns        25512 ns        27590
BM_VectorTestLoopFloat/22                26861 ns        26775 ns        25990
BM_VectorTestLoopFloat/23                27747 ns        27657 ns        25161
BM_VectorTestLoopFloat/24                29162 ns        29065 ns        24219
BM_VectorTestLoopFloat/25                30699 ns        30601 ns        22785
BM_VectorTestLoopFloat/26                31996 ns        31895 ns        21649
BM_VectorTestLoopFloat/27                33028 ns        32912 ns        21183
BM_VectorTestLoopFloat/28                34514 ns        34397 ns        20432
BM_VectorTestLoopFloat/29                35519 ns        35399 ns        19882
BM_VectorTestLoopFloat/30                36609 ns        36461 ns        19010
BM_VectorTestLoopFloat/31                37636 ns        37489 ns        18647
BM_VectorTestLoopFloat/32                39001 ns        38856 ns        18033
BM_VectorTestConstArraySizeFloat/1        1157 ns         1153 ns       600452
BM_VectorTestConstArraySizeFloat/2        1577 ns         1572 ns       440151
BM_VectorTestConstArraySizeFloat/3        2191 ns         2183 ns       321242
BM_VectorTestConstArraySizeFloat/4        3114 ns         3104 ns       223419
BM_VectorTestConstArraySizeFloat/5        3747 ns         3734 ns       187164
BM_VectorTestConstArraySizeFloat/6        4727 ns         4710 ns       148569
BM_VectorTestConstArraySizeFloat/7        5364 ns         5346 ns       131033
BM_VectorTestConstArraySizeFloat/8        6152 ns         6130 ns       113657
BM_VectorTestConstArraySizeFloat/9        7119 ns         7094 ns        99311
BM_VectorTestConstArraySizeFloat/10       7684 ns         7658 ns        91576
BM_VectorTestConstArraySizeFloat/11       8500 ns         8471 ns        82382
BM_VectorTestConstArraySizeFloat/12       9238 ns         9208 ns        75741
BM_VectorTestConstArraySizeFloat/13       9991 ns         9956 ns        70762
BM_VectorTestConstArraySizeFloat/14      10734 ns        10697 ns        65570
BM_VectorTestConstArraySizeFloat/15      11559 ns        11520 ns        60706
BM_VectorTestConstArraySizeFloat/16      12265 ns        12224 ns        57061
BM_VectorTestConstArraySizeFloat/17      13131 ns        13087 ns        53368
BM_VectorTestConstArraySizeFloat/18      13829 ns        13780 ns        50118
BM_VectorTestConstArraySizeFloat/19      15628 ns        15575 ns        44809
BM_VectorTestConstArraySizeFloat/20      17416 ns        17356 ns        40377
BM_VectorTestConstArraySizeFloat/21      20812 ns        20740 ns        33750
BM_VectorTestConstArraySizeFloat/22      23752 ns        23670 ns        29564
BM_VectorTestConstArraySizeFloat/23      25015 ns        24928 ns        27897
BM_VectorTestConstArraySizeFloat/24      27675 ns        27580 ns        25290
BM_VectorTestConstArraySizeFloat/25      28924 ns        28822 ns        24389
BM_VectorTestConstArraySizeFloat/26      29192 ns        29089 ns        24013
BM_VectorTestConstArraySizeFloat/27      31591 ns        31466 ns        22179
BM_VectorTestConstArraySizeFloat/28      33425 ns        33269 ns        21025
BM_VectorTestConstArraySizeFloat/29      34175 ns        34046 ns        20647
BM_VectorTestConstArraySizeFloat/30      36181 ns        36038 ns        19575
BM_VectorTestConstArraySizeFloat/31      37082 ns        36920 ns        18803
BM_VectorTestConstArraySizeFloat/32      36747 ns        36553 ns        19246
BM_VectorTestForcedIntrinsics/1           1170 ns         1166 ns       591357
BM_VectorTestForcedIntrinsics/2           1186 ns         1182 ns       598724
BM_VectorTestForcedIntrinsics/3           1685 ns         1679 ns       420630
BM_VectorTestForcedIntrinsics/4           1235 ns         1230 ns       572354
BM_VectorTestForcedIntrinsics/5           1880 ns         1873 ns       370863
BM_VectorTestForcedIntrinsics/6           1970 ns         1963 ns       360011
BM_VectorTestForcedIntrinsics/7           2781 ns         2771 ns       252813
BM_VectorTestForcedIntrinsics/8           2067 ns         2059 ns       342035
BM_VectorTestForcedIntrinsics/9           3212 ns         3200 ns       218060
BM_VectorTestForcedIntrinsics/10          3194 ns         3183 ns       220057
BM_VectorTestForcedIntrinsics/11          4074 ns         4060 ns       172551
BM_VectorTestForcedIntrinsics/12          3161 ns         3150 ns       220341
BM_VectorTestForcedIntrinsics/13          4460 ns         4443 ns       157565
BM_VectorTestForcedIntrinsics/14          4321 ns         4305 ns       162640
BM_VectorTestForcedIntrinsics/15          5091 ns         5072 ns       100000
BM_VectorTestForcedIntrinsics/16          4149 ns         4134 ns       169154
BM_VectorTestForcedIntrinsics/17          5723 ns         5702 ns       122228
BM_VectorTestForcedIntrinsics/18          5430 ns         5411 ns       129557
BM_VectorTestForcedIntrinsics/19          6516 ns         6492 ns       108324
BM_VectorTestForcedIntrinsics/20          5219 ns         5201 ns       139971
BM_VectorTestForcedIntrinsics/21          6929 ns         6905 ns       101141
BM_VectorTestForcedIntrinsics/22          6445 ns         6426 ns       109573
BM_VectorTestForcedIntrinsics/23          7772 ns         7747 ns        90492
BM_VectorTestForcedIntrinsics/24          6119 ns         6099 ns       115904
BM_VectorTestForcedIntrinsics/25          8135 ns         8111 ns        80751
BM_VectorTestForcedIntrinsics/26          7698 ns         7674 ns        86225
BM_VectorTestForcedIntrinsics/27          9014 ns         8982 ns        77138
BM_VectorTestForcedIntrinsics/28          7267 ns         7242 ns        96210
BM_VectorTestForcedIntrinsics/29          9368 ns         9336 ns        74626
BM_VectorTestForcedIntrinsics/30          9016 ns         8980 ns        78822
BM_VectorTestForcedIntrinsics/31         10224 ns        10185 ns        69073
BM_VectorTestForcedIntrinsics/32          8325 ns         8287 ns        84343

*/

// A small subset of code from audio_utils/intrinsic_utils.h

// We conditionally include neon optimizations for ARM devices
#pragma push_macro("USE_NEON")
#undef USE_NEON

#if defined(__ARM_NEON__) || defined(__aarch64__)
#include <arm_neon.h>
#define USE_NEON
#endif

template <typename T>
inline constexpr bool dependent_false_v = false;

// Type of array embedded in a struct that is usable in the Neon template functions below.
// This type must satisfy std::is_array_v<>.
template<typename T, size_t N>
struct internal_array_t {
    T v[N];
    static constexpr size_t size() { return N; }
};

#ifdef USE_NEON

template<int N>
struct vfloat_struct {};

template<int N>
using vfloat_t = typename vfloat_struct<N>::t;  // typnemae required for Android 14 and earlier.

template<typename F, int N>
using vector_hw_t = std::conditional_t<
        std::is_same_v<F, float>, vfloat_t<N>, internal_array_t<F, N>>;

// Recursively define the NEON types required for a given vector size.
// intrinsic_utils.h allows structurally recursive type definitions based on
// pairs of types (much like Lisp list cons pairs).
template<>
struct vfloat_struct<1> { using t = float; };
template<>
struct vfloat_struct<2> { using t = float32x2_t; };
template<>
struct vfloat_struct<3> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<2> a; vfloat_t<1> b; } s; }; };
template<>
struct vfloat_struct<4> { using t = float32x4_t; };
template<>
struct vfloat_struct<5> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<4> a; vfloat_t<1> b; } s; }; };
template<>
struct vfloat_struct<6> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<4> a; vfloat_t<2> b; } s; }; };
template<>
struct vfloat_struct<7> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<4> a; vfloat_t<3> b; } s; }; };
template<>
struct vfloat_struct<8> { using t = float32x4x2_t; };
template<>
struct vfloat_struct<9> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<8> a; vfloat_t<1> b; } s; }; };
template<>
struct vfloat_struct<10> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<8> a; vfloat_t<2> b; } s; }; };
template<>
struct vfloat_struct<11> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<8> a; vfloat_t<3> b; } s; }; };
template<>
struct vfloat_struct<12> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<8> a; vfloat_t<4> b; } s; }; };
template<>
struct vfloat_struct<13> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<8> a; vfloat_t<5> b; } s; }; };
template<>
struct vfloat_struct<14> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<8> a; vfloat_t<6> b; } s; }; };
template<>
struct vfloat_struct<15> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<8> a; vfloat_t<7> b; } s; }; };
template<>
struct vfloat_struct<16> { using t = float32x4x4_t; };
template<>
struct vfloat_struct<17> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<1> b; } s; }; };
template<>
struct vfloat_struct<18> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<2> b; } s; }; };
template<>
struct vfloat_struct<19> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<3> b; } s; }; };
template<>
struct vfloat_struct<20> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<4> b; } s; }; };
template<>
struct vfloat_struct<21> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<5> b; } s; }; };
template<>
struct vfloat_struct<22> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<6> b; } s; }; };
template<>
struct vfloat_struct<23> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<7> b; } s; }; };
template<>
struct vfloat_struct<24> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<8> b; } s; }; };
template<>
struct vfloat_struct<25> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<9> b; } s; }; };
template<>
struct vfloat_struct<26> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<10> b; } s; }; };
template<>
struct vfloat_struct<27> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<11> b; } s; }; };
template<>
struct vfloat_struct<28> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<12> b; } s; }; };
template<>
struct vfloat_struct<29> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<13> b; } s; }; };
template<>
struct vfloat_struct<30> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<14> b; } s; }; };
template<>
struct vfloat_struct<31> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<15> b; } s; }; };
template<>
struct vfloat_struct<32> { using t = struct { struct __attribute__((packed)) {
    vfloat_t<16> a; vfloat_t<16> b; } s; }; };

#else

// use loop vectorization if no HW type exists.
template<typename F, int N>
using vector_hw_t = internal_array_t<F, N>;

#endif

template<typename T>
static inline T vmul(T a, T b) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return a * b;

#ifdef USE_NEON
    } else if constexpr (std::is_same_v<T, float32x2_t>) {
        return vmul_f32(a, b);
    } else if constexpr (std::is_same_v<T, float32x4_t>) {
        return vmulq_f32(a, b);
#if defined(__aarch64__)
    } else if constexpr (std::is_same_v<T, float64x2_t>) {
        return vmulq_f64(a, b);
#endif
#endif // USE_NEON

    } else /* constexpr */ {
        T ret;
        auto &[retval] = ret;  // single-member struct
        const auto &[aval] = a;
        const auto &[bval] = b;
        if constexpr (std::is_array_v<decltype(retval)>) {
#pragma unroll
            for (size_t i = 0; i < std::size(aval); ++i) {
                retval[i] = vmul(aval[i], bval[i]);
            }
            return ret;
        } else /* constexpr */ {
             auto &[r1, r2] = retval;
             const auto &[a1, a2] = aval;
             const auto &[b1, b2] = bval;
             r1 = vmul(a1, b1);
             r2 = vmul(a2, b2);
             return ret;
        }
    }
}

#pragma pop_macro("USE_NEON")

// end intrinsics subset

static constexpr size_t kDataSize = 2048;

static void TestArgs(benchmark::internal::Benchmark* b) {
    constexpr int kChannelCountMin = 1;
    constexpr int kChannelCountMax = 32;
    for (int i = kChannelCountMin; i <= kChannelCountMax; ++i) {
        b->Args({i});
    }
}

// Macro to instantiate switch case statements.

#define INSTANTIATE(N) \
    case N: \
    mFunc = [](F* out, const F* in1, const F* in2, size_t count) { \
        constexpr size_t vsize = sizeof(V<F, N>); \
        static_assert(vsize == N * sizeof(F)); \
        static_assert(alignof(V<F, N>) <= (vsize & -vsize)); \
        for (size_t i = 0; i < count; ++i) { \
            *reinterpret_cast<V<F, N>*>(out) = vmul( \
                    *reinterpret_cast<const V<F, N>*>(in1), \
                    *reinterpret_cast<const V<F, N>*>(in2)); \
            out += N; \
            in1 += N; \
            in2 += N; \
            } \
        }; \
    break;

template <template <typename, int> class V, typename F>
class Processor {
public:
    Processor(int channelCount, bool loop = false)
        : mChannelCount(channelCount) {
        if (loop) {
            mFunc = [channelCount](F* out, const F* in1, const F* in2, size_t count) {
                for (size_t i = 0; i < count; ++i) {
                    for (size_t j = 0; j < channelCount; ++j) {
                        *reinterpret_cast<V<F, 1>*>(out) = vmul(
                        *reinterpret_cast<const V<F, 1>*>(in1),
                        *reinterpret_cast<const V<F, 1>*>(in2));
                        ++out;
                        ++in1;
                        ++in2;
                    }
                }
            };
            return;
        }
        switch (channelCount) {
        INSTANTIATE(1);
        INSTANTIATE(2);
        INSTANTIATE(3);
        INSTANTIATE(4);
        INSTANTIATE(5);
        INSTANTIATE(6);
        INSTANTIATE(7);
        INSTANTIATE(8);
        INSTANTIATE(9);
        INSTANTIATE(10);
        INSTANTIATE(11);
        INSTANTIATE(12);
        INSTANTIATE(13);
        INSTANTIATE(14);
        INSTANTIATE(15);
        INSTANTIATE(16);
        INSTANTIATE(17);
        INSTANTIATE(18);
        INSTANTIATE(19);
        INSTANTIATE(20);
        INSTANTIATE(21);
        INSTANTIATE(22);
        INSTANTIATE(23);
        INSTANTIATE(24);
        INSTANTIATE(25);
        INSTANTIATE(26);
        INSTANTIATE(27);
        INSTANTIATE(28);
        INSTANTIATE(29);
        INSTANTIATE(30);
        INSTANTIATE(31);
        INSTANTIATE(32);
        }
    }

    void process(F* out, const F* in1, const F* in2, size_t frames) {
        mFunc(out, in1, in2, frames);
    }

    const size_t mChannelCount;
    /* const */ std::function<void(F*, const F*, const F*, size_t)> mFunc;
};

template <template <typename, int> class V, typename F>
static void BM_VectorTest(benchmark::State& state, bool loop) {
    const size_t channelCount = state.range(0);

    std::vector<F> input1(kDataSize * channelCount);
    std::vector<F> input2(kDataSize * channelCount);
    std::vector<F> output(kDataSize * channelCount);

    // Initialize input buffer and coefs with deterministic pseudo-random values
    std::minstd_rand gen(42);
    const F amplitude = 1.;
    std::uniform_real_distribution<> dis(-amplitude, amplitude);
    for (auto& in : input1) {
        in = dis(gen);
    }
    for (auto& in : input2) {
        in = dis(gen);
    }

    Processor<V, float> processor(channelCount, loop);

    // Run the test
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(input1.data());
        benchmark::DoNotOptimize(input2.data());
        benchmark::DoNotOptimize(output.data());
        processor.process(output.data(), input1.data(), input2.data(), kDataSize);
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(channelCount);
}

// Test using two loops.
static void BM_VectorTestLoopFloat(benchmark::State& state) {
    BM_VectorTest<internal_array_t, float>(state, true /* loop */);
}

// Test using two loops, the inner loop is constexpr size.
static void BM_VectorTestConstArraySizeFloat(benchmark::State& state) {
    BM_VectorTest<internal_array_t, float>(state, false) /* loop */;
}

// Test using intrinsics, if available.
static void BM_VectorTestForcedIntrinsics(benchmark::State& state) {
    BM_VectorTest<vector_hw_t, float>(state, false /* loop */);
}

BENCHMARK(BM_VectorTestLoopFloat)->Apply(TestArgs);

BENCHMARK(BM_VectorTestConstArraySizeFloat)->Apply(TestArgs);

BENCHMARK(BM_VectorTestForcedIntrinsics)->Apply(TestArgs);

BENCHMARK_MAIN();
