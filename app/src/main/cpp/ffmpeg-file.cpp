//
// Created by austin stone on 2019-07-01.
//

#include "ffmpeg-file.h"

extern "C" {

#include <libavformat/avformat.h>

}

//文件删除
int deleteFile(const char *path) {
    int ret;
    ret = avpriv_io_delete(path); //删除文件gcl
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "failed to delete file %s", path);
    }

//    avpriv_io_move(v1, v2) //移动并重命名

    return ret;
}

//文件目录操作
int opDirectory(const char *path) {
    int ret;

    AVIODirContext *avioContext = nullptr;
    AVDictionary *d = nullptr;
    AVIODirEntry *entry = nullptr;

    ret = avio_open_dir(&avioContext, path, &d); //内部初始化 avioContext 指针
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "1. failed to open file dir %s", av_err2str(ret));
        goto __fail; //跳转到__fail
    }

    while (ret >= 0) {
        ret = avio_read_dir(avioContext, &entry);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "2. failed to read file dir %s", av_err2str(ret));
        }

        if (!entry || ret < 0) {
            break;
        }

        /*
         * %12 PRId64 => %12 ld
         *  效果就是 含长整数在内右端对齐的 12位字符，缺失的以空格代替
         *
         *  entry->mame 文件或目录名
         */
        av_log(nullptr, AV_LOG_INFO, "3. %12" PRId64 " %s \n", entry->size, entry->name);
    }

    __fail:
    av_log(nullptr, AV_LOG_INFO, "into close dir statement");
    ret = avio_close_dir(&avioContext);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "2. failed to close file dir %s", av_err2str(ret));
    }

    return ret;
}