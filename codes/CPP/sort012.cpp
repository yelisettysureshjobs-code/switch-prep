#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;
class Solution {
public:
    void sortColors(vector<int>& nums) {
        int z = 0, t = nums.size() - 1, o = 0;
        while (o <= t) {
            if (nums[o] == 0) {
                swap(nums[z],nums[o]);
                z++,o++;
            } else if (nums[o] == 2) {
                swap(nums[t],nums[o]);
                t--;
            }
            else{
                o++;
            }
        }
        return ;
    }
};