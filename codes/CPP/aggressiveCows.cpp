// binary search

#include <bits/stdc++.h>
using namespace std;

bool possible(vector<int> &v, int n, int k, int mid)
{
    int c = 1, lastPos = v[0];

    for (int i = 1; i < n; i++)
    {
        if (v[i] - lastPos >= mid)
        {
            c++;
            lastPos = v[i];
        }
        if (c == k)
        {
            return true;
        }
    }
    return c == k;
}

int main()
{
    int n;
    cin >> n;
    vector<int> v(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }
    int k;
    cin >> k;
    sort(v.begin(), v.end());
    int l = v[1] - v[0];
    int r = v[n - 1] - v[0], ans = -1;
    while (l <= r)
    {
        int mid = l + (r - l) / 2;
        if (possible(v, n, k, mid))
        {
            ans = mid;
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }
    cout << ans << endl;
}