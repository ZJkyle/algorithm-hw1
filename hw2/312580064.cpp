#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>

using namespace std;
const int Max_n = 501;
int cost[Max_n][Max_n]; // min cost from i to j key e.g. cost[0][0] = freq[0]
int root[Max_n][Max_n]; // best root from i to j.
int freqsum[Max_n+1];
vector<int> keys(Max_n), freq(Max_n);


int freqsuminterval (int i, int j){  // calculate sum of freq[i]~freq[j]
    return freqsum[j+1] - freqsum[i];
}

struct treenode{
    int key;
    treenode* leftnode;
    treenode* rightnode;
    treenode(int val): key(val), leftnode(nullptr), rightnode(nullptr){} // constructor: Node* root = new Node(10); -> key = 10
};

 // task: buildtree(i, j) to find best root and contrust left, right subtree recursively  
treenode* buildtree (int i, int j){
    // end cond: if i<j
    if(i>j) return nullptr;

    // for interval i~j, best root r = root[i][j] and build node(key[r]) 
    int r = root[i][j];
    treenode* node = new treenode(keys[r]);
    node->leftnode = buildtree(i, r-1);
    node->rightnode = buildtree(r+1, j);

    // contrust left, right subtree recursively
    return node;
}

int main (){
    int t; 
    cin >> t;
    while (t--){ // do until T rounds 
        int n;
        cin >> n;
        for (int i = 0; i < n; i++) cin >> keys[i];
        for (int i = 0; i < n; i++) cin >> freq[i];
        // construct freq sum table for calculations
        freqsum[0] = 0;
        for (int i =0;i<n;i++){
            freqsum[i+1] = freqsum[i]+freq[i];
        }
        // DP special cases: 
        for(int i = 0; i < n; i++){
            // single key
            cost[i][i] = freq[i]; 
            root[i][i] = i;
            // illegal issue 
            for (int j=0; j <n ;j++){
                if(i>j) cost[i][j]=0;
            }
        }
        // DP main logic
        // Sub A: interval length control
        int leftcost, rightcost, fsum, totalcost;
        for (int len = 1; len<=n-1; len++){ // control length of interval 
            for(int i=0; i <n-len; i++){  // caution: avoid make i overrange
                int j = i+len;
                // Sub B: 
                // find min cost[i][j]
                // initial cost[i][j]= inf
                // calculate total cost = = min { dp[i][r-1] + dp[r+1][j] + sum(freq[i...j]) }
                cost[i][j] = numeric_limits<int>::max();
                for(int r=i;r<=j;r++ ){                    
                    leftcost = cost[i][r-1];
                    rightcost = cost[r+1][j];
                    fsum = freqsuminterval(i, j);
                    totalcost = leftcost+rightcost+fsum;
                    if(cost[i][j]>totalcost) {
                        cost[i][j] = totalcost; // min cost for i~j
                        root[i][j] = r; // best root for i~j
                    }
                }
            }    
        }
        // rebuild OBST and output 
        treenode* node = buildtree(0, n-1); // this will build OBST tree
        // task: use queue to go through the tree, and store output to result
        queue<treenode*> q;
        vector<string> result; 
        q.push(node); 
        while(!q.empty()){
            treenode* first = q.front(); // first node(root) in queue 
            q.pop();

            if(first == nullptr){
                result.push_back("-1");
            } else {
                result.push_back(to_string(first->key));
                q.push(first->leftnode);
                q.push(first->rightnode);
            }
        }
        while(!result.empty()&& result.back()=="-1"){
            result.pop_back();
        }
        for(int i=0;i<result.size();i++){
            cout<<result[i]<<" ";
        }
        cout <<"\n";
    }

}