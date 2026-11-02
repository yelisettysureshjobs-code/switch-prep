
#include <bits/stdc++.h>
using namespace std;

void generateParenthesis(int cc, int oc, int n, string &s, vector<string> &ans)
{
    if (oc > n || cc > n)
    {
        return;
    }

    if (oc == n && cc == n)
    {
        ans.push_back(s);
        return;
    }

    s.push_back('(');
    generateParenthesis(cc, oc + 1, n, s, ans);
    s.pop_back();
    if (oc > cc)
    {
        s.push_back(')');
        generateParenthesis(cc + 1, oc, n, s, ans);
        s.pop_back();
    }
}
vector<string> helper(int n)
{
    vector<string> ans;
    string s;
    generateParenthesis(0, 0, n, s, ans);
    return ans;
}
int main()
{
    int n;
    cin >> n;
    vector<string> ans = helper(n);
    for (const string &s : ans)
    {
        cout << s << '\n';
    }
}