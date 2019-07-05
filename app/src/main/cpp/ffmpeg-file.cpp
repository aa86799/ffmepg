//
// Created by austin stone on 2019-07-01.
//

#include "ffmpeg-file.h"

extern "C" {

#include <libavformat/avformat.h>

}

//文件删除
int deleteFile(const char *input) {
    int ret;
    ret = avpriv_io_delete(input); //删除文件gcl
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "failed to delete file %s", input);
    }

//    avpriv_io_move(v1, v2) //移动并重命名

    return ret;
}

//文件目录操作
int opDirectory(const char *input) {
    int ret;

    AVIODirContext *avioContext = nullptr;
    AVDictionary *d = nullptr;
    AVIODirEntry *entry = nullptr;

    ret = avio_open_dir(&avioContext, input, &d); //内部初始化 avioContext 指针
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

int printFileInfo(const char *input) {
    /*
     * 注册所有已编译的muxer、demuxer和协议
     * 4.x 已过时，可以去掉
     */
    av_register_all();

    //初始化网络. 确定不需要网络，可以不加
//    avformat_network_init();


    av_log(nullptr, AV_LOG_INFO, "打开文件流或网络流. input %s", input);
    AVFormatContext *fmtCxt = avformat_alloc_context(); //可以赋值 null，也可以先分配内存
//    int ret = avformat_open_input(&fmtCxt, input, nullptr, nullptr); //input 的后缀是 mp4，第三个参数就可以为 null
    int ret = avformat_open_input(&fmtCxt, input, nullptr, nullptr); //反之，要指定第三个参数，即输入文件格式结构指针 AVInputFormat
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "打开多媒体文件流失败. input %s", av_err2str(ret));
    }

    av_log(nullptr, AV_LOG_INFO, "打印关于输入或输出格式的详细信息(是一个手工调试的函数, 例如\n"
                                 "*持续时间，比特率，流，容器，程序，元数据，边数据，\n"
                                 "*编解码器和时基). input %s", input);
    av_dump_format(fmtCxt, 0, input, 0); //最后一个0表示输入文件，若为1表示输出。

    av_log(nullptr, AV_LOG_INFO, "关闭流. input %s", input);
    avformat_close_input(&fmtCxt);

    if (fmtCxt) {
        avformat_free_context(fmtCxt);
        av_log(nullptr, AV_LOG_INFO, "释放 AVFormatContext");
    }

    return ret;
}

