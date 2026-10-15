#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<int> v(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }
    int maxc = 0;
    unordered_set<int> o(v.begin(), v.end());
    for (int i = 0; i < n; i++)
    {
        if (o.find(v[i] - 1) == o.end())
        {
            int c = 0;
            int t = v[i];
            while (o.find(t) != o.end())
            {
                c++;
                t++;
            }
            maxc = max(maxc, c);
        }
    }
    cout << maxc << endl;
}