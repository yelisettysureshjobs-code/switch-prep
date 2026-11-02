#include <bits/stdc++.h>
using namespace std;

void helper(int ind, int n, string &s, vector<string> &ans)
{
    if (ind == n)
    {
        ans.push_back(s);
        return;
    }

    for (int i = ind; i < n; i++)
    {
        swap(s[i], s[ind]);
        helper(ind + 1, n, s, ans);
        swap(s[i], s[ind]);
    }
}

int main()
{
    string s;
    getline(cin, s);
    vector<string> ans;
    helper(0, s.size(), s, ans);
    for (string str : ans)
    {
        cout << str << endl;
    }
}