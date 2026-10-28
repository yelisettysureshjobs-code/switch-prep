// binary search

#include <bits/stdc++.h>
using namespace std;

bool possible(vector<int> &v, int n, int m, int k)
{
    int currentCount = 0, currentPages = 0;
    for (int i = 0; i < n; i++)
    {
        if (currentPages + v[i] <= k)
        {
            currentPages += v[i];
        }
        else
        {
            currentCount++;
            currentPages = v[i];
        }
    }
    if (currentPages > 0)
    {
        currentCount++;
    }
    return currentCount <= m;
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
    int m;
    cin >> m;
    int l = v[n - 1];
    int s = 0;
    for (int i = 0; i < n; i++)
    {
        s += v[i];
    }

    int r = s;
    int ans = 1e9;
    while (l <= r)
    {
        int mid = l + (r - l) / 2;
        if (possible(v, n, m, mid))
        {
            ans = min(ans, mid);
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }
    cout << ans << endl;
}