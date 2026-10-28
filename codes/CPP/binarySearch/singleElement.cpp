// binary search

#include <bits/stdc++.h>
using namespace std;

int main()
{

    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }
    if (n == 1)
        return v[0];
    if (v[0] != v[1])
    {
        return v[0];
    }
    if (v[n - 1] != v[n - 2])
    {
        return v[n - 1];
    }
    int l = 1, r = n - 2;
    while (l <= r)
    {
        int mid = (l + r) / 2;
        if (v[mid] != v[mid - 1] && v[mid] != v[mid + 1])
        {
            cout << v[mid] << endl;
            break;
        }
        else if (mid % 2 != 0)
        {
            if (v[mid - 1] == v[mid])
            {
                l = mid + 1;
            }
            else
            {
                r = mid - 1;
            }
        }
        else
        {
            if (v[mid - 1] == v[mid])
            {
                r = mid - 1;
            }
            else
            {
                l = mid + 1;
            }
        }
    }
}