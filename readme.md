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
     - ffmpeg头文件 `<libavformat/avformat.h>`
     - 资源(文件或目录)删除 avpriv_io_delete()
     - 打开目录 avio_open_dir() 
     - 读取目录 avio_read_dir()
     - 关闭目录 avio_close_dir()
  