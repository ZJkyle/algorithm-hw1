#include <iostream>
#include <vector>
using namespace std;
const int Max_n = 501;
int cost[Max_n][Max_n]; // min cost from i to j key e.g. cost[0][0] = freq[0]
int root[Max_n][Max_n]; // 

int main (){
    int t; 
    cin >> t;
    while (t--){
        int n;
        cin >> n;
        vector<int> keys(n), freq(n);
        for (int i = 0; i < n; i++) cin >> keys[i];
        for (int i = 0; i < n; i++) cin >> freq[i];


    }

}