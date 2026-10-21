// binary search

#include <bits/stdc++.h>
using namespace std;

int upperBound(vector<int> &v, int n, int e)
{
    int l = 0, r = n - 1;
    int ind = n;
    while (l <= r)
    {
        int mid = (l + r) / 2;
        if (e < v[mid])
        {
            ind = mid;
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }
    return ind;
}
int lowerBound(vector<int> &v, int n, int e)
{
    // int l = 0, r = n - 1;
    // int ind = -1;
    // while (l <= r)
    // {
    //     int mid = (l + r) / 2;
    //     if (e <= v[mid])
    //     {
    //         ind = mid;
    //         r = mid - 1;
    //     }
    //     else
    //     {
    //         l = mid + 1;
    //     }
    // }
    // if (ind == -1)
    //     return n;
    // return ind;
    return lower_bound(v.begin(), v.end(), e) - v.begin();
}
int upperBoundCount(vector<vector<int>> &v, int n, int m, int e)
{
    int c = 0;

    for (int i = 0; i < n; i++)
    {
        c += upperBound(v[i], m, e);
    }

    return c;
}

int main()
{

    int n, m, f = 0;
    cin >> n >> m;
    vector<vector<int>> v(n, vector<int>(m, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> v[i][j];
        }
    }
    int l = INT_MAX, r = INT_MIN, medianCount = (n * m) / 2;
    for (int i = 0; i < n; i++)
    {
        l = min(l, v[i][0]);
        r = max(r, v[i][m - 1]);
    }

    cout << medianCount << endl;
    while (l <= r)
    {
        int mid = (l + r) / 2;
        int currentCount = upperBoundCount(v, n, m, mid);
        // cout << currentCount << i << j << endl;
        if (currentCount <= medianCount)
        {
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }

    cout << l << endl;
}