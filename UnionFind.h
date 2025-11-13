//
// Created by Ruizhen Mai on 11/10/25.
//
#ifndef PLAYGROUND_UNIONFIND_H
#define PLAYGROUND_UNIONFIND_H

#include <vector>

class UnionFind {
private:
    std::vector<int> parents_;
    std::vector<int> rank_;
    int num_roots_;
public:
    explicit UnionFind(int n): num_roots_(n), rank_(std::vector<int>(n, 1)) {
        for (int i = 0; i < n; i++) {
            parents_.push_back(i);
        }
    }
    int Find(int x) {
        // only root will have itself as parent`
        if (parents_[x] != x) {
            parents_[x] = Find(parents_[x]);
        }
        return parents_[x];
    }

    int NumComponents() const {
        return num_roots_;
    }

    void Union(int x, int y) {
        int px = Find(x);
        int py = Find(y);

        if (px == py) {
            return;
        }

        // rank小的挂到rank大的下面
        if (rank_[px] < rank_[py]) {
            parents_[px] = parents_[py];
        } else if (rank_[px] > rank_[py]) {
            parents_[py] = parents_[px];
        } else {
            parents_[py] = parents_[px];
            rank_[px] += 1;
        }
        num_roots_ -= 1;
    }
};
#endif //PLAYGROUND_UNIONFIND_H