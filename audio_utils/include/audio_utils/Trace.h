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

#pragma once

#include "StringUtils.h"
#include "TraceConstants.h"

#include <string>
#include <type_traits>

namespace android::audio_utils::trace {

/*
 * Audio Tracing.
 *
 * We use an "Object" metadata formatter to ensure consistent
 * behavior.  The Object formatter is not thread-safe, so
 * locking must be provided by the caller.
 *
 * Object:
 * This is a C++ object encapsulating key, value pairs.
 *
 * Native                              Java equivalent
 * int32                               (int)
 * int64                               (long)
 * float                               (float)
 * double                              (double)
 * std::string                         (String)
 *
 * TBD: Array definition.
 * TBD: Recursive definition.
 *
 * The Object may be dumped in text form (used for ATRACE)
 * using the method toTrace().
 *
 * The canonical Object format will have all key, value pairs sorted
 * by key, with no duplicate keys.
 *
 * For practical use, we relax the sorting requirement by allowing
 * "new" keys to be appended to the end.
 *
 * To service such requirements (and to add JSON, XML based
 * output) requires an auxiliary map<> data structure, which
 * is heavier weight.
 *
 * Currently the object supports a set() but no get().
 *
 * TODO(b/377400056): Add JSON output formatting.
 * TODO(b/377400056): Add XML output formatting.
 * TODO(b/377400056): Enforce sorted output.
 * TODO(b/377400056): Select trailing commas.
 * TODO(b/377400056): Enable sorted output.
 * TODO(b/377400056): Allow key conversion between camel case to snake case.
 * TODO(b/377400056): Escape string delimiter token from strings.
 * TODO(b/377400056): Consider nested objects, or strings that contain {}.
 */
class Object {
public:
    /**
     * Add a numeric value to the Object.
     *
     * @param key name to us.
     * @param value an arithmetic value.
     * @return Object for use in fluent style.
     */
    template <typename S, typename T>
    requires std::is_convertible_v<S, std::string> && std::is_arithmetic_v<T>
    Object& set(const S& key, const T& value) {
        if (!object_.empty()) object_.append(object_delimiter_token_);
        object_.append(key).append(assign_token_).append(std::to_string(value));
        return *this;
    }

    /**
     * Add a string value to the Object.
     *
     * @param key name to us.
     * @param value a string convertible value.
     * @return Object for use in fluent style.
     */
    template <typename S, typename T>
    requires std::is_convertible_v<S, std::string> && std::is_convertible_v<T, std::string>
    Object& set(const S& key, const T& value) {
        if (!object_.empty()) object_.append(object_delimiter_token_);
        object_.append(key).append(assign_token_).append(string_begin_token_);
        // ATRACE does not like '|', so replace with '+'.
        stringutils::appendWithReplacement(object_, value, '|', '+');
        object_.append(string_end_token_);
        return *this;
    }

    /**
     * Returns true if the Object is empty (nothing is recorded).
     */
    bool empty() const { return object_.empty(); }

    /**
     * Clears the contents of the object.
     */
    void clear() { object_.clear(); }

    /**
     * Returns a text-formatted string suitable for ATRACE.
     */
    template <typename S>
    requires std::is_convertible_v<S, std::string>
    std::string toTrace(const S& tag) const {
        std::string ret(tag);
        ret.append(object_begin_token_).append(object_).append(object_end_token_);
        return ret;
    }

    std::string toTrace() const {
        return toTrace("");
    }

protected:
    // Make these configurable  (ATRACE text definition)
    static constexpr char assign_token_[] = "=";
    static constexpr char object_begin_token_[] = "{ ";
    static constexpr char object_end_token_[] = " }";
    static constexpr char object_delimiter_token_[] = " ";
    static constexpr char string_begin_token_[] = "\"";
    static constexpr char string_end_token_[] = "\"";

    std::string object_;
};

} // namespace android::audio_utils::trace
