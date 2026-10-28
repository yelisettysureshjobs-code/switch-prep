// sliding window

#include <bits/stdc++.h>
using namespace std;

int helper(vector<int> &v, int n, int k)
{
    int l = 0, r = 0, s = 0, c = 0;
    while (r < n)
    {
        if (v[r] % 2 == 1)
        {
            s++;
        }
        while (s > k)
        {
            if (v[l] % 2 == 1)
            {
                s--;
            }
            l++;
        }
        c += (r - l + 1);
        r++;
    }
    return c;
}

int main()
{
    int n, k;
    cin >> n >> k;
    vector<int> v(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }

    cout << helper(v, n, k) - helper(v, n, k - 1) << endl;
}