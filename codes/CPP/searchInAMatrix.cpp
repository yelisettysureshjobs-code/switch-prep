#include <bits/stdc++.h>
using namespace std;

int main()
{

    int n, m, k, f = 0;
    cin >> n >> m >> k;
    vector<vector<int>> v(n, vector<int>(m, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> v[i][j];
        }
    }
    int l = 0, r = n * m - 1;
    while (l <= r)
    {
        int mid = (l + r) / 2;
        int i = mid / n;
        int j = mid % m;
        if (v[i][j] == k)
        {
            f = 1;
            cout << "found" << endl;
            break;
        }
        else if (v[i][j] < k)
        {
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }
    if (!f)
    {

        cout << "not found" << endl;
    }
}