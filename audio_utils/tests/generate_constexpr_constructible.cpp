/*
 * Copyright (C) 2025 The Android Open Source Project
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

#include <iostream>

// Generates constexpr code for intrinsic_utils.h
//
// To dump the constexpr code to stdout:
//
// $ clang++ -std=c++2a generate_constexpr_constructible.cpp
// $ ./a.out
//

using namespace std;

int main() {
  const std::string indent("    ");
  constexpr size_t kElements = 32;
  // vapply
  for (size_t i = kElements; i >= 1; --i) {
    cout << indent << indent;
    if (i != kElements) cout << "} else ";
    cout << "if constexpr (is_braces_constructible<VT,\n";
    for (size_t j = 0; j < i; ) {
      cout << indent << indent << indent << indent;
      for (size_t k = 0; k < 8 && j < i; ++k, ++j) {
        if (k > 0) cout << " ";
        cout << "any_type";
        if (j < i - 1) {
          cout << ",";
        } else {
          if (k == 7) {
            cout << "\n" << indent << indent << indent << indent;
          }
          cout << ">()) {";
        }
      }
      cout << "\n";
    }
    for (size_t j = 0; j < i; ) {
      cout << indent << indent << indent;
      if (j == 0) {
        cout << "auto& [";
      } else {
        cout << indent << indent;
      }
      for (size_t k = 0; k < 8 && j < i; ++k, ++j) {
        if (k > 0) cout << " ";
        cout << "v" << (j + 1) <<  (j < i - 1 ? "," : "] = vv;");
      }
      cout << "\n";
    }

    for (size_t j = 0; j < i; ++j) {
      cout << indent << indent << indent;
      cout << "vapply(f, v" << (j + 1) << ");\n";
    }
  }
  cout << indent << indent;
  cout << "} else {\n";
  cout << indent << indent << indent;
  cout << "static_assert(false, \"Currently supports up to "
      << kElements << " members only.\");\n";
  cout << indent << indent;
  cout << "}\n";
  return 0;
}
