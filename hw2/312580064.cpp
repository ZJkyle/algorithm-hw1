// Optimal binary search tree
// Target: minimize Total cost = summation of frequency * level
/*
input format 
T = number of test cases
N = number of keys
k1 k2 ... kN
f1 f2 ... fN
Constraints
1 ≤ T ≤ 100
1 ≤ N ≤ 500
1 ≤ ki ≤ 10000
1 ≤ fi ≤ 10000
The keys are distinct.
*/

#include <bits/stdc++.h>
using namespace std;

// dp[i][j]: keys[i..j] 最佳化子樹的最小成本
// root[i][j]: 使 dp[i][j] 最小的那個根節點索引
int dp[501][501], rootIdx[501][501];
int prefixFreq[502];  // prefixFreq[i] = freq[0] + ... + freq[i-1]

int sumFreq(int i, int j) {
    return prefixFreq[j+1] - prefixFreq[i];
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; 
    cin >> T;
    while (T--) {
        int N; 
        cin >> N;
        vector<int> keys(N), freq(N);
        for(int i=0;i<N;i++) cin>>keys[i];
        for(int i=0;i<N;i++) cin>>freq[i];

        // 前置：建 prefixFreq
        prefixFreq[0] = 0;
        for(int i=0;i<N;i++) 
            prefixFreq[i+1] = prefixFreq[i] + freq[i];

        // 初始化
        for(int i=0;i<N;i++){
            dp[i][i] = freq[i];
            rootIdx[i][i] = i;
        }
        for(int i=0;i<N;i++){
            dp[i][i-1] = 0;   // 空區間
        }

        // DP
        for(int len=2; len<=N; len++){
            for(int i=0; i+len-1 < N; i++){
                int j = i+len-1;
                dp[i][j] = numeric_limits<int>::max();
                // Knuth 優化（可選，若 keys 已有單調性可加速到 O(N³)→O(N²)）
                for(int r = rootIdx[i][j-1]; r <= rootIdx[i+1][j]; r++){
                    int costLeft  = dp[i][r-1];
                    int costRight = dp[r+1][j];
                    int costAll   = sumFreq(i,j);  
                    int total = costLeft + costRight + costAll;
                    if(total < dp[i][j]){
                        dp[i][j] = total;
                        rootIdx[i][j] = r;
                    }
                }
            }
        }

        // 重建樹，然後做層序遍歷輸出
        // 我們用一個簡單的節點結構 + queue
        struct Node { int key; Node *l,*r; Node(int k):key(k),l(nullptr),r(nullptr){} };
        function<Node*(int,int)> build = [&](int i,int j)->Node* {
            if(i>j) return nullptr;
            int r = rootIdx[i][j];
            Node* node = new Node(keys[r]);
            node->l = build(i, r-1);
            node->r = build(r+1, j);
            return node;
        };
        Node* root = build(0, N-1);

        // 層序遍歷並輸出
        queue<Node*> q;
        q.push(root);
        vector<string> out;
        while(!q.empty()){
            Node* u = q.front(); q.pop();
            if(u){
                out.push_back(to_string(u->key));
                q.push(u->l);
                q.push(u->r);
            } else {
                out.push_back("-1");
            }
        }
        // 去掉尾端多餘的 -1
        while(!out.empty() && out.back() == "-1")
            out.pop_back();

        // 輸出一行
        for(auto &s: out) 
            cout << s << ' ';
        cout << "\n";

        // (若要避免 leak，可遞迴 delete Tree，但競程裡常略過)
    }

    return 0;
}
