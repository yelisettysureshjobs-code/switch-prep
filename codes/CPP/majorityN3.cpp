#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;
class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        int fe=-1,fc=0;
        int se=-1,sc=0;
        for(int i:nums){
            if(fc==0&&se!=i){
                fe=i,fc=1;
            }
            else if(sc==0&&fe!=i){
                se=i,sc=1;
            }
            else if(fe==i){
                fc++;
            }
            else if(se==i){
                sc++;
            }
            else{
                fc--,sc--;
            }
            
        }
        int tc1=0,tc2=0;
        for(int i:nums){
            if(i==fe){
                tc1++;
            }
            else if(i==se){
                tc2++;
            }
        }
        vector<int>ans;
        int n=nums.size();
        if(floor(n/3)<tc1){
            ans.push_back(fe);
        }
        if(floor(n/3)<tc2){
            ans.push_back(se);
        }
        return ans;
    }
};