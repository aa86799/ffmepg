package com.stone.ffmpeg

import android.Manifest
import android.annotation.SuppressLint
import android.os.Bundle
import android.os.Environment
import androidx.appcompat.app.AppCompatActivity
import com.tbruyelle.rxpermissions2.RxPermissions
import io.reactivex.schedulers.Schedulers
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private val rxPermissions = RxPermissions(this)

    @SuppressLint("CheckResult")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        sample_text.text = stringFromJNI()

        ffmpegLog()

        ffmpegDeleteFile(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path + "/abcdef.txt")


        rxPermissions.request(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE)
            .subscribeOn(Schedulers.io())
            .subscribe { granted ->
                run {
                    if (granted) {
//                        ffmpegOpDir(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path + "/abcdef")
//                        ffmpegOpDir(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path)

                        /*
                         * adb push /Users/stone/Documents/aavp/ffmpeg/out-dump2.ts sdcard/Download/
                         */
//                        ffmpegPrintFileInfo("${Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path}/out-dump2.ts")
//                        ffmpegPrintFileInfo("${Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path}/out.mp4")

                        /*
                         * 抽取到的 aac 音频数据，不能直接播放。需要添加 ADTS header。
                         * 可以参考：http://www.imooc.com/article/254733
                         */
//                        extractAudioData("${Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path}/out.mp4",
//                            "${Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path}/out-audio.aac")

                        /*
                         * 使用 ffmpeg 制取 mp4 AAC 音频，并写入 AAC 音频文件
                         */
                        extractAudioDataForAAC("${Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path}/input.mp4",
                            "${Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path}/out-audio2.aac")
                    } else {

                    }
                }
            }

    }

    /**
     * A native method that is implemented by the 'ffmpeg-stone-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun ffmpegLog()

    external fun ffmpegDeleteFile(path: String)

    external fun ffmpegOpDir(path: String)

    external fun ffmpegPrintFileInfo(path: String)

    external fun extractAudioData(input: String, output:String)

    external fun extractAudioDataForAAC(input: String, output:String)

    companion object {

        // Used to load the 'ffmpeg-stone-lib' library on application startup.
//        init {
//            System.loadLibrary("ffmpeg-stone-lib")
//        }
    }

    init {
        System.loadLibrary("ffmpeg-stone-lib")
    }
}
