# 这样分级写CMakeLists.txt文件就可以使用vscode里的构建工具，直接构建整个项目
cmake_minimum_required(VERSION 3.0)
project(main) #工程的名字，不是可执行文件的名字

# 配置编译选项 就是-o-g那些
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# 指定搜索的子目录
add_subdirectory(testmuduo)
