// sliding window
#include <bits/stdc++.h>
using namespace std;

int main()
{

    string s;
    int k;
    cin >> k;
    cin >> s;
    int l = 0, ans = -1;
    unordered_map<char, int> o;
    for (int i = 0; i < s.size(); i++)
    {
        o[s[i]]++;
        while (o.size() > k)
        {
            if (o[s[l]] == 1)
                o.erase(s[l]);
            else
                o[s[l]]--;
            l++;
        }
        ans = max(ans, i - l + 1);
    }
    cout << ans << endl;
}