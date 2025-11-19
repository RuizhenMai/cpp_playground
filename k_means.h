//
// Created by Ruizhen Mai on 11/12/25.
//

#ifndef PLAYGROUND_K_MEANS_H
#define PLAYGROUND_K_MEANS_H

#include <vector>
using namespace std;
using Point = vector<double>;


class KMeans {
private:
    vector<vector<double>> points_; // N x F
    vector<vector<double>> paired_; // N x F
    vector<vector<double>> centers_;// k x F
    int K_; // number of centers
    int iters_; // number of iterations on fit

    static double square(double a) {
        return a * a;
    }

public:
    KMeans(const vector<vector<double>>& points, const int k, const int iters): points_(points), K_(k), iters_(iters) {
        for (int i = 0; i < K_; i++) {
            const int center_idx = rand() % points.size();
            centers_.push_back(points_[center_idx]);
        }
    }

    static double l2(const vector<double> &a, const vector<double> &b) {
        // a and b size must be the same
        double ans = 0;
        for (size_t i = 0; i < a.size(); i++) {
            ans += square(a[i] - b[i]);
        }
        return sqrt(ans);
    }


    static vector<vector<double>> distance(const vector<vector<double>>& points, const vector<vector<double>>& centers) {
        vector<vector<double>> ans;
        ans.reserve(points.size());
        for (const vector<double>& p : points) { // for each point
            ans.emplace_back();
            for (const vector<double>& center: centers) { // for each center
                ans[ans.size()-1].push_back(l2(p,center)); // there is a distance
            }
        }
        return ans; // N x k x 1
    }


    void Fit() {
        vector<vector<double>> distances = distance(points_, centers_); // N x k
        vector<int> labels; // N x 1
        for (const vector<double>& distance_per_center : distances) {
            const auto min_it = ranges::min_element(distance_per_center);
            int arg_min_idx = std::distance(distance_per_center.begin(), min_it);
            labels.push_back(arg_min_idx);
        }

        for (int k = 0; k < K_; k++) {
            vector<vector<double>> temp_group;
            for (int i = 0; i < points_.size(); i++) {
                if (labels[i] == k) {

                }
            }
        }
    }
};


#endif //PLAYGROUND_K_MEANS_H