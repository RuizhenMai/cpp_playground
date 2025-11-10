#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <unistd.h>

#include "stream_client.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

using namespace std;
struct ListNode {
    int val;
    shared_ptr<ListNode*> next;
    explicit ListNode(int x) : val(x) {}
};


vector<int> merge_k_sorted_stream(StreamClient& sc, const vector<string>& stream_ids) {
    auto cmp = [](const pair<string, int>& lhs, const pair<string, int>& rhs) {
        return lhs.second > rhs.second;
    };

    priority_queue<pair<string, int>, vector<pair<string,int>>, decltype(cmp)> pq(cmp);

    vector<int> ans;
    // request data
    for (const string& stream_id : stream_ids) {
        sc.Request(stream_id);
    }

    // prepare all initial data
    for (const string& stream_id : stream_ids) {
        pq.emplace(stream_id, sc.Next(stream_id));
    }

    while (!pq.empty()) {
        const auto [stream_id, val] = pq.top(); pq.pop();
        ans.emplace_back(val);
        if (sc.HasNext(stream_id)) {
            sc.Request(stream_id);
            pq.emplace(stream_id, sc.Next(stream_id));
        }
    }

    return ans;
}

class ParallelMerger {

};
vector<int> parallel_merge_k_sorted_stream(StreamClient& sc, const vector<string>& stream_ids) {
    auto cmp = [](const pair<string, int>& lhs, const pair<string, int>& rhs) {
        return lhs.second > rhs.second;
    };

    priority_queue<pair<string, int>, vector<pair<string,int>>, decltype(cmp)> pq(cmp);

    vector<int> ans;
    // init request data
    for (const string& stream_id : stream_ids) {
        sc.Request(stream_id);
    }

    // prepare all initial data
    for (const string& stream_id : stream_ids) {
        pq.emplace(stream_id, sc.Next(stream_id));
    }

    auto polling = [&](const string& stream_id) {
      sc.Request(stream_id);

    };

    while (!pq.empty()) {
        const auto [stream_id, val] = pq.top(); pq.pop();
        ans.emplace_back(val);
        if (sc.HasNext(stream_id)) {
            sc.Request(stream_id);
            pq.emplace(stream_id, sc.Next(stream_id));
        }
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

    StreamClient sc(vv);

    auto start = std::chrono::high_resolution_clock::now();
    vector<int> k_sorted_stream = parallel_merge_k_sorted_stream(sc, {"0","1","2"});
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << duration.count() << endl;

    return 0;
}

int main() {
    pipe_playground();


    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}