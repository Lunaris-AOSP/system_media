/*
 * Copyright 2018 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_FD_TO_STRING_H
#define ANDROID_AUDIO_FD_TO_STRING_H

#include <android-base/unique_fd.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <utils/Timers.h>
#include <chrono>
#include <future>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

#include "clock.h"

namespace android {
namespace audio_utils {

/**
 * Launch reader task which accumulates data written to the fd that this class exposes.
 * Usage as follows:
 *  {
 *     writer = FdToString::createWriter(); // fork point, reader launched
 *     sendFdToWriters(writer.borrowFdUnsafe()); // fd is safe while writer is valid
 *     st = FdToString::closeWriterAndGetString(std::move(writer));
 *     // join point (writers must be done)
 *  } // writer dtor closes fd, joins reader if close not called
 *
 * This class expects that the write fd is unduped when close is called, otherwise the reader will
 * always hit the timeout. We implicitly trust that the borrowed fd won't be duped (or that its
 * dupes will be closed by closeWriterAndGetString()).
 * Note, the reader closes the fd to signal which closes the read end of the pipe. If the writer is
 * living in a process without signal(SIGPIPE, SIGIGN), they will crash.
 */
class FdToString {
  public:
    class Writer {
      public:
        /**
         * Returns the write end of the pipe as a file descriptor.
         * Non-Owning reference! This object must be valid to keep accessing the fd.
         * Do not close this fd directly as this class should own the fd.
         * Leaking dupes of this fd will keep the reader alive.
         * Use closeWriterAndGetString(Writer&& w) to consume this object and return the string.
         * The fd returned by this method is invalid after this point.
         */
        int borrowFdUnsafe() const { return mWriteFd.get(); }

        const android::base::unique_fd& getFd() const { return mWriteFd; }

      private:
        friend FdToString;
        // Pre-condition: fd and future both valid. Should only be called from create.
        Writer(android::base::unique_fd writeFd, std::future<std::string> output)
            : mOutput(std::move(output)), mWriteFd(std::move(writeFd)) {}

        std::future<std::string> mOutput;
        android::base::unique_fd mWriteFd;  // order important! must be destroyed first to join
    };

  public:
    /**
     * Factory method for Writer object. Launches the async reader.
     * \param prefix is the prefix string prepended to each new line.
     * \param timeoutMs is the total timeout to wait for obtaining data in milliseconds.
     * \returns nullopt on init error.
     */
    static std::optional<Writer> createWriter(
            std::string_view prefix_ = "- ",
            std::chrono::milliseconds timeout = std::chrono::milliseconds{200}) {
        android::base::unique_fd readFd, writeFd;
        if (!android::base::Pipe(&readFd, &writeFd)) return {};
        const auto flags = fcntl(readFd.get(), F_GETFL);
        if (flags < 0) return {};
        // Set (only) the reader as non-blocking. We want to only read until the deadline.
        if (fcntl(readFd, F_SETFL, flags | O_NONBLOCK) < 0) return {};
        const auto deadline = systemTime() + std::chrono::nanoseconds{timeout}.count();
        // Launch async reader task, will return after deadline
        return Writer{
                std::move(writeFd),
                std::async(std::launch::async,
                           // reader task to follow, logically oneshot
                           [fd = std::move(readFd), deadline,
                            prefix = std::string{prefix_}]() mutable {
                               char buf[4096];
                               std::string out;
                               bool requiresPrefix = true;

                               while (true) {
                                   struct pollfd pfd = {
                                           .fd = fd.get(),
                                           .events = POLLIN | POLLRDHUP,
                                   };
                                   const int waitMs =
                                           toMillisecondTimeoutDelay(systemTime(), deadline);
                                   if (waitMs <= 0) break;
                                   const int retval = poll(&pfd, 1 /* nfds*/, waitMs);
                                   // break on error or timeout
                                   if (retval <= 0 || (pfd.revents & POLLIN) != POLLIN) break;
                                   // data is available
                                   int red = read(fd, buf, sizeof(buf));
                                   if (red < 0) {
                                       break;
                                   } else if (red == 0) {
                                       continue;
                                   }

                                   std::string_view sv{buf, static_cast<size_t>(red)};

                                   if (!prefix.empty()) {
                                       size_t ind;
                                       while ((ind = sv.find('\n', 0)) != std::string_view::npos) {
                                           if (requiresPrefix) {
                                               out.append(prefix);
                                           }
                                           out.append(sv.data(), ind + 1);
                                           sv.remove_prefix(ind + 1);
                                           requiresPrefix = true;
                                       }
                                       if (sv.length() > 0) {
                                           out.append(sv);
                                           requiresPrefix = false;
                                       }
                                   } else {
                                       out.append(sv);
                                   }
                               }
                               // Explicit clear, because state is kept until future consumption
                               fd.reset();
                               return out;
                           })};
    }

    /**
     * Closes the write side. Returns the string representation of data written to the fd.
     * Awaits reader thread.
     *
     * All writers should have returned by this point.
     *
     */
    static std::string closeWriterAndGetString(Writer&& writer) {
        // Closes the fd, which finishes the reader
        writer.mWriteFd.reset();
        // moved out of future + NVRO
        return writer.mOutput.get();
    }
};

}  // namespace audio_utils
}  // namespace android

#endif  // !ANDROID_AUDIO_FD_TO_STRING_H
