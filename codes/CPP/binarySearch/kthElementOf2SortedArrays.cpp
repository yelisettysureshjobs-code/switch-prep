#include <bits/stdc++.h>
using namespace std;

int findCount(vector<int> &a, vector<int> &b, int k)
{
    int c = 0;
    c += upper_bound(a.begin(), a.end(), k) - a.begin();
    c += upper_bound(b.begin(), b.end(), k) - b.begin();
    return c;
}

int main()
{

    int n, m, k;
    cin >> n >> m >> k;
    vector<int> a(n, 0);
    vector<int> b(m, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for (int i = 0; i < m; i++)
    {
        cin >> b[i];
    }

    int l = INT_MAX, r = INT_MIN;
    for (int i : a)
    {
        l = min(l, i);
        r = max(r, i);
    }
    for (int i : b)
    {
        l = min(l, i);
        r = max(r, i);
    }
    while (l <= r)
    {
        int mid = (l + r) / 2;
        int count = findCount(a, b, mid);
        if (count <= k - 1)
        {
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }
    return l;
}