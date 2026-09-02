#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


void withoutNegativeOrZeros(){
    int n, k;
    cin >> n >> k;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int s = 0, ans = 0, maxl = -1;

    for (int e = 0; e < n; e++) {
        ans += arr[e];

        while (ans > k && s <= e) {
            ans -= arr[s];
            s++;
        }

        if (ans == k) {
            maxl = max(maxl, e - s + 1);
        }
    }

    cout << maxl << endl;

}
void withNegativeOrZeros(){

    int n, k;
    cin >> n >> k;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    unordered_map<int,int>om;
    int maxl = 0,s=0;

    for(int i=0;i<n;i++){
        s+=arr[i];
        if(s==k){
            maxl=max(maxl,i+1);
        }
        int rem=s-k;
        if(om.find(rem)!=om.end()){
            maxl=max(maxl,i-om[rem]);
        }
        if(om.find(s)==om.end()){
            om[s]=i;
        }
    }

    cout << maxl << endl;

}

int main()
{
    // Call the function to find the longest subarray with sum equal to k without negative numbers or zeros
    withoutNegativeOrZeros();
    // Call the function to find the longest subarray with sum equal to k with negative numbers or zeros
    withNegativeOrZeros();

    return 0;

}