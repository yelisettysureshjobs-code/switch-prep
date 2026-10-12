#include <bits/stdc++.h>
using namespace std;

int main()
{

    int n;
    cin >> n;
    vector<int> v;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin >> t;
        v.push_back(t);
    }
    int pivot = -1;

    // Find pivot
    for (int i = n - 1; i > 0; i--)
    {
        if (v[i - 1] < v[i])
        {
            pivot = i - 1;
            break;
        }
    }

    // No pivot → already the largest permutation
    if (pivot == -1)
    {
        reverse(v.begin(), v.end());
    }
    else
    {
        // Find the rightmost element greater than pivot
        int maxR = n - 1;

        for (int i = maxR; i >= 0; i--)
        {
            if (v[maxR] >= v[pivot])
            {
                break;
            }
        }

        swap(v[pivot], v[maxR]);

        // Reverse suffix
        int l = pivot + 1;
        int r = n - 1;

        while (l < r)
        {
            swap(v[l], v[r]);
            l++;
            r--;
        }
    }

    for (int i = 0; i < n; i++)
    {
        cout << v[i] << ' ';
    }
}