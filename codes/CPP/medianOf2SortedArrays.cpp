// binary search

#include <bits/stdc++.h>
using namespace std;

double median(vector<int> a, vector<int> b, int n1, int n2)
{
    if (n1 > n2)
        return median(b, a, n2, n1);
    int mid = (n1 + n2 + 1) / 2;
    int l = 0, r = n1;
    while (l <= r)
    {
        int mid1 = (l + r) / 2;
        int mid2 = mid - mid1;
        int l1 = INT_MIN;
        int l2 = INT_MIN;
        int r1 = INT_MAX;
        int r2 = INT_MAX;
        if (mid1 < n1)
            r1 = a[mid1];
        if (mid2 < n2)
            r2 = b[mid2];
        if (mid1 - 1 >= 0)
            l1 = a[mid1 - 1];
        if (mid2 - 1 >= 0)
            l2 = b[mid2 - 1];
        if (l1 < r2 && l2 < r1)
        {
            if ((n1 + n2) % 2 == 0)
            {
                return ((double)max(l1, l2) + min(r1, r2)) / 2.0;
            }
            else
            {
                return (double)max(l1, l2);
            }
        }
        else if (l1 > r2)
        {
            r = mid1 - 1;
        }
        else
        {
            l = mid1 + 1;
        }
    }
    return -1;
}

int main()
{

    int n1, n2;
    cin >> n1 >> n2;
    vector<int> a(n1, 0);
    vector<int> b(n2, 0);
    for (int i = 0; i < n1; i++)
    {
        cin >> a[i];
    }

    for (int i = 0; i < n2; i++)
    {
        cin >> b[i];
    }

    cout << median(a, b, n1, n2) << endl;
}