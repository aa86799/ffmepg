# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#引入 头文件所在目录  若有多个目录  空格 分隔
include_directories(
        ${CMAKE_SOURCE_DIR}/include/ffmpeg-4.1.3/
        ${CMAKE_SOURCE_DIR}
)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        ffmpeg-stone-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        # 配置源文件或者头文件的路径    这里的文件路径是相对于CMakeList.txt的路径
        ffmpeg-stone-lib.cpp
        ffmpeg-file.cpp
        )

add_library(avcodec SHARED IMPORTED)
add_library(avdevice SHARED IMPORTED)
add_library(avfilter SHARED IMPORTED)
add_library(avformat SHARED IMPORTED)
add_library(avresample SHARED IMPORTED)
add_library(avutil SHARED IMPORTED)
add_library(postproc SHARED IMPORTED)
add_library(swresample SHARED IMPORTED)
add_library(swscale SHARED IMPORTED)
add_library(fdk-aac SHARED IMPORTED)

set(ffmpegLibs "${CMAKE_SOURCE_DIR}/libs/ffmpeg-4.1.3/${CMAKE_ANDROID_ARCH_ABI}")
set_target_properties(
        avcodec
        PROPERTIES IMPORTED_LOCATION
        ${ffmpegLibs}/libavcodec-58.so
)

set_target_properties(
        avdevice
        PROPERTIES IMPORTED_LOCATION
        ${ffmpegLibs}/libavdevice-58.so
)

set_target_properties(
        avfilter
        PROPERTIES IMPORTED_LOCATION
        ${ffmpegLibs}/libavfilter-7.so
)

set_target_properties(
        avformat
        PROPERTIES IMPORTED_LOCATION
        ${ffmpegLibs}/libavformat-58.so
)

set_target_properties(
        avresample
        PROPERTIES IMPORTED_LOCATION
        ${ffmpegLibs}/libavresample-4.so
)

set_target_properties(
        avutil
        PROPERTIES IMPORTED_LOCATION
        ${ffmpegLibs}/libavutil-56.so
)

set_target_properties(
        postproc
        PROPERTIES IMPORTED_LOCATION
        ${ffmpegLibs}/libpostproc-55.so
)

set_target_properties(
        swresample
        PROPERTIES IMPORTED_LOCATION
        ${ffmpegLibs}/libswresample-3.so
)

set_target_properties(
        swscale
        PROPERTIES IMPORTED_LOCATION
        ${ffmpegLibs}/libswscale-5.so
)

set_target_properties(
        fdk-aac
        PROPERTIES IMPORTED_LOCATION
        ${ffmpegLibs}/libfdk-aac.so
)


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

#target_link_libraries里库文件的顺序符合gcc链接顺序的规则，即被依赖的库放在依赖它的库的后面
target_link_libraries( # Specifies the target library.
        ffmpeg-stone-lib

        avutil
        postproc
        avcodec
        avdevice
        avfilter
        avformat
        avresample
        swresample
        swscale
        fdk-aac

        android
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})