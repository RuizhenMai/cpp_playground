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
    std::unordered_map<std::string, std::vector<int>> m_;
    std::unordered_map<std::string, int> idx_;
    std::unordered_map<std::string, int> ready_idx_;
    std::vector<std::string> stream_ids;
    std::unordered_map<std::string, std::thread> asyncs_;

    std::unordered_map<std::string, StreamClient> scs_;

public:
    explicit StreamManager(const std::vector<std::vector<int>>& vv) {
        for (int i = 0; i < vv.size(); ++i) {
            std::string s_id = std::to_string(i);
            m_[s_id] = vv[i];
            idx_[s_id] = 0;
            ready_idx_[s_id] = 0;
            stream_ids.emplace_back(s_id);
        }
    }

    void Request(const std::string& stream_id) {
        // all data requested
        if (ready_idx_[stream_id] == m_[stream_id].size()) {
            return;
        }

        // read data asynchronously
        auto fetch = [&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ready_idx_[stream_id] += 1;
        };
        asyncs_[stream_id] = std::thread(fetch);
    }


    bool AllReady() const {
        return std::all_of(stream_ids.cbegin(), stream_ids.end() + 1, [&](const std::string& id) {
            return ready_idx_.at(id) >= idx_.at(id);
        });
    }

    int Next(const std::string& stream_id) {
        if (ready_idx_[stream_id] < idx_[stream_id]) {
            asyncs_[stream_id].join();
        }
        // check ready_idx_ must be >= idx_
        return m_[stream_id][idx_[stream_id]++];
    }

    int HasNext(const std::string& stream_id) {
        return idx_[stream_id] < m_[stream_id].size();
    }
};



#endif //PLAYGROUND_STREAM_MANAGER_H