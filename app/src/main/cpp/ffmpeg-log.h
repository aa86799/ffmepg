//
// Created by austin stone on 2019-06-28.
//

#include <jni.h>
#include <libavutil/log.h>
#include <android/log.h>

#define FF_LOG_TAG "FFMPEG_LOG_"
#define charSize 2048

/*
 * 整体是将 ffmpeg log 换成 android log
 */

/*
 * 定义一组宏，传入不同的 android log level 和 ffmpeg log tag，实际调用 android log 中的 __android_log_vprint()
 *
 */
#define VLOG(level, TAG, ...)    ((int)__android_log_vprint(level, TAG, __VA_ARGS__))
#define VLOGV(...)  VLOG(ANDROID_LOG_VERBOSE,   FF_LOG_TAG, __VA_ARGS__)
#define VLOGD(...)  VLOG(ANDROID_LOG_DEBUG,     FF_LOG_TAG, __VA_ARGS__)
#define VLOGI(...)  VLOG(ANDROID_LOG_INFO,      FF_LOG_TAG, __VA_ARGS__)
#define VLOGW(...)  VLOG(ANDROID_LOG_WARN,      FF_LOG_TAG, __VA_ARGS__)
#define VLOGE(...)  VLOG(ANDROID_LOG_ERROR,     FF_LOG_TAG, __VA_ARGS__)

/*
 * 定义一组宏，传入不同的 android log level 和 ffmpeg log tag，实际调用 android log 中的 __android_log_print()
 *
 */
#define ALOG(level, TAG, ...)    ((int)__android_log_print(level, TAG, __VA_ARGS__))
#define ALOGV(...)  ALOG(ANDROID_LOG_VERBOSE,   FF_LOG_TAG, __VA_ARGS__)
#define ALOGD(...)  ALOG(ANDROID_LOG_DEBUG,     FF_LOG_TAG, __VA_ARGS__)
#define ALOGI(...)  ALOG(ANDROID_LOG_INFO,      FF_LOG_TAG, __VA_ARGS__)
#define ALOGW(...)  ALOG(ANDROID_LOG_WARN,      FF_LOG_TAG, __VA_ARGS__)
#define ALOGE(...)  ALOG(ANDROID_LOG_ERROR,     FF_LOG_TAG, __VA_ARGS__)

// 原样输出FFmpeg日志
static void ffp_log_callback_brief(void *ptr, int level, const char *fmt, va_list vl) {
    int ffplv;
    switch (level) {
        case AV_LOG_WARNING: //16
            ffplv = ANDROID_LOG_WARN;
            break;
        case AV_LOG_ERROR: //24
            ffplv = ANDROID_LOG_ERROR;
            break;
        case AV_LOG_INFO: //32
            ffplv = ANDROID_LOG_INFO;
            break;
        case AV_LOG_VERBOSE: //40
            ffplv = ANDROID_LOG_VERBOSE;
        default: //AV_LOG_DEBUG 56
            ffplv = ANDROID_LOG_DEBUG;
            break;
    }

//    if (level <= AV_LOG_INFO)
        VLOG(ffplv, FF_LOG_TAG, fmt, vl);
}

// 对FFmpeg日志进行格式化
static void callback_report(void *ptr, int level, const char *fmt, va_list vl) {
    int ffplv;
    switch (level) {
        case AV_LOG_ERROR:
            ffplv = ANDROID_LOG_ERROR;
            break;
        case AV_LOG_WARNING:
            ffplv = ANDROID_LOG_WARN;
            break;
        case AV_LOG_INFO:
            ffplv = ANDROID_LOG_INFO;
            break;
        case AV_LOG_VERBOSE:
            ffplv = ANDROID_LOG_VERBOSE;
        default:
            ffplv = ANDROID_LOG_DEBUG;
            break;
    }
    va_list vl2;
    char line[charSize];
    static int print_prefix = 1;
    va_copy(vl2, vl);
    av_log_format_line(ptr, level, fmt, vl2, line, sizeof(line), &print_prefix);
    va_end(vl2);
    ALOG(ffplv, FF_LOG_TAG, "%s", line);

}
