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

/*
 * To promote uniform usage of audio key values within the ATRACE environment
 * we keep a master list of all keys used and the meaning of the associated values.
 *
 * These macro definitions allow compile-time check of misspellings.
 *
 * By convention we use camel case, but automated conversion to snake case
 * depending on the output medium is possible.
 *
 * Values here are all specified in alphabetical order.
 */

/*
 * Do not modify any AUDIO_TRACE_THREAD_ or AUDIO_TRACE_TRACK_
 * events without consulting the git blame owner.
 *
 * These form a taxonomy of events suitable for prefix filtering.
 *
 * audio.track will include all track events.
 * audio.track.interval will include only the track interval events.
 */

// This is used for the linux thread name, which is limited to 16 chars in linux.
#define AUDIO_TRACE_THREAD_NAME_PREFIX "atd."             // Data worker thread

#define AUDIO_TRACE_PREFIX_AUDIO         "audio."         // Top level prefix
#define AUDIO_TRACE_PREFIX_FASTCAPTURE   "fastCapture."
#define AUDIO_TRACE_PREFIX_FASTMIXER     "fastMixer."
#define AUDIO_TRACE_PREFIX_THREAD        "thread."        // Data worker thread
#define AUDIO_TRACE_PREFIX_TRACK         "track."         // Audio(Track|Record)

#define AUDIO_TRACE_PREFIX_AUDIO_THREAD         AUDIO_TRACE_PREFIX_AUDIO AUDIO_TRACE_PREFIX_THREAD
#define AUDIO_TRACE_PREFIX_AUDIO_TRACK          AUDIO_TRACE_PREFIX_AUDIO AUDIO_TRACE_PREFIX_TRACK

#define AUDIO_TRACE_PREFIX_AUDIO_TRACK_ACTION   AUDIO_TRACE_PREFIX_AUDIO_TRACK "action."
#define AUDIO_TRACE_PREFIX_AUDIO_TRACK_FRDY     AUDIO_TRACE_PREFIX_AUDIO_TRACK "fRdy."
#define AUDIO_TRACE_PREFIX_AUDIO_TRACK_INTERVAL AUDIO_TRACE_PREFIX_AUDIO_TRACK "interval."
#define AUDIO_TRACE_PREFIX_AUDIO_TRACK_NRDY     AUDIO_TRACE_PREFIX_AUDIO_TRACK "nRdy."

/*
 * Events occur during the trace timeline.
 */
#define AUDIO_TRACE_EVENT_BEGIN_INTERVAL     "beginInterval"
#define AUDIO_TRACE_EVENT_END_INTERVAL       "endInterval"
#define AUDIO_TRACE_EVENT_FLUSH              "flush"
#define AUDIO_TRACE_EVENT_PAUSE              "pause"
#define AUDIO_TRACE_EVENT_REFRESH_INTERVAL   "refreshInterval"
#define AUDIO_TRACE_EVENT_START              "start"
#define AUDIO_TRACE_EVENT_STOP               "stop"
#define AUDIO_TRACE_EVENT_UNDERRUN           "underrun"

/*
 * Key, Value pairs are used to designate what happens during an event.
 */
#define AUDIO_TRACE_OBJECT_KEY_CHANNEL_MASK "channelMask" // int32_t
#define AUDIO_TRACE_OBJECT_KEY_CONTENT_TYPE "contentType" // string (audio_content_type_t)
#define AUDIO_TRACE_OBJECT_KEY_DEVICES      "devices"     // string (audio_devices_t,
                                                          //   separated by '+')
#define AUDIO_TRACE_OBJECT_KEY_EVENT        "event"       // string (AUDIO_TRACE_EVENT_*)
#define AUDIO_TRACE_OBJECT_KEY_FLAGS        "flags"       // string (audio_output_flags_t,
                                                          //   audio_input_flags_t,
                                                          //   separated by '+')
#define AUDIO_TRACE_OBJECT_KEY_FRAMECOUNT   "frameCount"  // int64_t
#define AUDIO_TRACE_OBJECT_KEY_FORMAT       "format"      // string
#define AUDIO_TRACE_OBJECT_KEY_ID           "id"          // int32_t (for threads io_handle_t)
#define AUDIO_TRACE_OBJECT_KEY_PID          "pid"         // int32_t
#define AUDIO_TRACE_OBJECT_KEY_SAMPLE_RATE  "sampleRate"  // int32_t
#define AUDIO_TRACE_OBJECT_KEY_TYPE         "type"        // string (for threads
                                                          //   IAfThreadBase::type_t)
#define AUDIO_TRACE_OBJECT_KEY_UID          "uid"         // int32_t
#define AUDIO_TRACE_OBJECT_KEY_USAGE        "usage"       // string (audio_usage_t)
