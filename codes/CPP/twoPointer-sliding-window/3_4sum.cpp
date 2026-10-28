#include <bits/stdc++.h>
using namespace std;

void threeSum()
{
    int n, k;
    cin >> n;
    vector<int> v;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin >> t;
        v.push_back(t);
    }

    cin >> k;
    sort(v.begin(), v.end());

    for (int i = 0; i < n - 1; i++)
    {
        int l = i + 1, r = n - 1;
        while (l < r)
        {
            int cur_sum = v[i] + v[l] + v[r];
            if (cur_sum == k)
            {
                cout << v[i] << ' ' << v[l] << ' ' << v[r] << endl;
                l++, r--;
                while (l < r && v[l] == v[l - 1])
                    l++;

                while (l < r && v[r] == v[r + 1])
                    r--;
            }
            else if (cur_sum < k)
            {
                l++;
            }
            else if (cur_sum > k)
            {
                r--;
            }
        }
    }
}

void fourSum()
{
    int n, k;
    cin >> n;
    vector<int> v;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin >> t;
        v.push_back(t);
    }

    cin >> k;
    sort(v.begin(), v.end());

    for (int i = 0; i < n - 2; i++)
    {
        if (i > 0 && v[i] == v[i - 1]) // important
            continue;
        for (int j = i + 1; j < n - 1; j++)
        {
            if (j > i + 1 && v[j] == v[j - 1]) // important
                continue;

            int l = j + 1, r = n - 1;
            while (l < r)
            {
                int cur_sum = v[i] + v[j] + v[l] + v[r];
                if (cur_sum == k)
                {
                    cout << v[i] << ' ' << v[j] << ' ' << v[l] << ' ' << v[r] << endl;
                    l++, r--;
                    while (l < r && v[l] == v[l - 1])
                        l++;

                    while (l < r && v[r] == v[r + 1])
                        r--;
                }
                else if (cur_sum < k)
                {
                    l++;
                }
                else if (cur_sum > k)
                {
                    r--;
                }
            }
        }
    }
}

int main()
{
    fourSum();
}