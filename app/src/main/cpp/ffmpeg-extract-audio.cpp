//
// Created by austin stone on 2019-07-04.
//

#include "ffmpeg-extract-audio.h"

extern "C" {

#include <libavformat/avformat.h>
#include <libavutil/mathematics.h> //用于处理时间戳和时间基的数学实用程序。

}

int extractAudioData(const char *input, const char *output) {
    /*
     * 注册所有已编译的muxer、demuxer和协议
     * 4.x 已过时，可以去掉
     */
    av_register_all();

    //初始化网络. 确定不需要网络，可以不加
//    avformat_network_init();


    av_log(nullptr, AV_LOG_INFO, "打开文件流或网络流. input %s", input);
    AVFormatContext *fmtCxt = avformat_alloc_context(); //可以赋值 null，也可以先分配内存
//    int ret = avformat_open_input(&fmtCxt, path, nullptr, nullptr); //path 的后缀是 mp4，第三个参数就可以为 null
    int ret = avformat_open_input(&fmtCxt, input, nullptr, nullptr); //反之，要指定第三个参数，即输入文件格式结构指针 AVInputFormat
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "打开多媒体文件流失败. path %s", av_err2str(ret));
        return -1001;
    }

    FILE *dstFile = fopen(output, "wbe"); //读写模式打开输出文件
    if (dstFile == nullptr) {
        av_log(nullptr, AV_LOG_ERROR, "输出文件打开失败. output %s", output);
        avformat_close_input(&fmtCxt);
        return -1002;
    }

    av_log(nullptr, AV_LOG_INFO, "打印关于输入或输出格式的详细信息(是一个手工调试的函数, 例如\n"
                                 "*持续时间，比特率，流，容器，程序，元数据，边数据，\n"
                                 "*编解码器和时基). input %s", input);
    av_dump_format(fmtCxt, 0, input, 0); //最后一个0表示输入文件，若为1表示输出。

    av_log(nullptr, AV_LOG_INFO, "查找音频. input %s", input);
//    //-1,-1 要处理流的索引号，音频、视频、字幕流的索引号； -1 让它自动选择
    ret = av_find_best_stream(fmtCxt, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "av_find_best_stream error. %s", av_err2str(ret));
        return -1003;
    }
    int audioIndex = ret; //音频流的 index

    av_log(nullptr, AV_LOG_INFO, "读音频流");
    AVPacket pkt;
    av_init_packet(&pkt); //初始化数据包
    while (av_read_frame(fmtCxt, &pkt) >= 0) {//从数据包内读取帧
        if (pkt.stream_index == audioIndex) {
            //将数据包，每次写入，总数量，写入文件；返回实际写入的数量
            auto len = fwrite(pkt.data, 1, static_cast<size_t>(pkt.size), dstFile);
            if (len != pkt.size) {
                av_log(nullptr, AV_LOG_WARNING, "fwrite 文件写入发生了问题. ");
            }
        }
        av_packet_unref(&pkt); //清理数据包引用
    }

    av_log(nullptr, AV_LOG_INFO, "关闭流. input %s", input);
    avformat_close_input(&fmtCxt);

    if (fmtCxt != nullptr) {//正常 close 后，这里一定是 null 的
        avformat_free_context(fmtCxt);
        av_log(nullptr, AV_LOG_INFO, "释放 AVFormatContext");
    }

    fclose(dstFile);//关闭输出文件指针

//A/libc: Fatal signal 5 (SIGTRAP), code 1 in tid xxx .  不写 return 语句会报这个的
    return ret;
}

int extractAudioDataForAAC(const char *input, const char *output) {
    /*
  * 注册所有已编译的muxer、demuxer和协议
  * 4.x 已过时，可以去掉
  */
    av_register_all();

    //初始化网络. 确定不需要网络，可以不加
//    avformat_network_init();

    int ret;
    char errors[1024];

    av_log(nullptr, AV_LOG_INFO, "打开文件流或网络流. input %s", input);
    AVFormatContext *fmtCxt = nullptr;//avformat_alloc_context(); //可以赋值 null，也可以先分配内存
    ret = avformat_open_input(&fmtCxt, input, nullptr, nullptr); //反之，要指定第三个参数，即输入文件格式结构指针 AVInputFormat
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "打开多媒体文件流失败. path %s", av_err2str(ret));
        return -1001;
    }

    av_log(nullptr, AV_LOG_INFO, "查找信息，写入 fmtCxt. input %s", input);
    if ((ret = avformat_find_stream_info(fmtCxt, nullptr)) < 0) {
        av_strerror(ret, errors, 1024);
        av_log(nullptr, AV_LOG_DEBUG, "failed to find outStream information: %s, %d(%s)\n",
               input,
               ret,
               errors);
        return -1;
    }

    /*dump input information*/
//    av_dump_format(fmtCxt, 0, input, 0);

    AVStream *in_stream = fmtCxt->streams[1];//音频流
    AVCodecParameters *in_codecpar = in_stream->codecpar; //输入文件的 codecpar，即 编解码器参数
    av_log(nullptr, AV_LOG_INFO, "判断是否是音频流信息");
    if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO) {
        av_log(nullptr, AV_LOG_ERROR, "The Codec type is invalid!\n");
    }

    av_log(nullptr, AV_LOG_INFO, "猜测输出文件格式，得到 AVOutputFormat*");
    AVOutputFormat *outFmt = av_guess_format(nullptr, output, nullptr);
    if (outFmt == nullptr) {
        av_log(nullptr, AV_LOG_ERROR, "猜格式出了问题 %s", output);
        return -1002;
    }

    av_log(nullptr, AV_LOG_INFO, "初始化输出文件的 AVFormatContext");
    AVFormatContext *outFmtCxt = avformat_alloc_context();
    outFmtCxt->oformat = outFmt;

    av_log(nullptr, AV_LOG_INFO, "从输出文件的 AVFormatContext 创建流 AVStream*");
    AVStream *outStream = avformat_new_stream(outFmtCxt, nullptr);
    if (outStream == nullptr) {
        av_log(nullptr, AV_LOG_ERROR, "创建流 出了问题");
        return -1003;
    }

    av_log(nullptr, AV_LOG_INFO, "判断输入文件的 AVFormatContext.streams 流的数量是否小于2，若小于说明没有音频流");
    if ((ret = fmtCxt->nb_streams) < 2) {
        av_log(nullptr, AV_LOG_ERROR, "the number of outStream is too less!\n");
        return ret;
    }

    av_log(nullptr, AV_LOG_INFO, "复制输入文件的编解码参数到输出流中");
    if ((ret = avcodec_parameters_copy(outStream->codecpar, in_codecpar)) < 0) {
        av_strerror(ret, errors, 1024);
        av_log(nullptr, AV_LOG_ERROR,
               "Failed to copy codec parameter, %d(%s)\n",
               ret, errors);
        return ret;
    }

    outStream->codecpar->codec_tag = 0;//编解码器的附加信息 tag

    av_log(nullptr, AV_LOG_INFO, "创建并初始化用于访问输出文件的 AVIOContext* ");
    ret = avio_open(&outFmtCxt->pb, output, AVIO_FLAG_WRITE);
    if (ret < 0) {
        char *buf = static_cast<char *>(malloc(1024));
        av_strerror(ret, buf, 1024);
        av_log(nullptr, AV_LOG_ERROR, "avio_open() 出了问题");
        return -1004;
    }

    /*dump output information*/
//    av_dump_format(outFmtCxt, 0, output, 1);

    AVPacket pkt;
    av_init_packet(&pkt); //初始化数据包
    pkt.data = nullptr;
    pkt.size = 0;

    av_log(nullptr, AV_LOG_INFO, "在输入文件的 AVFormatContext* 中，查找最合适的音频流");
    ret = av_find_best_stream(fmtCxt, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "av_find_best_stream error. %s", av_err2str(ret));
        return -1003;
    }
    int audioIndex = ret; //音频流的 index

    av_log(nullptr, AV_LOG_INFO, "向输出的 AVFormatContext* 中，写入 header");
    ret = avformat_write_header(outFmtCxt, nullptr);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "写入 header 出了问题");
        return -1004;
    }

    av_log(nullptr, AV_LOG_INFO, "循环读取帧到数据包中；若读到，且是音频流，处理时间戳等，再写入到输出的 AVFormatContext*");
    while (av_read_frame(fmtCxt, &pkt) >= 0) {//从数据包内读取帧
        if (pkt.stream_index == audioIndex) {
            //时间戳处理
            pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, outStream->time_base, static_cast<AVRounding> (AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
            pkt.dts = pkt.pts;
            //持续时间
            pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, outStream->time_base);
            pkt.pos = -1;
            pkt.stream_index = 0;
            //将数据包写入
            av_interleaved_write_frame(outFmtCxt, &pkt);
        }
        av_packet_unref(&pkt); //清理数据包引用
    }
    av_write_trailer(outFmtCxt); //将 stream trailer 写入输出文件，并释放私有 data
    avformat_close_input(&fmtCxt); //关闭输入
    avio_close(outFmtCxt->pb); //关闭输出
    return ret;
}
