//
// Created by austin stone on 2019-07-01.
//

#ifndef FFMPEG_FFMPEG_FILE_H
#define FFMPEG_FFMPEG_FILE_H
//删除文件或目录
int deleteFile(const char* input);
//文件目录操作
int opDirectory(const char* input);
//打印多媒体文件信息
int printFileInfo(const char* input);



#endif //FFMPEG_FFMPEG_FILE_H
