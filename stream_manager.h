//
// Created by ByteDance on 11/9/25.
//
#include <chrono> // For std::chrono::duration
#include <thread> // For std::this_thread::sleep_for
#include "stream_client.h"

#ifndef PLAYGROUND_STREAM_MANAGER_H
#define PLAYGROUND_STREAM_MANAGER_H

class StreamManager {
private:
    std::unordered_map<std::string, StreamClient> scs_;
    std::vector<std::string> stream_ids_;
    std::unordered_map<std::string, std::thread> threads_;

public:
    explicit StreamManager(const std::vector<std::vector<int>>& vv) {
        for (int i = 0; i < vv.size(); ++i) {
            std::string s_id = std::to_string(i);
            stream_ids_.emplace_back(s_id);
            // StreamClient sc(std::move(s_id), vv[i]);
            // std::pair<std::string, StreamClient> p(std::move(s_id), sc);
            scs_.emplace(std::piecewise_construct, std::forward_as_tuple(s_id), std::forward_as_tuple(s_id, vv[i]));
        }
    }

    void Request(const std::string& stream_id) {
        // all data requested
        scs_.at(stream_id).Request();
    }

    bool Ready(const std::string& stream_id) const {
        return scs_.at(stream_id).Ready();
    }
    //
    // bool AllReady() const {
    //     for (const std::string& stream_id : stream_ids_) {
    //         bool rd =scs_.at(stream_id).Ready();
    //         // std::cout << stream_id << " is ready: " << rd << std::endl;
    //         if (!rd) {
    //             return false;
    //         }
    //     }
    //     std::cout << "all ready!" << std::endl;
    //     return true;
    // }

    int Next(const std::string& stream_id) {
        return scs_.at(stream_id).Next();
    }

    bool HasNext(const std::string& stream_id) const {
        return scs_.at(stream_id).HasNext();
    }

    int Idx(const std::string& stream_id) const {
        return scs_.at(stream_id).idx_;
    }
};



#endif //PLAYGROUND_STREAM_MANAGER_H