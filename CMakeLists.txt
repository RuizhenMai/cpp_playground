cmake_minimum_required(VERSION 4.0)
project(Playground)

set(CMAKE_CXX_STANDARD 20)

add_executable(Playground main.cpp
        stream_client.h
        shared_mem_ipc.h
        ByteQueue.h
        UnionFind.h)
