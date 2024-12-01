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

#include <string>
#include <vector>

namespace android::audio_utils::stringutils {

// C++ String Utilities
//
// Original file extracted from frameworks/av/services/mediametrics

/**
 * Return string tokens from iterator, separated by spaces and reserved chars.
 */
std::string tokenizer(std::string::const_iterator& it,
        const std::string::const_iterator& end, const char* reserved);

/**
 * Splits flags string based on delimiters (or, whitespace which is removed).
 */
std::vector<std::string> split(const std::string& flags, const char* delim);


/**
 * Parses a vector of integers using ',' '{' and '}' as delimiters. Leaves
 * vector unmodified if the parsing fails.
 */
bool parseVector(const std::string& str, std::vector<int32_t>* vector);

/**
 * Returns a vector of device address pairs from the devices string.
 *
 * A failure to parse returns early with the contents that were able to be parsed.
 */
std::vector<std::pair<std::string, std::string>>
getDeviceAddressPairs(const std::string& devices);

/**
 * For purposes of field naming and logging, we have common formats:
 *
 * Lower camel case: Often used for variables or method names.
 *                   "helloWorld" "toString()"
 *
 * Upper camel case: Often used for classes or structs.
 *                   "HelloWorld" "MyClass"
 *
 * Lower snake case: Often used for variable names or method names.
 *                   "hello_world" "to_string()"
 *
 * Upper snake case: Often used for MACRO names or constants.
 *                   "HELLO_WORLD" "TO_STRING()"
 */
enum class NameFormat {
    kFormatLowerCamelCase,  // Example: helloWorld
    kFormatUpperCamelCase,  // Example: HelloWorld
    kFormatLowerSnakeCase,  // Example: hello_world
    kFormatUpperSnakeCase,  // Example: HELLO_WORLD
};

/**
 * Returns a string with the name tokens converted to a particular format.
 *
 * changeNameFormat("hello_world", NameFormat::kFormatLowerCamelCase) -> "helloWorld"
 *
 * This is used for consistent logging, where the log convention may differ from
 * the string/stringify convention of the name.
 *
 * The following rules are used:
 *
 * 1) A name consists of one or more concatenated words, connected by a case change,
 *    a '_', or a switch between number to alpha sequence.
 *
 * 2) A '_', a number, or a lower to upper case transition will count as a new word.
 *    A number sequence counts as a word.
 *
 * 3) A non alphanumeric character (such as '.') signifies a new name follows
 *    and is copied through. For example, "helloWorld.toString".
 *
 * 4) Conversion of multiple numeric fields separated by '_' will preserve the underscore
 *    to avoid confusion.  As an example:
 *    changeNameFormat("alpha_10_100", NameFormat::kFormatUpperCamelCase)
 *            -> "Alpha10_100" (not Alpha10100)
 *
 * 5) When the target format is upper or lower snake case, attempt to preserve underscores.
 */
std::string changeNameFormat(const std::string& name, NameFormat format);

inline std::string toLowerCamelCase(const std::string& name) {
    return changeNameFormat(name, NameFormat::kFormatLowerCamelCase);
}

inline std::string toUpperCamelCase(const std::string& name) {
    return changeNameFormat(name, NameFormat::kFormatUpperCamelCase);
}

inline std::string toLowerSnakeCase(const std::string& name) {
    return changeNameFormat(name, NameFormat::kFormatLowerSnakeCase);
}

inline std::string toUpperSnakeCase(const std::string& name) {
    return changeNameFormat(name, NameFormat::kFormatUpperSnakeCase);
}

/**
 * Appends a suffix string, with replacement of a character.
 *
 * \param s string to append suffix to.
 * \param suffix string suffix.
 * \param from target character to replace in suffix.
 * \param to character to replace with.
 */
inline void appendWithReplacement(std::string& s, const std::string& suffix, char from, char to) {
    std::transform(suffix.begin(), suffix.end(), std::back_inserter(s),
                   [from, to](char in) { return in == from ? to : in; });
}

} // android::audio_utils::stringutils
