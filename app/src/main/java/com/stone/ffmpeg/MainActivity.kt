package com.stone.ffmpeg

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        sample_text.text = stringFromJNI()

        ffmpegLog()

        ffmpegDeleteFile(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).path+"/abcdef.txt")
    }

    /**
     * A native method that is implemented by the 'ffmpeg-stone-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun ffmpegLog()

    external fun ffmpegDeleteFile(path:String)

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
