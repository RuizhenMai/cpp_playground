//
// Created by ByteDance on 11/9/25.
//

#ifndef PLAYGROUND_STREAM_CLIENT_H
#define PLAYGROUND_STREAM_CLIENT_H

#include <chrono> // For std::chrono::duration
#include <thread> // For std::this_thread::sleep_for
#include <atomic>


class StreamClient {
public:
    std::atomic<int> idx_; // 0
    std::atomic<int> ready_idx_; // -1
    std::atomic<int> pq_idx_; // -1
    std::string stream_id_ ;
    std::thread async_;
    std::vector<int> v_;
    std::atomic<int> running_;

public:
    StreamClient(const std::string& s_id, const std::vector<int>& v): idx_(0), ready_idx_(-1), pq_idx_(-1), stream_id_(s_id), v_(v) {
    }

    StreamClient(std::string&& s_id, const std::vector<int>& v): idx_(0), ready_idx_(-1), pq_idx_(-1), stream_id_(s_id), v_(v) {
    }


    void Request() {
        // all data requested
        if (!HasNext()) {
            return;
        }

        // read data asynchronously
        auto fetch = [&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ++ready_idx_;
        };
        async_ = std::thread(fetch);
        running_ = 1;
    }


    int Next() {
        if (running_ == 1) {
            async_.join();
            running_ = 0;
        }

        return v_[idx_++];;
    }

    bool ValInPq() const {
        return pq_idx_ < ready_idx_;
    }

    void IncrementPq() {
        ++pq_idx_;
    }

    bool Ready() const {
        bool res = ready_idx_ >= idx_;
        // printf("stream id [%s] ready is [%d], [%d]/[%d], \n", stream_id_.c_str(), res, static_cast<int>(ready_idx_), static_cast<int>(idx_));
        return ready_idx_ >= idx_;
    }

    bool HasNext() const {
        return idx_ < v_.size();
    }
};



#endif //PLAYGROUND_STREAM_CLIENT_H