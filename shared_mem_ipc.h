//
// Created by ByteDance on 11/9/25.
//

#ifndef PLAYGROUND_SHARED_MEM_IPC_H
#define PLAYGROUND_SHARED_MEM_IPC_H
#include <cstdint>
#include <queue>
#include <string>

class Ipc {
private:
    uint32_t capacity_;
    char* data_;
    std::queue<char> q_;

public:
    explicit Ipc(uint32_t capacity): capacity_(capacity) {
        data_ = new char[capacity_];

    }

    void Write(char* input) {
        std::string s = "hello";
        data_
    }
};


#endif //PLAYGROUND_SHARED_MEM_IPC_H