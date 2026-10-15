// binary search
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
    int r = n - 1, c = 0;
    while (r >= 0 && c < m)
    {
        if (v[r][c] == k)
        {
            cout << r << ' ' << c << endl;
            f = 1;
            break;
        }
        else if (v[r][c] > k)
        {
            r--;
        }
        else
        {
            c++;
        }
    }
    if (!f)
    {
        cout << "Not Found" << endl;
    }
}