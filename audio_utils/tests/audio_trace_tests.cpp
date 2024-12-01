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

#include <audio_utils/Trace.h>

#include <gtest/gtest.h>

TEST(trace_object, basic) {
    android::audio_utils::trace::Object obj;

    EXPECT_TRUE(obj.empty());
    obj.set("one", 10);
    EXPECT_FALSE(obj.empty());

    auto trace = obj.toTrace();

    EXPECT_NE(trace.find("one"), std::string::npos);
    EXPECT_NE(trace.find("10"), std::string::npos);
    EXPECT_EQ("{ one=10 }", trace);

    obj.set("two", "twenty");
    auto trace2 = obj.toTrace();
    EXPECT_NE(trace2.find("one"), std::string::npos);
    EXPECT_NE(trace2.find("10"), std::string::npos);
    EXPECT_NE(trace2.find("two"), std::string::npos);
    EXPECT_NE(trace2.find("twenty"), std::string::npos);
    EXPECT_EQ("{ one=10 two=\"twenty\" }", trace2);

    obj.clear();
    EXPECT_TRUE(obj.empty());
    EXPECT_EQ("{  }", obj.toTrace());
}
