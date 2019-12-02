# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(opencv_version OpenCV410-android-sdk)
set(opencv_lib libopencv_java4.so)

if (false)
    set(OpenCV_STATIC ON)
    set(OpenCV_DIR /home/lynx/fuck_mount/Android/Proj/${opencv_version}/sdk/native/jni)
    find_package(OpenCV REQUIRED)
    if (OpenCV_FOUND)
        message(WARNING "opencv libs: ${OpenCV_LIBS}")
    else (OpenCV_FOUND)
        message(WARNING "opencv not found!")
    endif(OpenCV_FOUND)
elseif (true)
    if (false)
        include_directories(/home/lynx/fuck_mount/Android/Proj/${opencv_version}/sdk/native/jni/include)
        add_library(fuck SHARED IMPORTED)
        set_target_properties(fuck PROPERTIES IMPORTED_LOCATION
                /home/lynx/fuck_mount/Android/Proj/${opencv_version}/sdk/native/libs/${ANDROID_ABI}/${opencv_lib})
    endif()
    if (true)
        include_directories(/home/lynx/fuck_mount/Android/Proj/OpenCV410-sdk/OpenCV-android-sdk/sdk/native/jni/include)
        add_library(shit SHARED IMPORTED)
        set_target_properties(shit PROPERTIES IMPORTED_LOCATION
                /home/lynx/fuck_mount/Android/Proj/OpenCV410-sdk/OpenCV-android-sdk/sdk/native/libs/${ANDROID_ABI}/libopencv_world.so)
    endif()
endif()


add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # 自己写的cpp文件相对路径
        test_b.cpp
        test_a.cpp
        test_c.cpp
        test_e.cpp)

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

target_link_libraries( # Specifies the target library.
        native-lib

        # opencv库定义的
#        fuck
        shit
        # TODO 解决 AndroidBitmap 报错, 使本机代码安全地访问Java对象的像素缓冲区的位图
        jnigraphics

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
#       TODO
        ${OpenCV_LIBS})