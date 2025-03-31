#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Sort the data by x coordinate, use hybrid introsort algorithm, time complexity = O(n log n)
void sort_data(vector<pair<float, float>> &data) {
    sort(data.begin(), data.end());
}

// 2D similarity calculation
float similarity(pair<float, float> p1, pair<float, float> p2) {
    return sqrt((p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second));
}

// Find the minimum distance between two points
float strip_closest(vector<pair<float, float>>& strip, float d) {
    float min_d = d;
    sort(strip.begin(), strip.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    int n = strip.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n && (strip[j].second - strip[i].second) < min_d; ++j) {
            min_d = min(min_d, similarity(strip[i], strip[j]));
        }
    }
    return min_d;
}

float closest_pair_recursive(vector<pair<float, float>>& pts, int left, int right) {
    if (right - left <= 3) {
        float min_d = 1e9;
        for (int i = left; i < right; ++i) {
            for (int j = i + 1; j < right; ++j) {
                min_d = min(min_d, similarity(pts[i], pts[j]));
            }
        }
        sort(pts.begin() + left, pts.begin() + right, [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
        return min_d;
    }

    int mid = (left + right) / 2;
    float mid_x = pts[mid].first;
    float d_left = closest_pair_recursive(pts, left, mid);
    float d_right = closest_pair_recursive(pts, mid, right);
    float d = min(d_left, d_right);

    inplace_merge(pts.begin() + left, pts.begin() + mid, pts.begin() + right,
                  [](const auto& a, const auto& b) {
                      return a.second < b.second;
                  });

    vector<pair<float, float>> strip;
    for (int i = left; i < right; ++i) {
        if (fabs(pts[i].first - mid_x) < d) {
            strip.push_back(pts[i]);
        }
    }

    return min(d, strip_closest(strip, d));
}

float find_min_distance(vector<pair<float, float>> &data) {
    sort(data.begin(), data.end());  // sort by x
    return closest_pair_recursive(data, 0, data.size());
}


int main() {
    int T, n;
    float min_distance;
    // 2d vector to store the points
    // try float first, if the error is too large, try double
    // Use vector pair to store the data xi, yi
    vector<pair<float, float>> data;
    
    cin >> T; // T = number of test cases
    for (int i = 0; i < T; i++) {
        cin >> n; // n = number of champs
        // Read n pairs of cpoints
        for (int j = 0; j < n; j++) {
            float x, y;
            cin >> x >> y;
            data.push_back({x, y});
        }
        
        // Calculate the smallest similarity, like the minimum distance between two points.
        // If using 暴力法, time complexity = O(n^2)
        // Divide: Sort data in x-coordinate and slice data into two parts
        // Conquer: find minimum distance in left/right area
        // Combine: find the minimum distance between two areas
        sort_data(data);
        min_distance = find_min_distance(data);
        cout << min_distance << endl;
        data.clear();
    }
    return 0;
}

