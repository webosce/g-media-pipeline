// Copyright (c) 2018-2020 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#include <base/types.h>
#include "composer.h"

namespace gmp { namespace parser {

template<>
pbnjson::JValue to_json(const base::result_t & result) {
  return pbnjson::JObject {{"state", result.state},
                           {"mediaId", result.mediaId}};
}

template<>
pbnjson::JValue to_json(const base::video_info_t & info) {
  return pbnjson::JObject {{"video",
      pbnjson::JObject{{"codec", info.codec},
                       {"bitrate", (int64_t)info.bit_rate},
                       {"width", (int32_t)info.width},
                       {"height", (int32_t)info.height},
                       {"frame_rate", pbnjson::JObject {{"num", info.frame_rate.num},
                                                        {"den", info.frame_rate.den}}}}}};
}

template<>
pbnjson::JValue to_json(const base::audio_info_t & info) {
  return pbnjson::JObject {{"codec", info.codec},
                {"bitrate", (int64_t)info.bit_rate},
                {"sample_rate", (int32_t)info.sample_rate}};
}

template<>
pbnjson::JValue to_json(const base::source_info_t & info) {
  pbnjson::JArray programs;
  for (const auto & program : info.programs)
    programs.put(programs.arraySize(), pbnjson::JObject {
                              {"audio_stream", (int32_t)program.audio_stream},
                              {"video_stream", (int32_t)program.video_stream}
                              });
  pbnjson::JArray audio_streams;
  for (const auto & audio_stream : info.audio_streams)
    audio_streams.put(audio_streams.arraySize(), pbnjson::JObject {
                              {"codec", audio_stream.codec},
                              {"bit_rate", (int64_t)audio_stream.bit_rate},
                              {"sample_rate", (int32_t)audio_stream.sample_rate}
                              });
  pbnjson::JArray video_streams;
  for (const auto & video_stream : info.video_streams)
    video_streams.put(video_streams.arraySize(), to_json(video_stream));

  return pbnjson::JObject {{"container", info.container},
               {"duration", info.duration},
               {"seekable", info.seekable},
               {"programs", programs},
               {"audio_streams", audio_streams},
               {"video_streams", video_streams}};
}

template<>
pbnjson::JValue to_json(const base::error_t & error) {
  return pbnjson::JObject {{"errorCode", error.errorCode},
               {"errorText", error.errorText},
               {"mediaId", error.mediaId}};
}

template<>
pbnjson::JValue to_json(const base::buffer_range_t & buffer_range) {
  return pbnjson::JObject {{"beginTime", (int64_t)buffer_range.beginTime},
                           {"endTime", (int64_t)buffer_range.endTime},
                           {"remainingTime", (int64_t)buffer_range.remainingTime},
                           {"percent", (int64_t)buffer_range.percent}};
}

template<>
pbnjson::JValue to_json(const base::media_info_t & info) {
  return pbnjson::JObject {{"mediaId", info.mediaId}};
}

// {"options":{"option":{"windowId":"_Window_Id_1","useSeekableRanges":true,"videoDisplayMode":"Textured","appId":"com.webos.app.mediaevents-test","needAudio":true,"bufferControl":{"userBufferCtrl":false},"transmission":{"httpHeader":{"referer":"https://www.w3.org/2010/05/video/mediaevents.html","userAgent":"Mozilla/5.0 (Web0S; Linux) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36 WebAppManager","cookies":""}},"preload":"false"}},"id":"_dPG8v3e9kM98mI","uri":"https://media.w3.org/2010/05/sintel/trailer.mp4"}
template<>
pbnjson::JValue to_json(const base::load_param_t & load_param) {
  return pbnjson::JObject {{"options",
    pbnjson::JObject {{"option",
        pbnjson::JObject {{"videoDisplayMode", load_param.videoDisplayMode},
                          {"display-path", (int32_t)load_param.displayPath},
                          {"windowId", load_param.windowId}}}}},
                          {"uri", load_param.uri}};
}

Composer::Composer() : _dom(pbnjson::JObject()) {}

std::string Composer::result() {
  std::string result = _dom.stringify();
  return result;
}

}  // namespace parser
}  // namespace gmp
