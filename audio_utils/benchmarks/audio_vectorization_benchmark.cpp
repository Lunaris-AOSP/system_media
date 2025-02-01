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
BM_VectorTestLoopFloat/1                  1216 ns         1212 ns       580560
BM_VectorTestLoopFloat/2                  2272 ns         2264 ns       309745
BM_VectorTestLoopFloat/3                  3366 ns         3354 ns       209391
BM_VectorTestLoopFloat/4                  4495 ns         4478 ns       157291
BM_VectorTestLoopFloat/5                  5660 ns         5627 ns       124649
BM_VectorTestLoopFloat/6                  6776 ns         6750 ns       104102
BM_VectorTestLoopFloat/7                  7942 ns         7916 ns        89257
BM_VectorTestLoopFloat/8                  9120 ns         9086 ns        77234
BM_VectorTestLoopFloat/9                 10252 ns        10212 ns        69253
BM_VectorTestLoopFloat/10                11475 ns        11432 ns        61646
BM_VectorTestLoopFloat/11                12704 ns        12658 ns        55493
BM_VectorTestLoopFloat/12                13864 ns        13812 ns        50944
BM_VectorTestLoopFloat/13                15024 ns        14967 ns        47169
BM_VectorTestLoopFloat/14                16340 ns        16282 ns        43531
BM_VectorTestLoopFloat/15                17422 ns        17356 ns        40328
BM_VectorTestLoopFloat/16                18680 ns        18609 ns        37820
BM_VectorTestLoopFloat/17                19892 ns        19819 ns        35348
BM_VectorTestLoopFloat/18                21099 ns        21015 ns        33253
BM_VectorTestLoopFloat/19                22238 ns        22154 ns        31681
BM_VectorTestLoopFloat/20                23551 ns        23433 ns        29829
BM_VectorTestLoopFloat/21                24707 ns        24612 ns        28525
BM_VectorTestLoopFloat/22                26041 ns        25916 ns        27004
BM_VectorTestLoopFloat/23                27236 ns        27122 ns        25123
BM_VectorTestLoopFloat/24                28535 ns        28409 ns        24505
BM_VectorTestLoopFloat/25                29715 ns        29542 ns        23744
BM_VectorTestLoopFloat/26                31163 ns        31002 ns        22640
BM_VectorTestLoopFloat/27                32259 ns        32065 ns        21859
BM_VectorTestLoopFloat/28                33580 ns        33391 ns        20702
BM_VectorTestLoopFloat/29                34891 ns        34699 ns        20281
BM_VectorTestLoopFloat/30                36242 ns        36007 ns        19400
BM_VectorTestLoopFloat/31                37423 ns        37154 ns        18875
BM_VectorTestLoopFloat/32                38858 ns        38608 ns        17699
BM_VectorTestConstArraySizeFloat/1         185 ns          184 ns      3771794
BM_VectorTestConstArraySizeFloat/2         663 ns          660 ns      1068518
BM_VectorTestConstArraySizeFloat/3        2159 ns         2152 ns       318170
BM_VectorTestConstArraySizeFloat/4        3919 ns         3905 ns       179267
BM_VectorTestConstArraySizeFloat/5        1861 ns         1854 ns       374407
BM_VectorTestConstArraySizeFloat/6        1964 ns         1956 ns       362563
BM_VectorTestConstArraySizeFloat/7        2789 ns         2779 ns       252684
BM_VectorTestConstArraySizeFloat/8        2070 ns         2062 ns       342189
BM_VectorTestConstArraySizeFloat/9        3191 ns         3179 ns       220216
BM_VectorTestConstArraySizeFloat/10       3128 ns         3117 ns       225340
BM_VectorTestConstArraySizeFloat/11       4049 ns         4025 ns       174288
BM_VectorTestConstArraySizeFloat/12       3124 ns         3106 ns       225711
BM_VectorTestConstArraySizeFloat/13       4440 ns         4424 ns       158540
BM_VectorTestConstArraySizeFloat/14       4276 ns         4256 ns       164144
BM_VectorTestConstArraySizeFloat/15       5325 ns         5306 ns       132282
BM_VectorTestConstArraySizeFloat/16       4091 ns         4072 ns       172111
BM_VectorTestConstArraySizeFloat/17       5711 ns         5682 ns       122226
BM_VectorTestConstArraySizeFloat/18       5373 ns         5349 ns       129827
BM_VectorTestConstArraySizeFloat/19       6500 ns         6474 ns       108150
BM_VectorTestConstArraySizeFloat/20       5131 ns         5109 ns       136649
BM_VectorTestConstArraySizeFloat/21       6896 ns         6867 ns        99598
BM_VectorTestConstArraySizeFloat/22       6579 ns         6529 ns       108221
BM_VectorTestConstArraySizeFloat/23       7752 ns         7705 ns        91673
BM_VectorTestConstArraySizeFloat/24       6129 ns         6102 ns       114269
BM_VectorTestConstArraySizeFloat/25       8151 ns         8120 ns        85643
BM_VectorTestConstArraySizeFloat/26       7512 ns         7474 ns        94708
BM_VectorTestConstArraySizeFloat/27       9100 ns         9047 ns        79200
BM_VectorTestConstArraySizeFloat/28       7191 ns         7149 ns        97121
BM_VectorTestConstArraySizeFloat/29       9417 ns         9362 ns        74720
BM_VectorTestConstArraySizeFloat/30       8952 ns         8893 ns        80378
BM_VectorTestConstArraySizeFloat/31      10342 ns        10284 ns        66481
BM_VectorTestConstArraySizeFloat/32       8189 ns         8132 ns        85186
BM_VectorTestForcedIntrinsics/1            189 ns          189 ns      3629410
BM_VectorTestForcedIntrinsics/2           1192 ns         1188 ns       572025
BM_VectorTestForcedIntrinsics/3           1701 ns         1695 ns       412319
BM_VectorTestForcedIntrinsics/4           1234 ns         1229 ns       563105
BM_VectorTestForcedIntrinsics/5           1936 ns         1929 ns       367124
BM_VectorTestForcedIntrinsics/6           2002 ns         1994 ns       350985
BM_VectorTestForcedIntrinsics/7           2826 ns         2814 ns       247821
BM_VectorTestForcedIntrinsics/8           2106 ns         2098 ns       332577
BM_VectorTestForcedIntrinsics/9           3240 ns         3229 ns       216567
BM_VectorTestForcedIntrinsics/10          3176 ns         3164 ns       219614
BM_VectorTestForcedIntrinsics/11          4086 ns         4065 ns       173103
BM_VectorTestForcedIntrinsics/12          3095 ns         3083 ns       226427
BM_VectorTestForcedIntrinsics/13          4459 ns         4441 ns       157019
BM_VectorTestForcedIntrinsics/14          4298 ns         4281 ns       162819
BM_VectorTestForcedIntrinsics/15          5232 ns         5211 ns       130653
BM_VectorTestForcedIntrinsics/16          4166 ns         4150 ns       168336
BM_VectorTestForcedIntrinsics/17          5713 ns         5687 ns       122828
BM_VectorTestForcedIntrinsics/18          5424 ns         5403 ns       131831
BM_VectorTestForcedIntrinsics/19          6517 ns         6487 ns       107246
BM_VectorTestForcedIntrinsics/20          5208 ns         5179 ns       135608
BM_VectorTestForcedIntrinsics/21          6927 ns         6882 ns       101059
BM_VectorTestForcedIntrinsics/22          6593 ns         6542 ns       108036
BM_VectorTestForcedIntrinsics/23          7789 ns         7745 ns        90793
BM_VectorTestForcedIntrinsics/24          6241 ns         6200 ns       113967
BM_VectorTestForcedIntrinsics/25          8178 ns         8130 ns        84883
BM_VectorTestForcedIntrinsics/26          7768 ns         7724 ns        91931
BM_VectorTestForcedIntrinsics/27          9017 ns         8954 ns        78657
BM_VectorTestForcedIntrinsics/28          7250 ns         7206 ns        98287
BM_VectorTestForcedIntrinsics/29          9419 ns         9365 ns        74588
BM_VectorTestForcedIntrinsics/30          8943 ns         8885 ns        77512
BM_VectorTestForcedIntrinsics/31         10217 ns        10159 ns        69207
BM_VectorTestForcedIntrinsics/32          8271 ns         8221 ns        86206

Pixel 6 Pro (1/29/2025)
------------------------------------------------------------------------------
Benchmark                                    Time             CPU   Iterations
------------------------------------------------------------------------------
BM_VectorTestLoopFloat/1                  1522 ns         1514 ns       459906
BM_VectorTestLoopFloat/2                  2391 ns         2383 ns       293707
BM_VectorTestLoopFloat/3                  3437 ns         3426 ns       205663
BM_VectorTestLoopFloat/4                  4482 ns         4468 ns       157406
BM_VectorTestLoopFloat/5                  5665 ns         5645 ns       125564
BM_VectorTestLoopFloat/6                  6784 ns         6762 ns       105112
BM_VectorTestLoopFloat/7                  7930 ns         7902 ns        89104
BM_VectorTestLoopFloat/8                  9043 ns         9011 ns        77654
BM_VectorTestLoopFloat/9                 10178 ns        10145 ns        68967
BM_VectorTestLoopFloat/10                11338 ns        11296 ns        61958
BM_VectorTestLoopFloat/11                12500 ns        12456 ns        56104
BM_VectorTestLoopFloat/12                13686 ns        13634 ns        51361
BM_VectorTestLoopFloat/13                14794 ns        14744 ns        47477
BM_VectorTestLoopFloat/14                16040 ns        15979 ns        43158
BM_VectorTestLoopFloat/15                17098 ns        17036 ns        40926
BM_VectorTestLoopFloat/16                18413 ns        18343 ns        37962
BM_VectorTestLoopFloat/17                19462 ns        19382 ns        36093
BM_VectorTestLoopFloat/18                20788 ns        20704 ns        33897
BM_VectorTestLoopFloat/19                22168 ns        21967 ns        31994
BM_VectorTestLoopFloat/20                23420 ns        23322 ns        30136
BM_VectorTestLoopFloat/21                24424 ns        24316 ns        28773
BM_VectorTestLoopFloat/22                25789 ns        25686 ns        27195
BM_VectorTestLoopFloat/23                26980 ns        26870 ns        25939
BM_VectorTestLoopFloat/24                28349 ns        28238 ns        24906
BM_VectorTestLoopFloat/25                29486 ns        29355 ns        23815
BM_VectorTestLoopFloat/26                30686 ns        30554 ns        22853
BM_VectorTestLoopFloat/27                31781 ns        31630 ns        22034
BM_VectorTestLoopFloat/28                33161 ns        33008 ns        21133
BM_VectorTestLoopFloat/29                34482 ns        34329 ns        20290
BM_VectorTestLoopFloat/30                35676 ns        35531 ns        19434
BM_VectorTestLoopFloat/31                37037 ns        36835 ns        19033
BM_VectorTestLoopFloat/32                38379 ns        38178 ns        18409
BM_VectorTestConstArraySizeFloat/1        1138 ns         1134 ns       605601
BM_VectorTestConstArraySizeFloat/2        1551 ns         1546 ns       451139
BM_VectorTestConstArraySizeFloat/3        2157 ns         2149 ns       326085
BM_VectorTestConstArraySizeFloat/4        3082 ns         3070 ns       228235
BM_VectorTestConstArraySizeFloat/5        3694 ns         3668 ns       191253
BM_VectorTestConstArraySizeFloat/6        4708 ns         4691 ns       149290
BM_VectorTestConstArraySizeFloat/7        5255 ns         5236 ns       133227
BM_VectorTestConstArraySizeFloat/8        6239 ns         6217 ns       115033
BM_VectorTestConstArraySizeFloat/9        7087 ns         7058 ns        99388
BM_VectorTestConstArraySizeFloat/10       7640 ns         7613 ns        91195
BM_VectorTestConstArraySizeFloat/11       8471 ns         8438 ns        83724
BM_VectorTestConstArraySizeFloat/12       9132 ns         9101 ns        77836
BM_VectorTestConstArraySizeFloat/13       9963 ns         9928 ns        71043
BM_VectorTestConstArraySizeFloat/14      10601 ns        10565 ns        67362
BM_VectorTestConstArraySizeFloat/15      11428 ns        11384 ns        61646
BM_VectorTestConstArraySizeFloat/16      12061 ns        12017 ns        58708
BM_VectorTestConstArraySizeFloat/17      13094 ns        13043 ns        53478
BM_VectorTestConstArraySizeFloat/18      13624 ns        13553 ns        52138
BM_VectorTestConstArraySizeFloat/19      15633 ns        15541 ns        45464
BM_VectorTestConstArraySizeFloat/20      17379 ns        17299 ns        40665
BM_VectorTestConstArraySizeFloat/21      20772 ns        20675 ns        34104
BM_VectorTestConstArraySizeFloat/22      23613 ns        23485 ns        29856
BM_VectorTestConstArraySizeFloat/23      24967 ns        24800 ns        28081
BM_VectorTestConstArraySizeFloat/24      27395 ns        27278 ns        25481
BM_VectorTestConstArraySizeFloat/25      28858 ns        28701 ns        24520
BM_VectorTestConstArraySizeFloat/26      29251 ns        29068 ns        24195
BM_VectorTestConstArraySizeFloat/27      31487 ns        31293 ns        22507
BM_VectorTestConstArraySizeFloat/28      33355 ns        33137 ns        20929
BM_VectorTestConstArraySizeFloat/29      34385 ns        34229 ns        20417
BM_VectorTestConstArraySizeFloat/30      36031 ns        35811 ns        19543
BM_VectorTestConstArraySizeFloat/31      37079 ns        36905 ns        19051
BM_VectorTestConstArraySizeFloat/32      36857 ns        36715 ns        19077
BM_VectorTestForcedIntrinsics/1           1163 ns         1159 ns       598027
BM_VectorTestForcedIntrinsics/2           1175 ns         1170 ns       599275
BM_VectorTestForcedIntrinsics/3           1680 ns         1673 ns       419149
BM_VectorTestForcedIntrinsics/4           1210 ns         1205 ns       581791
BM_VectorTestForcedIntrinsics/5           1874 ns         1867 ns       374320
BM_VectorTestForcedIntrinsics/6           1954 ns         1946 ns       364700
BM_VectorTestForcedIntrinsics/7           2763 ns         2753 ns       253086
BM_VectorTestForcedIntrinsics/8           2057 ns         2049 ns       347318
BM_VectorTestForcedIntrinsics/9           3186 ns         3175 ns       218684
BM_VectorTestForcedIntrinsics/10          3112 ns         3101 ns       225780
BM_VectorTestForcedIntrinsics/11          4044 ns         4023 ns       175125
BM_VectorTestForcedIntrinsics/12          3088 ns         3077 ns       229106
BM_VectorTestForcedIntrinsics/13          4405 ns         4388 ns       159480
BM_VectorTestForcedIntrinsics/14          4248 ns         4232 ns       164753
BM_VectorTestForcedIntrinsics/15          5018 ns         4983 ns       140497
BM_VectorTestForcedIntrinsics/16          4131 ns         4095 ns       172113
BM_VectorTestForcedIntrinsics/17          5714 ns         5679 ns       123282
BM_VectorTestForcedIntrinsics/18          5387 ns         5358 ns       132204
BM_VectorTestForcedIntrinsics/19          6515 ns         6481 ns       110209
BM_VectorTestForcedIntrinsics/20          5108 ns         5081 ns       100000
BM_VectorTestForcedIntrinsics/21          6913 ns         6876 ns       101935
BM_VectorTestForcedIntrinsics/22          6564 ns         6517 ns       108434
BM_VectorTestForcedIntrinsics/23          7763 ns         7718 ns        92602
BM_VectorTestForcedIntrinsics/24          6184 ns         6132 ns       115958
BM_VectorTestForcedIntrinsics/25          8152 ns         8099 ns        87568
BM_VectorTestForcedIntrinsics/26          7720 ns         7674 ns        93561
BM_VectorTestForcedIntrinsics/27          8977 ns         8919 ns        78819
BM_VectorTestForcedIntrinsics/28          7206 ns         7153 ns        99046
BM_VectorTestForcedIntrinsics/29          9373 ns         9310 ns        74948
BM_VectorTestForcedIntrinsics/30          8888 ns         8830 ns        79500
BM_VectorTestForcedIntrinsics/31         10233 ns        10163 ns        70094
BM_VectorTestForcedIntrinsics/32          8209 ns         8139 ns        84943

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

// Macro test operator

#define OPERATOR(N) \
    *reinterpret_cast<V<F, N>*>(out) = vmul( \
    *reinterpret_cast<const V<F, N>*>(in1), \
    *reinterpret_cast<const V<F, N>*>(in2)); \
    out += N; \
    in1 += N; \
    in2 += N;

// Macro to instantiate switch case statements.

#define INSTANTIATE(N) \
    case N: \
    mFunc = [](F* out, const F* in1, const F* in2, size_t count) { \
        static_assert(sizeof(V<F, N>) == N * sizeof(F)); \
        for (size_t i = 0; i < count; ++i) { \
            OPERATOR(N); \
        } \
    }; \
    break;

template <typename Traits>
class Processor {
public:
    // shorthand aliases
    using F = typename Traits::data_t;
    template <typename T, int N>
    using V = typename Traits::template container_t<T, N>;

    Processor(int channelCount)
        : mChannelCount(channelCount) {

        if constexpr (Traits::loop_) {
            mFunc = [channelCount](F* out, const F* in1, const F* in2, size_t count) {
                for (size_t i = 0; i < count; ++i) {
                    for (size_t j = 0; j < channelCount; ++j) {
                        OPERATOR(1);
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

template <typename Traits>
static void BM_VectorTest(benchmark::State& state) {
    using F = typename Traits::data_t;
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

    Processor<Traits> processor(channelCount);

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

// Clang has an issue with -frelaxed-template-template-args where
// it may not follow the C++17 guidelines.  Use a traits struct to
// pass in parameters.

// Test using two loops.
struct LoopFloatTraits {
    template <typename F, int N>
    using container_t = internal_array_t<F, N>;
    using data_t = float;
    static constexpr bool loop_ = true;
};
static void BM_VectorTestLoopFloat(benchmark::State& state) {
    BM_VectorTest<LoopFloatTraits>(state);
}

// Test using two loops, the inner loop is constexpr size.
struct ConstArraySizeFloatTraits {
    template <typename F, int N>
    using container_t = internal_array_t<F, N>;
    using data_t = float;
    static constexpr bool loop_ = false;
};
static void BM_VectorTestConstArraySizeFloat(benchmark::State& state) {
    BM_VectorTest<ConstArraySizeFloatTraits>(state);
}

// Test using intrinsics, if available.
struct ForcedIntrinsicsTraits {
    template <typename F, int N>
    using container_t = vector_hw_t<F, N>;
    using data_t = float;
    static constexpr bool loop_ = false;
};
static void BM_VectorTestForcedIntrinsics(benchmark::State& state) {
    BM_VectorTest<ForcedIntrinsicsTraits>(state);
}

BENCHMARK(BM_VectorTestLoopFloat)->Apply(TestArgs);

BENCHMARK(BM_VectorTestConstArraySizeFloat)->Apply(TestArgs);

BENCHMARK(BM_VectorTestForcedIntrinsics)->Apply(TestArgs);

BENCHMARK_MAIN();
