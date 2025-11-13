#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <unistd.h>

#include "stream_client.h"
#include "stream_manager.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

using namespace std;
struct ListNode {
    int val;
    shared_ptr<ListNode*> next;
    explicit ListNode(int x) : val(x) {}
};


vector<int> merge_k_sorted_stream(StreamManager& sm, const vector<string>& stream_ids) {
    auto cmp = [](const pair<string, int>& lhs, const pair<string, int>& rhs) {
        return lhs.second > rhs.second;
    };

    priority_queue<pair<string, int>, vector<pair<string,int>>, decltype(cmp)> pq(cmp);

    vector<int> ans;
    // request data
    for (const string& stream_id : stream_ids) {
        sm.Request(stream_id);
    }

    // prepare all initial data
    for (const string& stream_id : stream_ids) {
        pq.emplace(stream_id, sm.Next(stream_id));
    }

    while (!pq.empty()) {
        const auto [stream_id, val] = pq.top(); pq.pop();
        ans.emplace_back(val);
        if (sm.HasNext(stream_id)) {
            sm.Request(stream_id);
            pq.emplace(stream_id, sm.Next(stream_id));
        }
    }

    return ans;
}

class ParallelMerger {

};
vector<int> parallel_merge_k_sorted_stream(StreamManager& sm, const vector<string>& stream_ids) {
    auto cmp = [](const pair<string, int>& lhs, const pair<string, int>& rhs) {
        return lhs.second > rhs.second;
    };

    priority_queue<pair<string, int>, vector<pair<string,int>>, decltype(cmp)> pq(cmp);

    vector<int> ans;
    unordered_map<string, thread> threads;
    mutex m;
    unordered_map<string, int> count;

    for (const string& stream_id : stream_ids) {
        threads[stream_id] = thread([&]() {
            while (sm.HasNext(stream_id)) {
                sm.Request(stream_id);
                int val = sm.Next(stream_id);
                {
                    lock_guard<mutex> lock(m);
                    pq.emplace(stream_id, val);
                    ++count[stream_id];
                }
            }


        });
    }

    while (true) {
        bool pq_has_all_stream = false;
        bool should_exit = false;
        {
            lock_guard<mutex> lock(m);
            pq_has_all_stream = std::all_of(count.cbegin(), count.cend(), [&](const auto& e) {
                    return e.second > 0 || !sm.HasNext(e.first);
                });
            // 注意这个should_exit，很大概率为true，但这个时候实际上pq不应该是empty，在并发往里面写数据，但有lock，很容易就对不上了
            // should_exit = std::none_of(count.cbegin(), count.cend(), [&](const auto& e) {
            //         return sm.HasNext(e.first);
            //     });
            if (!pq.empty() && pq_has_all_stream) {
                const auto [stream_id, val] = pq.top(); pq.pop();
                ans.emplace_back(val);
                --count[stream_id];
            }
        }

        // this_thread::sleep_for(chrono::milliseconds(500));
        printf("pq size: %ld, all stream: %d\n", pq.size(), pq_has_all_stream);
        for (const string& stream_id : stream_ids) {
            printf("id: %s count in pq: %d, idx in sc: %d\n", stream_id.c_str(), count[stream_id], sm.Idx(stream_id));
        }
        for (const int val: ans) {
            cout << val << ",";
        }
        cout << endl;
        if (pq.empty() && std::none_of(count.cbegin(), count.cend(), [&](const auto& e) {
            return sm.HasNext(e.first);
            })) {
            break;
        }
    }


    for (const string& stream_id : stream_ids) {
        threads[stream_id].join();
    }
    return ans;
}


int pipe_playground() {
    int fd[2]; // file descriptor
    pipe(fd);

    // create a new process
    if (fork() == 0) {
        // child
        close(fd[0]); // close read end
        const char* content = "hello world from child~";
        write(fd[1], content, strlen(content));
    } else {
        close(fd[1]); // close write end
        char buf[64];
        read(fd[0], buf, sizeof(buf));
        cout << "Main Process received: " << buf << endl;
    }




    return 0;
}

int merge_k_sorted_stream_playground() {
    vector<vector<int>> vv = {
        {1,4,5},
        {1,3,4},
        {2,6}
    };

    StreamManager sm(vv);

    auto start = std::chrono::high_resolution_clock::now();
    vector<int> k_sorted_stream = parallel_merge_k_sorted_stream(sm, {"0","1","2"});
    for (const int val: k_sorted_stream) {
        cout << val << ",";
    }
    cout << endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << duration.count() << endl;

    return 0;
}

int main() {
    merge_k_sorted_stream_playground();


    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}