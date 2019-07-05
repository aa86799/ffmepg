#include <jni.h>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#include "ffmpeg-log.h"
#include <cstdarg>

#ifdef __cplusplus
}
#endif

#include "ffmpeg-file.h"
#include "ffmpeg-extract-audio.h"

extern "C" {//按照C语言方式编译和链接
JNIEXPORT jstring JNICALL Java_com_stone_ffmpeg_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void JNICALL Java_com_stone_ffmpeg_MainActivity_ffmpegLog(
        JNIEnv *env,
        jobject /* this */) {
    //注册日志全局的回调处理函数
    av_log_set_callback(callback_report);

    av_log_set_level(AV_LOG_DEBUG);// 设置日志级别，在这个级别值以下的级别都将输出
    av_log(NULL, AV_LOG_INFO, "hello，%s", "stone");

    ALOG(ANDROID_LOG_ERROR, "my  tag", "hello，%s","stone1");
    ALOGE("hello，%s", "stone2");
    ALOGE("hello, stone3");

    /*
     * 实际编码时，纯 ffmpeg 编程
     *   av_log_set_level(AV_LOG_ERROR);
     *   av_log(...)  第一个参数可以传 NULL
     *
     * 有了 ffmpeg-log.h 后，
     *  只要调用 ALOG 相关函数宏就好
     *
     *  而可变参数列表， 即 VLOG 相关函数宏 最终调用了 android/log.h  vprint 函数，使用了 va_list 宏。
     *  在当前函数中有可变参数列表  如，  func(char* msg, ...)
     *   (需要使用头文件 stdarg.h  ，c++中用 cstdarg )
     *   va_list vaList;
     *   va_start(vaList, msg); //第一个参数指向可变列表的地址,地址自动增加，第二个参数位固定值
     *    va_arg(vaList, char*); //va_arg第一个参数是可变参数的地址，第二个参数是传入参数的类型，返回值就是va_list中接着地址的参数值
     *    最后 va_end(vaList); //结束可变参数列表
     */
}

JNIEXPORT void JNICALL Java_com_stone_ffmpeg_MainActivity_ffmpegDeleteFile(
        JNIEnv *env,
        jobject jobj,
        jstring path) {

    int ret = deleteFile(env->GetStringUTFChars(path, JNI_FALSE));
    ALOGI("ret=%d", ret);
}

JNIEXPORT void JNICALL Java_com_stone_ffmpeg_MainActivity_ffmpegOpDir(
        JNIEnv *env,
        jobject jobj,
        jstring path) {

    int ret = opDirectory(env->GetStringUTFChars(path, JNI_FALSE));
    ALOGI("ret=%d", ret);
}

JNIEXPORT void JNICALL Java_com_stone_ffmpeg_MainActivity_ffmpegPrintFileInfo(
        JNIEnv *env,
        jobject jobj,
        jstring path) {

    int ret = printFileInfo(env->GetStringUTFChars(path, JNI_FALSE));
    ALOGI("ret=%d", ret);
}

JNIEXPORT void JNICALL Java_com_stone_ffmpeg_MainActivity_extractAudioData(
        JNIEnv *env,
        jobject jobj,
        jstring input,
        jstring output) {

    int ret = extractAudioData(env->GetStringUTFChars(input, JNI_FALSE), env->GetStringUTFChars(output, JNI_FALSE));
    ALOGI("ret=%d", ret);
}

JNIEXPORT void JNICALL Java_com_stone_ffmpeg_MainActivity_extractAudioDataForAAC(
        JNIEnv *env,
        jobject jobj,
        jstring input,
        jstring output) {

    int ret = extractAudioDataForAAC(env->GetStringUTFChars(input, JNI_FALSE), env->GetStringUTFChars(output, JNI_FALSE));
    ALOGI("ret=%d", ret);
}


}


