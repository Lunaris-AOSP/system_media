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

#include <audio_utils/RunRemote.h>
#include <gtest/gtest.h>
#include <memory>

static void WorkerThread(android::audio_utils::RunRemote& runRemote) {
    while (true) {
        const int c = runRemote.getc();
        switch (c) {
            case 'a':
                runRemote.putc('a');  // send ack
                break;
            case 'b':
                runRemote.putc('b');
                break;
            default:
                runRemote.putc('x');
                break;
        }
    }
}

TEST(RunRemote, basic) {
    auto remoteWorker = std::make_shared<android::audio_utils::RunRemote>(WorkerThread);
    remoteWorker->run();

    remoteWorker->putc('a');
    EXPECT_EQ('a', remoteWorker->getc());

    remoteWorker->putc('b');
    EXPECT_EQ('b', remoteWorker->getc());

    remoteWorker->putc('c');
    EXPECT_EQ('x', remoteWorker->getc());

    remoteWorker->stop();
    EXPECT_EQ(-1, remoteWorker->getc());  // remote closed
}
