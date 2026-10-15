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
    int l = 0, r = n - 1;
    while (l <= r)
    {
        int mid = (l + r) / 2;
        if (v[mid - 1] < v[mid] && v[mid] > v[mid + 1])
        {
            cout << v[mid] << endl;
            break;
        }
        else if (v[mid - 1] < v[mid] && v[mid] < v[mid + 1])
        {
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }
}