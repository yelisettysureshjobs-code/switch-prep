// binary search

#include <bits/stdc++.h>
using namespace std;

bool check(vector<int> v, int n, int h, int k)
{
    int c = 0;
    for (int i = 0; i < n; i++)
    {
        // if (v[i] % k == 0)
        // {
        //     c += (v[i] / k);
        // }
        // else
        // {
        //     c = c + (v[i] / k) + 1;
        // }
        c = c + (v[i] + k - 1) / k; // this handles the over flow condition for large values.
    }
    return c <= h;
}
// 3, 6, 7, 11 , h=8
int main()
{

    int n;
    cin >> n;
    vector<int> v(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }
    int h;
    cin >> h;
    int maxV = v[0], minV = v[0];
    for (int i = 1; i < n; i++)
    {
        maxV = max(maxV, v[i]);
        minV = min(minV, v[i]);
    }

    int l = minV, r = maxV, ans = -1;
    while (l <= r)
    {
        int mid = (l + r) / 2;
        if (check(v, n, h, mid))
        {
            ans = mid;
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }
    cout << ans << endl;
}