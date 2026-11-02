#include <bits/stdc++.h>
using namespace std;

void helper(int ind, int n, string &digits, string &ds, vector<string> &ans,
            vector<string> &mapp)
{
    if (ind == n)
    {
        ans.push_back(ds);
        return;
    }
    string cur = mapp[digits[ind] - '0'];
    for (char c : cur)
    {
        ds.push_back(c);
        helper(ind + 1, n, digits, ds, ans, mapp);
        ds.pop_back();
    }
}
vector<string> letterCombinations(string digits)
{
    vector<string> mapp = {"", "", "abc", "def", "ghi",
                           "jkl", "mno", "pqrs", "tuv", "wxyz"};
    vector<string> ans;
    string ds;
    helper(0, digits.size(), digits, ds, ans, mapp);
    return ans;
}

int main()
{
    string s;
    getline(cin, s);
    vector<string> ans = letterCombinations(s);
    for (string str : ans)
    {
        cout << str << endl;
    }
}