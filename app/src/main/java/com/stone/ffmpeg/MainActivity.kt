package com.stone.ffmpeg

import android.Manifest
import android.annotation.SuppressLint
import android.os.Bundle
import android.os.Environment
import androidx.appcompat.app.AppCompatActivity
import com.tbruyelle.rxpermissions2.RxPermissions
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
            .subscribe { granted->
                run {
                    if (granted) {
                        ffmpegOpDir(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path + "/abcdef")
                        ffmpegOpDir(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path)
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
