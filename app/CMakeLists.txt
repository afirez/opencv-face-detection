# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)

set(libs "${CMAKE_SOURCE_DIR}/src/main/jniLibs")

include_directories(src/main/cpp/include)
include_directories(src/main/jniLibs/include)

add_library(opencv_imgproc STATIC IMPORTED)
add_library(opencv_core STATIC IMPORTED)

set_target_properties(opencv_imgproc PROPERTIES IMPORTED_LOCATION
    ${libs}/${ANDROID_ABI}/libopencv_imgproc.a)
set_target_properties(opencv_core PROPERTIES IMPORTED_LOCATION
    ${libs}/${ANDROID_ABI}/libopencv_core.a)

add_library(smart_cropper SHARED
    src/main/cpp/Scanner.cpp
    src/main/cpp/smart_cropper.cpp
    src/main/cpp/android_utils.cpp)

find_library(log-lib log)
find_library(jnigraphics-lib jnigraphics)

target_link_libraries(smart_cropper
    opencv_imgproc
    opencv_core
    ${log-lib}
    ${jnigraphics-lib})