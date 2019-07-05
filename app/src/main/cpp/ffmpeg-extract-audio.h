//
// Created by austin stone on 2019-07-04.
//

#ifndef FFMPEG_FFMPEG_EXTRACT_AUDIO_H
#define FFMPEG_FFMPEG_EXTRACT_AUDIO_H

//提取音频数据(AAC 音频需要添加ADTS header才能播放)
int extractAudioData(const char* input, const char* output);
//提取音频数据(AAC 音频，用 ffmpeg写入)
int extractAudioDataForAAC(const char* input, const char* output);


#endif //FFMPEG_FFMPEG_EXTRACT_AUDIO_H
