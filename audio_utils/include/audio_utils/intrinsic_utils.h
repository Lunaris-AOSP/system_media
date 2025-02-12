/*
 * Copyright 2020 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_UTILS_INTRINSIC_UTILS_H
#define ANDROID_AUDIO_UTILS_INTRINSIC_UTILS_H

#include <array>  // std::size
#include <type_traits>
#include "template_utils.h"

/*
  The intrinsics utility library contain helper functions for wide width DSP support.
  We use templated types to allow testing from scalar to vector values.

  See the Eigen project for general abstracted linear algebra acceleration.
  http://eigen.tuxfamily.org/
*/

// We conditionally include neon optimizations for ARM devices
#pragma push_macro("USE_NEON")
#undef USE_NEON

#if defined(__ARM_NEON__) || defined(__aarch64__)
#include <arm_neon.h>
#define USE_NEON
#endif

namespace android::audio_utils::intrinsics {

// For static assert(false) we need a template version to avoid early failure.
// See: https://stackoverflow.com/questions/51523965/template-dependent-false
template <typename T>
inline constexpr bool dependent_false_v = false;

// Detect if the value is directly addressable as an array.
// This is more advanced than std::is_array and works with neon intrinsics.
template<typename T>
concept is_array_like = requires(T a) {
    a[0];  // can index first element
};

/**
 * Applies a functional or a constant to an intrinsic struct.
 *
 * The vapply method has no return value, but can modify an input intrinsic struct
 * through element-wise application of a functional.
 * Compare the behavior with veval which returns a struct result.
 *
 * Using vector terminology:
 *   if f is a constant: v[i] = f;
 *   if f is a void method that takes an element value: f(v[i]);
 *   if f returns an element value but takes no arg: v[i] = f();
 *   if f returns an element value but takes an element value: v[i] = f(v[i]);
 */
template <typename V, typename F>
constexpr void vapply(const F& f, V& v) {
    if constexpr (std::is_same_v<V, float> || std::is_same_v<V, double>) {
        using E = std::decay_t<decltype(v)>;
        if constexpr (std::is_invocable_r_v<void, F, E>) {
            f(v);
        } else if constexpr (std::is_invocable_r_v<E, F, E>) {
            v = f(v);
        } else if constexpr (std::is_invocable_r_v<E, F>) {
            v = f();
        } else /* constexpr */ {
            v = f;
        }
    } else if constexpr (is_array_like<V>) {
        // this vector access within a neon object prevents constexpr.
        using E = std::decay_t<decltype(v[0])>;
#pragma unroll
        for (size_t i = 0; i < sizeof(v) / sizeof(v[0]); ++i) {
            if constexpr (std::is_invocable_r_v<void, F, E>) {
                f(v[i]);
            } else if constexpr (std::is_invocable_r_v<E, F, E>) {
                v[i] = f(v[i]);
            } else if constexpr (std::is_invocable_r_v<E, F>) {
                v[i] = f();
            } else /* constexpr */ {
                v[i] = f;
            }
        }
    } else /* constexpr */ {
        auto& [vv] = v;
        // for constexpr purposes, non-const references can't bind to array elements.
        using VT = decltype(vv);
        // automatically generated from tests/generate_constexpr_constructible.cpp
        if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type
                >()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22, v23, v24,
                    v25, v26, v27, v28, v29, v30, v31, v32] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
            vapply(f, v23);
            vapply(f, v24);
            vapply(f, v25);
            vapply(f, v26);
            vapply(f, v27);
            vapply(f, v28);
            vapply(f, v29);
            vapply(f, v30);
            vapply(f, v31);
            vapply(f, v32);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22, v23, v24,
                    v25, v26, v27, v28, v29, v30, v31] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
            vapply(f, v23);
            vapply(f, v24);
            vapply(f, v25);
            vapply(f, v26);
            vapply(f, v27);
            vapply(f, v28);
            vapply(f, v29);
            vapply(f, v30);
            vapply(f, v31);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22, v23, v24,
                    v25, v26, v27, v28, v29, v30] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
            vapply(f, v23);
            vapply(f, v24);
            vapply(f, v25);
            vapply(f, v26);
            vapply(f, v27);
            vapply(f, v28);
            vapply(f, v29);
            vapply(f, v30);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22, v23, v24,
                    v25, v26, v27, v28, v29] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
            vapply(f, v23);
            vapply(f, v24);
            vapply(f, v25);
            vapply(f, v26);
            vapply(f, v27);
            vapply(f, v28);
            vapply(f, v29);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22, v23, v24,
                    v25, v26, v27, v28] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
            vapply(f, v23);
            vapply(f, v24);
            vapply(f, v25);
            vapply(f, v26);
            vapply(f, v27);
            vapply(f, v28);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22, v23, v24,
                    v25, v26, v27] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
            vapply(f, v23);
            vapply(f, v24);
            vapply(f, v25);
            vapply(f, v26);
            vapply(f, v27);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22, v23, v24,
                    v25, v26] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
            vapply(f, v23);
            vapply(f, v24);
            vapply(f, v25);
            vapply(f, v26);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22, v23, v24,
                    v25] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
            vapply(f, v23);
            vapply(f, v24);
            vapply(f, v25);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type
                >()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22, v23, v24] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
            vapply(f, v23);
            vapply(f, v24);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22, v23] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
            vapply(f, v23);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21, v22] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
            vapply(f, v22);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20, v21] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
            vapply(f, v21);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19, v20] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
            vapply(f, v20);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18, v19] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
            vapply(f, v19);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17, v18] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
            vapply(f, v18);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16,
                    v17] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
            vapply(f, v17);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type
                >()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15, v16] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
            vapply(f, v16);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14, v15] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
            vapply(f, v15);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13, v14] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
            vapply(f, v14);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12, v13] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
            vapply(f, v13);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11, v12] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
            vapply(f, v12);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10, v11] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
            vapply(f, v11);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9, v10] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
            vapply(f, v10);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type,
                any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8,
                    v9] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
            vapply(f, v9);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type
                >()) {
            auto& [v1, v2, v3, v4, v5, v6, v7, v8] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
            vapply(f, v8);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6, v7] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
            vapply(f, v7);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5, v6] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
            vapply(f, v6);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4, v5] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
            vapply(f, v5);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type, any_type>()) {
            auto& [v1, v2, v3, v4] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
            vapply(f, v4);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type, any_type>()) {
            auto& [v1, v2, v3] = vv;
            vapply(f, v1);
            vapply(f, v2);
            vapply(f, v3);
        } else if constexpr (is_braces_constructible<VT,
                any_type, any_type>()) {
            auto& [v1, v2] = vv;
            vapply(f, v1);
            vapply(f, v2);
        } else if constexpr (is_braces_constructible<VT,
                any_type>()) {
            auto& [v1] = vv;
            vapply(f, v1);
        } else {
            static_assert(false, "Currently supports up to 32 members only.");
        }
    }
}

// Type of array embedded in a struct that is usable in the Neon template functions below.
// This type must satisfy std::is_array_v<>.
template<typename T, size_t N>
struct internal_array_t {
    T v[N];
    static constexpr size_t size() { return N; }
    constexpr bool operator==(const internal_array_t<T, N> other) const {
        for (size_t i = 0; i < N; ++i) {
            if (v[i] != other.v[i]) return false;
        }
        return true;
    }
    constexpr internal_array_t<T, N>& operator=(T value) {
        for (size_t i = 0; i < N; ++i) {
            v[i] = value;
        }
        return *this;
    }
    constexpr internal_array_t() = default;
    // explicit: disallow internal_array_t<float, 3> x  = 10.f;
    constexpr explicit internal_array_t(T value) {
        *this = value;
    }
    // allow internal_array_t<float, 3> x  = { 10.f };
    constexpr internal_array_t(std::initializer_list<T> value) {
        size_t i = 0;
        auto vptr = value.begin();
        for (; i < std::min(N, value.size()); ++i) {
            v[i] = *vptr++;
        }
        for (; i < N; ++i) {
            v[i] = {};
        }
    }
};

// assert our structs are trivially copyable so we can use memcpy freely.
static_assert(std::is_trivially_copyable_v<internal_array_t<float, 31>>);
static_assert(std::is_trivially_copyable_v<internal_array_t<double, 31>>);

// Vector convert between type T to type S.
template <typename S, typename T>
constexpr inline S vconvert(const T& in) {
    S out;

    if constexpr (is_array_like<S>) {
        if constexpr (is_array_like<T>) {
#pragma unroll
            // neon intrinsics need sizeof.
            for (size_t i = 0; i < sizeof(in) / sizeof(in[0]); ++i) {
                out[i] = in[i];
            }
        } else { /* constexpr */
            const auto& [inv] = in;
#pragma unroll
            for (size_t i = 0; i < T::size(); ++i) {
                out[i] = inv[i];
            }
        }
    } else { /* constexpr */
        auto& [outv] = out;
        if constexpr (is_array_like<T>) {
#pragma unroll
            // neon intrinsics need sizeof.
            for (size_t i = 0; i < sizeof(in) / sizeof(in[0]); ++i) {
                outv[i] = in[i];
            }
        } else { /* constexpr */
            const auto& [inv] = in;
#pragma unroll
            for (size_t i = 0; i < T::size(); ++i) {
                outv[i] = inv[i];
            }
        }
    }
    return out;
}

/*
  Generalized template functions for the Neon instruction set.

  See here for some general comments from ARM.
  https://developer.arm.com/documentation/dht0004/a/neon-support-in-compilation-tools/automatic-vectorization/floating-point-vectorization

  Notes:
  1) We provide scalar equivalents which are compilable even on non-ARM processors.
  2) We use recursive calls to decompose array types, e.g. float32x4x4_t -> float32x4_t
  3) NEON double SIMD acceleration is only available on 64 bit architectures.
     On Pixel 3XL, NEON double x 2 SIMD is actually slightly slower than the FP unit.

  We create a generic Neon acceleration to be applied to a composite type.

  The type follows the following compositional rules for simplicity:
      1) must be a primitive floating point type.
      2) must be a NEON data type.
      3) must be a struct with one member, either
           a) an array of types 1-3.
           b) a cons-pair struct of 2 possibly different members of types 1-3.

  Examples of possible struct definitions:
  using alternative_2_t = struct { struct { float a; float b; } s; };
  using alternative_9_t = struct { struct { float32x4x2_t a; float b; } s; };
  using alternative_15_t = struct { struct { float32x4x2_t a; struct { float v[7]; } b; } s; };
*/

/**
 * Returns the first element of the intrinsic struct.
 */
template <typename T>
constexpr auto first_element_of(const T& t) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return t;
    } else if constexpr (is_array_like<T>) {
        return first_element_of(t[0]);
    } else /* constexpr */ {
        const auto& [tval] = t;  // single-member struct
        if constexpr (std::is_array_v<decltype(tval)>) {
            return first_element_of(tval[0]);
        } else /* constexpr */ {
             const auto& [p1, p2] = tval;
             return first_element_of(p1);
        }
    }
}

/**
 * Evaluate f(v1 [, v2 [, v3]]) and return an intrinsic struct result.
 *
 * The veval method returns the vector result by element-wise
 * evaulating a functional f to one or more intrinsic struct inputs.
 * Compare this method with the single argument vapply,
 * which can modify a single struct argument in-place.
 */
template <typename F, typename V>
constexpr V veval(const F& f, const V& v1) {
    if constexpr (std::is_same_v<V, float> || std::is_same_v<V, double>) {
        return f(v1);
    } else if constexpr (is_array_like<V>) {
        V out;
#pragma unroll
        // neon intrinsics need sizeof.
        for (size_t i = 0; i < sizeof(v1) / sizeof(v1[0]); ++i) {
            out[i] = f(v1[i]);
        }
        return out;
    } else /* constexpr */ {
        V ret;
        auto& [retval] = ret;  // single-member struct
        const auto& [v1val] = v1;
        if constexpr (std::is_array_v<decltype(v1val)>) {
#pragma unroll
            for (size_t i = 0; i < std::size(v1val); ++i) {
                retval[i] = veval(f, v1val[i]);
            }
            return ret;
        } else /* constexpr */ {
             auto& [r1, r2] = retval;
             const auto& [p1, p2] = v1val;
             r1 = veval(f, p1);
             r2 = veval(f, p2);
             return ret;
        }
    }
}

template <typename F, typename V>
constexpr V veval(const F& f, const V& v1, const V& v2) {
    if constexpr (std::is_same_v<V, float> || std::is_same_v<V, double>) {
        return f(v1, v2);
    } else if constexpr (is_array_like<V>) {
        V out;
#pragma unroll
        // neon intrinsics need sizeof.
        for (size_t i = 0; i < sizeof(v1) / sizeof(v1[0]); ++i) {
            out[i] = f(v1[i], v2[i]);
        }
        return out;
    } else /* constexpr */ {
        V ret;
        auto& [retval] = ret;  // single-member struct
        const auto& [v1val] = v1;
        const auto& [v2val] = v2;
        if constexpr (std::is_array_v<decltype(v1val)>) {
#pragma unroll
            for (size_t i = 0; i < std::size(v1val); ++i) {
                retval[i] = veval(f, v1val[i], v2val[i]);
            }
            return ret;
        } else /* constexpr */ {
             auto& [r1, r2] = retval;
             const auto& [p11, p12] = v1val;
             const auto& [p21, p22] = v2val;
             r1 = veval(f, p11, p21);
             r2 = veval(f, p12, p22);
             return ret;
        }
    }
}

template <typename F, typename V>
constexpr V veval(const F& f, const V& v1, const V& v2, const V& v3) {
    if constexpr (std::is_same_v<V, float> || std::is_same_v<V, double>) {
        return f(v1, v2, v3);
    } else if constexpr (is_array_like<V>) {
        V out;
#pragma unroll
        // neon intrinsics need sizeof.
        for (size_t i = 0; i < sizeof(v1) / sizeof(v1[0]); ++i) {
            out[i] = f(v1[i], v2[i], v3[i]);
        }
        return out;
    } else /* constexpr */ {
        V ret;
        auto& [retval] = ret;  // single-member struct
        const auto& [v1val] = v1;
        const auto& [v2val] = v2;
        const auto& [v3val] = v3;
        if constexpr (std::is_array_v<decltype(v1val)>) {
#pragma unroll
            for (size_t i = 0; i < std::size(v1val); ++i) {
                retval[i] = veval(f, v1val[i], v2val[i], v3val[i]);
            }
            return ret;
        } else /* constexpr */ {
             auto& [r1, r2] = retval;
             const auto& [p11, p12] = v1val;
             const auto& [p21, p22] = v2val;
             const auto& [p31, p32] = v3val;
             r1 = veval(f, p11, p21, p31);
             r2 = veval(f, p12, p22, p32);
             return ret;
        }
    }
}

// --------------------------------------------------------------------

// add a + b
template<typename T>
static inline T vadd(T a, T b) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return a + b;

#ifdef USE_NEON
    } else if constexpr (std::is_same_v<T, float32x2_t>) {
        return vadd_f32(a, b);
    } else if constexpr (std::is_same_v<T, float32x4_t>) {
        return vaddq_f32(a, b);
#if defined(__aarch64__)
    } else if constexpr (std::is_same_v<T, float64x2_t>) {
        return vaddq_f64(a, b);
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
                retval[i] = vadd(aval[i], bval[i]);
            }
            return ret;
        } else /* constexpr */ {
             auto &[r1, r2] = retval;
             const auto &[a1, a2] = aval;
             const auto &[b1, b2] = bval;
             r1 = vadd(a1, b1);
             r2 = vadd(a2, b2);
             return ret;
        }
    }
}

// add internally
template<typename T>
inline auto vaddv(const T& a) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return a;

#ifdef USE_NEON
    } else if constexpr (std::is_same_v<T, float32x2_t>) {
        return vaddv_f32(a);
#if defined(__aarch64__)
    } else if constexpr (std::is_same_v<T, float32x4_t>) {
        return vaddvq_f32(a);
    } else if constexpr (std::is_same_v<T, float64x2_t>) {
        return vaddvq_f64(a);
#endif
#endif // USE_NEON
    } else if constexpr (is_array_like<T>) {
        using ret_t = std::decay_t<decltype(a[0])>;

        ret_t ret{};
        // array_like is not the same as an array, so we use sizeof here
        // to handle neon instrinsics.
#pragma unroll
        for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i) {
            ret += a[i];
        }
        return ret;
    } else /* constexpr */ {
        const auto &[aval] = a;
        using ret_t = std::decay_t<decltype(aval[0])>;
        ret_t ret{};

#pragma unroll
        for (size_t i = 0; i < std::size(aval); ++i) {
            ret += aval[i];
        }
        return ret;
    }
}

// duplicate float into all elements.
template<typename T, typename F>
static inline T vdupn(F f) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return f;

#ifdef USE_NEON
    } else if constexpr (std::is_same_v<T, float32x2_t>) {
        return vdup_n_f32(f);
    } else if constexpr (std::is_same_v<T, float32x4_t>) {
        return vdupq_n_f32(f);
#if defined(__aarch64__)
    } else if constexpr (std::is_same_v<T, float64x2_t>) {
        return vdupq_n_f64(f);
#endif
#endif // USE_NEON

    } else /* constexpr */ {
        T ret;
        auto &[retval] = ret;  // single-member struct
        if constexpr (std::is_array_v<decltype(retval)>) {
#pragma unroll
            for (auto& val : retval) {
                val = vdupn<std::decay_t<decltype(val)>>(f);
            }
            return ret;
        } else /* constexpr */ {
             auto &[r1, r2] = retval;
             using r1_type = std::decay_t<decltype(r1)>;
             using r2_type = std::decay_t<decltype(r2)>;
             r1 = vdupn<r1_type>(f);
             r2 = vdupn<r2_type>(f);
             return ret;
        }
    }
}

// load from float pointer.
template<typename T, typename F>
static inline T vld1(const F *f) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return *f;

#ifdef USE_NEON
    } else if constexpr (std::is_same_v<T, float32x2_t>) {
        return vld1_f32(f);
    } else if constexpr (std::is_same_v<T, float32x4_t>) {
        return vld1q_f32(f);
#if defined(__aarch64__)
    } else if constexpr (std::is_same_v<T, float64x2_t>) {
        return vld1q_f64(f);
#endif
#endif // USE_NEON

    } else /* constexpr */ {
        T ret;
        auto &[retval] = ret;  // single-member struct
        if constexpr (std::is_array_v<decltype(retval)>) {
            using element_type = std::decay_t<decltype(retval[0])>;
            constexpr size_t subelements = sizeof(element_type) / sizeof(F);
#pragma unroll
            for (size_t i = 0; i < std::size(retval); ++i) {
                retval[i] = vld1<element_type>(f);
                f += subelements;
            }
            return ret;
        } else /* constexpr */ {
             auto &[r1, r2] = retval;
             using r1_type = std::decay_t<decltype(r1)>;
             using r2_type = std::decay_t<decltype(r2)>;
             r1 = vld1<r1_type>(f);
             f += sizeof(r1) / sizeof(F);
             r2 = vld1<r2_type>(f);
             return ret;
        }
    }
}

/**
 * Returns c as follows:
 * c_i = a_i * b_i if a and b are the same vector type or
 * c_i = a_i * b if a is a vector and b is scalar or
 * c_i = a * b_i if a is scalar and b is a vector.
 */
template<typename T, typename S, typename F>
static inline T vmla(T a, S b, F c) {
    // Both types T and S are non-primitive and they are not equal.  T == S handled below.
    (void) a;
    (void) b;
    (void) c;
    static_assert(dependent_false_v<T>);
}

template<typename T, typename F>
static inline T vmla(T a, T b, F c) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        if constexpr (std::is_same_v<F, float> || std::is_same_v<F, double>) {
            return a + b * c;
        } else {
            static_assert(dependent_false_v<T>);
        }
    } else if constexpr (std::is_same_v<F, float> || std::is_same_v<F, double>) {
        // handle the lane variant
#ifdef USE_NEON
        if constexpr (std::is_same_v<T, float32x2_t>) {
            return vmla_n_f32(a, b, c);
        } else if constexpr (std::is_same_v<T, float32x4_t>) {
            return vmlaq_n_f32(a, b,c);
#if defined(__aarch64__)
        } else if constexpr (std::is_same_v<T, float64x2_t>) {
            return vmlaq_n_f64(a, b);
#endif
        } else
#endif // USE_NEON
        {
        T ret;
        auto &[retval] = ret;  // single-member struct
        const auto &[aval] = a;
        const auto &[bval] = b;
        if constexpr (std::is_array_v<decltype(retval)>) {
#pragma unroll
            for (size_t i = 0; i < std::size(aval); ++i) {
                retval[i] = vmla(aval[i], bval[i], c);
            }
            return ret;
        } else /* constexpr */ {
             auto &[r1, r2] = retval;
             const auto &[a1, a2] = aval;
             const auto &[b1, b2] = bval;
             r1 = vmla(a1, b1, c);
             r2 = vmla(a2, b2, c);
             return ret;
        }
        }
    } else {
        // Both types T and F are non-primitive and they are not equal.
        static_assert(dependent_false_v<T>);
    }
}

template<typename T, typename F>
static inline T vmla(T a, F b, T c) {
    return vmla(a, c, b);
}

// fused multiply-add a + b * c
template<typename T>
inline T vmla(const T& a, const T& b, const T& c) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return a + b * c;

#ifdef USE_NEON
    } else if constexpr (std::is_same_v<T, float32x2_t>) {
        return vmla_f32(a, b, c);
    } else if constexpr (std::is_same_v<T, float32x4_t>) {
        return vmlaq_f32(a, b, c);
#if defined(__aarch64__)
    } else if constexpr (std::is_same_v<T, float64x2_t>) {
        return vmlaq_f64(a, b, c);
#endif
#endif // USE_NEON

    } else /* constexpr */ {
        T ret;
        auto &[retval] = ret;  // single-member struct
        const auto &[aval] = a;
        const auto &[bval] = b;
        const auto &[cval] = c;
        if constexpr (std::is_array_v<decltype(retval)>) {
#pragma unroll
            for (size_t i = 0; i < std::size(aval); ++i) {
                retval[i] = vmla(aval[i], bval[i], cval[i]);
            }
            return ret;
        } else /* constexpr */ {
             auto &[r1, r2] = retval;
             const auto &[a1, a2] = aval;
             const auto &[b1, b2] = bval;
             const auto &[c1, c2] = cval;
             r1 = vmla(a1, b1, c1);
             r2 = vmla(a2, b2, c2);
             return ret;
        }
    }
}

/**
 * Returns c as follows:
 * c_i = a_i * b_i if a and b are the same vector type or
 * c_i = a_i * b if a is a vector and b is scalar or
 * c_i = a * b_i if a is scalar and b is a vector.
 */
template<typename T, typename F>
static inline auto vmul(T a, F b) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        if constexpr (std::is_same_v<F, float> || std::is_same_v<F, double>) {
            return a * b;
        } else /* constexpr */ {
            return vmul(b, a); // we prefer T to be the vector/struct form.
        }
    } else if constexpr (std::is_same_v<F, float> || std::is_same_v<F, double>) {
        // handle the lane variant
#ifdef USE_NEON
        if constexpr (std::is_same_v<T, float32x2_t>) {
            return vmul_n_f32(a, b);
        } else if constexpr (std::is_same_v<T, float32x4_t>) {
            return vmulq_n_f32(a, b);
#if defined(__aarch64__)
        } else if constexpr (std::is_same_v<T, float64x2_t>) {
            return vmulq_n_f64(a, b);
#endif
        } else
#endif // USE_NEON
        {
        T ret;
        auto &[retval] = ret;  // single-member struct
        const auto &[aval] = a;
        if constexpr (std::is_array_v<decltype(retval)>) {
#pragma unroll
            for (size_t i = 0; i < std::size(aval); ++i) {
                retval[i] = vmul(aval[i], b);
            }
            return ret;
        } else /* constexpr */ {
             auto &[r1, r2] = retval;
             const auto &[a1, a2] = aval;
             r1 = vmul(a1, b);
             r2 = vmul(a2, b);
             return ret;
        }
        }
    } else {
        // Both types T and F are non-primitive and they are not equal.
        static_assert(dependent_false_v<T>);
    }
}

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

// negate
template<typename T>
static inline T vneg(T f) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return -f;

#ifdef USE_NEON
    } else if constexpr (std::is_same_v<T, float32x2_t>) {
        return vneg_f32(f);
    } else if constexpr (std::is_same_v<T, float32x4_t>) {
        return vnegq_f32(f);
#if defined(__aarch64__)
    } else if constexpr (std::is_same_v<T, float64x2_t>) {
        return vnegq_f64(f);
#endif
#endif // USE_NEON

    } else /* constexpr */ {
        T ret;
        auto &[retval] = ret;  // single-member struct
        const auto &[fval] = f;
        if constexpr (std::is_array_v<decltype(retval)>) {
#pragma unroll
            for (size_t i = 0; i < std::size(fval); ++i) {
                retval[i] = vneg(fval[i]);
            }
            return ret;
        } else /* constexpr */ {
             auto &[r1, r2] = retval;
             const auto &[f1, f2] = fval;
             r1 = vneg(f1);
             r2 = vneg(f2);
             return ret;
        }
    }
}

// store to float pointer.
template<typename T, typename F>
static inline void vst1(F *f, T a) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        *f = a;

#ifdef USE_NEON
    } else if constexpr (std::is_same_v<T, float32x2_t>) {
        return vst1_f32(f, a);
    } else if constexpr (std::is_same_v<T, float32x4_t>) {
        return vst1q_f32(f, a);
#if defined(__aarch64__)
    } else if constexpr (std::is_same_v<T, float64x2_t>) {
        return vst1q_f64(f, a);
#endif
#endif // USE_NEON

    } else /* constexpr */ {
        const auto &[aval] = a;
        if constexpr (std::is_array_v<decltype(aval)>) {
            constexpr size_t subelements = sizeof(std::decay_t<decltype(aval[0])>) / sizeof(F);
#pragma unroll
            for (size_t i = 0; i < std::size(aval); ++i) {
                vst1(f, aval[i]);
                f += subelements;
            }
        } else /* constexpr */ {
             const auto &[a1, a2] = aval;
             vst1(f, a1);
             f += sizeof(std::decay_t<decltype(a1)>) / sizeof(F);
             vst1(f, a2);
        }
    }
}

// subtract a - b
template<typename T>
static inline T vsub(T a, T b) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return a - b;

#ifdef USE_NEON
    } else if constexpr (std::is_same_v<T, float32x2_t>) {
        return vsub_f32(a, b);
    } else if constexpr (std::is_same_v<T, float32x4_t>) {
        return vsubq_f32(a, b);
#if defined(__aarch64__)
    } else if constexpr (std::is_same_v<T, float64x2_t>) {
        return vsubq_f64(a, b);
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
                retval[i] = vsub(aval[i], bval[i]);
            }
            return ret;
        } else /* constexpr */ {
             auto &[r1, r2] = retval;
             const auto &[a1, a2] = aval;
             const auto &[b1, b2] = bval;
             r1 = vsub(a1, b1);
             r2 = vsub(a2, b2);
             return ret;
        }
    }
}

} // namespace android::audio_utils::intrinsics

#pragma pop_macro("USE_NEON")

#endif // !ANDROID_AUDIO_UTILS_INTRINSIC_UTILS_H
