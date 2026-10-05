#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

// 3 1 2 4 , k=3
// r=0,l=0 sum=3 , maxL=1
// r=1,l=0 sum=4 -> sum = 1 , l=1 
// r=2,l=1  sum = 3 , maxl=2
// r=3 , l=1 sum= 7 , sum = 6 , l = 2 , sum = 4 , l = 3
// ans = 2

void slidingWindow()
{
    int n;
    cin>>n;
    vector<int> arr;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin>>t;
        arr.push_back(t);
    }

    int k;
    cin>>k;
    int maxL=-1,sum=0;

    int l=0,r=0;
    while(r<n){
        sum+=arr[r];
        while(sum>k && l<r){
            sum-=arr[l];
            l++;
        }

        if(sum==k){
            maxL=max(maxL,r-l+1);
        }
        r++;
    }
    cout<<maxL<<endl;
    
}
void prefixSumForNegative(){
       int n;
    cin>>n;
    vector<int> arr;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin>>t;
        arr.push_back(t);
    }

    int k;
    cin>>k;
    int sum=0,maxl=-1;
    unordered_map<int,int>o;


    // 2 0 0 0 3 , k = 3 

    for(int i=0;i<n;i++){
        sum+=arr[i];
        if(o.find(sum-k)!=o.end()){
            maxl=max(maxl,i-o[sum-k]);
        }
        if(o.find(sum-k)==o.end()){
            o[sum-k]=i;
        }
    }
    cout<<maxl<<endl;
}