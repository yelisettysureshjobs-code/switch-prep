// sliding window

// #include <bits/stdc++.h>
// using namespace std;

// int main()
// {
//     string s;
//     cin >> s;
//     vector<int> v(3, -1);
//     int cnt = 0;
//     for (int i = 0; i < s.size(); i++)
//     {
//         v[s[i] - 'a'] = i;
//         cnt = cnt + 1 + min(v[0], min(v[1], v[2]));
//     }
//     cout << cnt << endl;
// }

#include <bits/stdc++.h>
using namespace std;

int helper(string s, int n, int k)
{
    int l = 0, r = 0, c = 0;
    unordered_map<int, int> o;
    while (r < n)
    {
        o[s[r]]++;
        while (o.size() > k)
        {
            if (o[s[l]] == 1)
            {
                o.erase(s[l]);
            }
            else
            {
                o[s[l]]--;
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
    string s;
    cin >> s;
    cout << helper(s, s.size(), 3) << " " << helper(s, s.size(), 2) << endl;
}