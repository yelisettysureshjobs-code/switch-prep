#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int num=-1,c=0;
        for(int i:nums){
            if(num==i){
                c++;
            }
            if(num!=i){
                if(c==0){
                    num=i;
                    c=1;
                }else{
                    c--;
                }
            }
        }
        return num;
    }
};