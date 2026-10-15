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
    int l = 0, r = n - 1, ans = -1;
    while (l <= r)
    {
        int mid = (l + r) / 2;
        if (v[mid] == k)
        {
            ans = mid;
            break;
        }
        if (v[l] == v[mid] && v[mid] == v[r]) // this is for the search in sorted array 2 , to handle duplicates
        // 3 3 1 2 3 3 3 3 , k=1
        {
            l++, r--;
            continue;
        }
        if (v[l] <= v[mid])
        {
            if (v[l] <= k && k <= v[mid])
            {
                r = mid - 1;
            }
            else
            {
                l = mid + 1;
            }
        }
        else
        {
            if (v[mid] <= k && k <= v[r])
            {
                l = mid + 1;
            }
            else
            {
                r = mid - 1;
            }
        }
    }
    cout << ans << endl;
}