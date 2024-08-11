cmake_minimum_required(VERSION 3.15)

project(smartHome)

#指定编译器
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
#添加调试信息
#set(CMAKE_BUILD_TYPE Debug)

#编译的时候加入宏
# add_definitions(-DDEBUG)

#配置CFLAGS(编译选项)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address") 

#指定头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tool/usr/include
${CMAKE_CURRENT_SOURCE_DIR}/inc
${CMAKE_CURRENT_SOURCE_DIR}/tool/usr/include/python3.10)

#指定源文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src DIR_SRCS)

#指定生成的可执行文件路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#这个必须放到add_executable前面，否则会报错
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/tool/usr/lib
${CMAKE_CURRENT_SOURCE_DIR}/tool/usr/lib/python3.10/config-3.10-aarch64-linux-gnu
${CMAKE_CURRENT_SOURCE_DIR}/tool/usr/lib/aarch64-linux-gnu)

add_executable(${PROJECT_NAME} ${DIR_SRCS})

#指定链接库
target_link_libraries(${PROJECT_NAME} pthread wiringPi inih crypt python3.10 expat z)
