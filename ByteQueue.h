//
// Created by ByteDance on 11/9/25.
//

#ifndef PLAYGROUND_BYTEQUEUE_H
#define PLAYGROUND_BYTEQUEUE_H
#include <cstdint>
#include <vector>

/**
 * an implementation of byte queue
 * in circular ring/buffer
 */
class ByteQueue {
private:
    std::uint32_t head_ = 0;
    std::uint32_t tail_ = 0;
    bool full = false;
    std::vector<uint8_t> buffer_(12,0);
public:
    ByteQueue() = default;

    void push(uint8_t byte) {

    }
};

#endif //PLAYGROUND_BYTEQUEUE_H