#include <bits/stdc++.h>
using namespace std;

int main()
{
    int m, n;
    cin >> m >> n;
    vector<int> nums1(m + n, 0);
    vector<int> nums2(n, 0);

    for (int i = 0; i < m; i++)
    {
        cin >> nums1[i];
    }
    for (int i = 0; i < n; i++)
    {
        cin >> nums2[i];
    }

    int i = m - 1, j = n - 1, k = m + n - 1;
    while (j >= 0)
    {
        if (i >= 0 && nums1[i] > nums2[j])
        {
            nums1[k--] = nums1[i--];
        }
        else
        {
            nums1[k--] = nums2[j--];
        }
    }
    for (int i = 0; i < m + n; i++)
    {
        cout << nums1[i] << ' ';
    }
}