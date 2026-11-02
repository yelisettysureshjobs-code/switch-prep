#include <bits/stdc++.h>
using namespace std;

bool isPlaindrome(int s, int e, string str)
{
    while (s <= e)
    {
        if (str[s++] != str[e--])
        {
            return false;
        }
    }
    return true;
}

void helper(int ind, int n, string &s, vector<vector<string>> &ans,
            vector<string> &ds,
            string &cur)
{

    if (ind == n)
    {
        ans.push_back(ds);
        return;
    }
    for (int i = ind; i < n; i++)
    {
        if (isPlaindrome(ind, i, s))
        {
            ds.push_back(s.substr(ind, i - ind + 1));
            helper(i + 1, n, s, ans, ds, cur);
            ds.pop_back();
        }
    }
}

int main()
{

    string s;
    getline(cin, s);
    vector<vector<string>> ans;
    vector<string> ds;
    string cur;
    helper(0, s.size(), s, ans, ds, cur);
    for (vector<string> v : ans)
    {
        for (string str : v)
        {
            cout << str << ' ';
        }
        cout << endl;
    }
}