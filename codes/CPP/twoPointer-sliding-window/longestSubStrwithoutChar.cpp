// sliding window
#include <bits/stdc++.h>
using namespace std;

int main()
{
    string s;
    getline(cin, s);
    unordered_map<char, int> o;
    int l = 0, r = 0, ans = 0;
    while (r < s.size())
    {

        if (o.find(s[r]) != o.end())
        {
            if (l <= o[s[r]])
            {
                l = o[s[l]] + 1;
            }
        }
        o[s[r]] = r;
        ans = max(ans, r - l + 1);
        r++;
    }
    cout << ans << endl;
}