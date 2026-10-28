// sliding window

#include <bits/stdc++.h>
using namespace std;

int main()
{
    string s;
    cin >> s;
    int k;
    cin >> k;
    int l = 0, r = 0, maxL = -1, maxFreq = 0;
    unordered_map<char, int> o;
    while (r < s.size())
    {
        o[s[r]]++;
        maxFreq = max(maxFreq, o[s[r]]);
        while ((r - l + 1) - maxFreq > k)
        {
            o[s[l]]--;
            if (o[s[l]] == 0)
                o.erase(s[l]);
            l++;
        }
        maxL = max(maxL, r - l + 1);
        r++;
    }
    cout << maxL << endl;
}