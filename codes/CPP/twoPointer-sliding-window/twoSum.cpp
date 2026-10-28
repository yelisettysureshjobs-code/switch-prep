#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int>om;
        for(int i=0;i<nums.size();i++){
            om[nums[i]]=i;
        }
        vector<int>ans;
        for(int i=0;i<nums.size();i++){
            if(om.find(target-nums[i])!=om.end() && i!=om[target-nums[i]]){
                ans.push_back(i);
                ans.push_back(om[target-nums[i]]);
                return ans;
            }
        }
        return ans;
    }
};