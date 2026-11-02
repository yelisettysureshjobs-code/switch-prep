// recursion

#include <bits/stdc++.h>
using namespace std;

void combinationSum(int ind, int n, vector<int> &v, int t, vector<int> &ds,
                    vector<vector<int>> &ans)
{

    if (ind == n || t <= 0)
    {
        if (0 == t)
        {
            ans.push_back(ds);
        }
        return;
    }

    combinationSum(ind + 1, n, v, t, ds, ans);
    if (v[ind] <= t)
    {
        ds.push_back(v[ind]);
        combinationSum(ind, n, v, t - v[ind], ds, ans);
        ds.pop_back();
    }
    return;
}

void combinationSum2(int ind, int n, vector<int> &v, int t, vector<int> &ds,
                     vector<vector<int>> &ans)
{

    if (ind >= n || t <= 0)
    {
        if (0 == t)
        {
            ans.push_back(ds);
        }
        return;
    }

    if (v[ind] <= t)
    {
        ds.push_back(v[ind]);
        combinationSum2(ind + 1, n, v, t - v[ind], ds, ans);
        ds.pop_back();
    }
    while (ind < n - 1 && v[ind + 1] == v[ind])
    {
        ind++;
    }
    combinationSum2(ind + 1, n, v, t, ds, ans);
    return;
}

vector<vector<int>> helper(vector<int> &v, int t)
{
    vector<vector<int>> ans;
    vector<int> ds;
    sort(v.begin(), v.end()); // no need of this sort and call the other function if a element can be picked unlimited times
    combinationSum2(0, v.size(), v, t, ds, ans);
    return ans;
}

int main()
{
    int n, t;
    cin >> n >> t;
    vector<int> v(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }
    vector<vector<int>> ans = helper(v, t);
}