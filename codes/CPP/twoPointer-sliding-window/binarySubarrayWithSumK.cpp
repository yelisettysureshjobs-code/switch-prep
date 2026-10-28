// sliding window

#include <bits/stdc++.h>
using namespace std;

int atMostK(vector<int> &v, int n, int k)
{
    int l = 0, r = 0, sum = 0, cnt = 0;
    while (r < n)
    {
        sum += v[r];
        while (sum > k)
        {
            sum -= v[l];
            l++;
        }
        cnt += (r - l + 1);
        r++;
    }
    return cnt;
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
    cout << atMostK(v, n, k) - atMostK(v, n, k - 1) << endl;
}