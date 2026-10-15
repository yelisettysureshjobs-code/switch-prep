// binary search

#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n, k;
    cin >> n;
    vector<int> v(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }

    cin >> k;
    int l = 0, r = n - 1, ans = 0;
    while (l <= r)
    {
        int mid = (l + (r - l) / 2);
        // if (k <= v[mid]) for lower bound , we need to find the element <= to the given value
        if (k < v[mid]) // for upper bound , we need to find the element < to the given value
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