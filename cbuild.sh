#!/bin/bash
###
 # @Author: Fleuret 614832056@qq.com
 # @Date: 2024-01-17 17:35:47
 # @LastEditors: Fleuret 614832056@qq.com
 # @LastEditTime: 2024-01-17 17:50:30
 # @FilePath: /smart-home---reuse-model/build.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%A
### 
#到到当前build目录下 改成使用相对路径
cd "$(dirname "$0")/build"
# 写一个条件判断,如果编译成功,就运行
# 提示用户输入
echo "Do you want to delete all files in the build directory? (y/others) ~_~"
read answer

# 检查用户的输入
if [ "$answer" == "y" ]
then
    echo "Deleting all files in the build directory..."
    rm -rf *
else
    echo "Not deleting files."
fi

if [ -f Makefile ]
then
    make -j$(nproc)  # -j$(nproc) 用多线程编译 快速编译
else
    cmake ..
    make -j$(nproc)
fi


