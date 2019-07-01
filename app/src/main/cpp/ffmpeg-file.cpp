//
// Created by austin stone on 2019-07-01.
//

#include "ffmpeg-file.h"

extern "C" {

#include <libavformat/avformat.h>

}

int deleteFile(const char* path) {
    int ret;
    ret = avpriv_io_delete(path); //删除文件gcl
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "failed to delete file %s", path);
    }

//    avpriv_io_move(v1, v2) //移动并重命名

    return ret;
}