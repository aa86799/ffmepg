# 项目环境
    基于ffmepg4.1.3编译的 so 文件。
    其中 libavresample 已过时，被 libswresample 所替换。libavdevice 可能也用不着，可以裁剪掉，即不编译它。
    整体 so 文件，包含以下库：
>   libavcodec
    libavdevice
    libavfilter
    libavformat
    libavresample
    libavutil
    libpostproc
    libswresample
    libswscale
  
# 文件操作 *ffmpeg-file.cpp*
     - ffmpeg头文件 <libavformat/avformat.h> 处理文件格式
     - 资源(文件或目录)删除 avpriv_io_delete()
     - 打开目录 avio_open_dir() 
     - 读取目录 avio_read_dir()
     - 关闭目录 avio_close_dir()
     
# 打印多媒体文件信息 *ffmpeg-file.cpp*
    printFileInfo() 内，使用了 <libavformat/avformat.h> 。
    打印函数 av_dump_format()
  
# 抽取音频 *ffmpeg-extract-audio.cpp*
    - ffmpeg头文件  <libavformat/avformat.h> 处理文件格式
    - <libavutil/mathematics.h> //用于处理时间戳和时间基的数学实用程序。
    extractAudioData() 读取mp4的AAC音频数据，输出到 ".aac" 文件中可能无法播放(需要根据 aac 协议，添加 ADTS header)。
    extractAudioDataForAAC()  读取mp4的AAC音频数据，并使用 ffmpeg 相关函数输出到 ".aac"文件中，且可以播放。