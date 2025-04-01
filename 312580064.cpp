#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;


// Precision: error must <= 10^-4   -->  use double data type
// Test number up to 100000 --> use O(nlogn) algorithm
// No (xi=xj and yi=yj), but there exists xi=xj, or yi=yj.



// 2D similarity calculation: 歐幾里得距離
double similarity(pair<double, double> p1, pair<double, double> p2) {
    return sqrt((p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second));
}

// Find the minimum distance in strip
double closest_cross(vector<pair<double, double>>& strip, double d) {
    double min_d = d; 
    // sorting along y-coor first 
    sort(strip.begin(), strip.end(), [](const pair<double, double>& a, const pair<double, double>& b){
    return a.second < b.second; // 依 y 座標排序
    }); 
    //sort(strip.begin(), strip.end());

    int n = strip.size(); // n = number of points in the strip
    for (int i = 0; i < n; i++) {
        // j = i + 1 = two points. 
        // strip[j].second - strip[i].second = distance between two points, if it's smaller than min_d, then update min_d
        for (int j = i + 1; j < n && (strip[j].second - strip[i].second) < min_d; j++) { 
            min_d = min(min_d, similarity(strip[i], strip[j]));
        }
    }
    return min_d;
}

// Brute Force time complexity = O(n^2)
double bruteForce(vector<pair<double, double>>& pts, int left, int right) {
    // initial min_d = maximum double value
    double min_d = numeric_limits<double>::max();
    for (int i = left; i < right; i++) {
        for (int j = i + 1; j < right; j++) {
            min_d = min(min_d, similarity(pts[i], pts[j]));
        }
    }
    return min_d;
}


// Main recursive function to find min_distance
double closest_pair_recursive(vector<pair<double, double>>& pts, int left, int right) {
    // if the number of points is less than a small number, use force solution.
    if (right - left <= 10) {
        return bruteForce(pts, left, right);
    }
    // Devide and conquer
    double mid = (left + right) / 2;

    // x = first element of the pair
    double mid_x = pts[mid].first;

    // Conquer the rest part.
    double distance_left = closest_pair_recursive(pts, left, mid);
    double distance_right = closest_pair_recursive(pts, mid, right);
    double min_distance = min(distance_left, distance_right);

    // combine: store the points in the strip 
    // put any points that have smaller distance than min_distance into the strip
    vector<pair<double, double>> strip;
    for (int i = left; i < right; i++) {
        if (fabs(pts[i].first - mid_x) < min_distance) { // distance between x and mid_x is smaller than min_distance
            strip.push_back(pts[i]);
        }
    }

    // return the smaller one
    return min(min_distance, closest_cross(strip, min_distance));
}

double find_min_distance(vector<pair<double, double>>& champs) {
    return closest_pair_recursive(champs, 0, champs.size());
}

int main() {
    int T, n;
    double min_distance;
    // 2d vector to store the points
    // Use vector pair to store the data xi, yi
    vector<pair<double, double>> data;
    
    cin >> T; // T = number of test cases
    for (int i = 0; i < T; i++) {
        cin >> n; // n = number of champs 
        // Read n pairs of points
        for (int j = 0; j < n; j++) {
            double x, y;
            cin >> x >> y;
            data.push_back({x, y});
        }
        
        sort(data.begin(), data.end()); // we need to sort along x-axis first, then along y-axis
        min_distance = find_min_distance(data);
        cout << fixed << setprecision(6) << min_distance << endl; // set precision to 6
        data.clear();
    }
    return 0;
}

