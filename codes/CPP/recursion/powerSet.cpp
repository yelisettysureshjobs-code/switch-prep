// recursion

#include <bits/stdc++.h>
using namespace std;
void printPowerSets(vector<int> v, int n, int ind, vector<int> ans)
{
    if (ind >= n)
    {
        if (ans.size() != 0)
        {
            for (int a : ans)
            {
                cout << a << ' ';
            }
            cout << endl;
        }
        return;
    }
    ans.push_back(v[ind]);
    printPowerSets(v, n, ind + 1, ans);
    ans.pop_back();

    printPowerSets(v, n, ind + 1, ans);
    return;
}
void printPowerSets2(vector<int> v, int n, int ind, vector<int> ans)
{
    if (ind >= n)
    {
        if (ans.size() != 0)
        {
            for (int a : ans)
            {
                cout << a << ' ';
            }
            cout << endl;
        }
        return;
    }
    ans.push_back(v[ind]);
    printPowerSets(v, n, ind + 1, ans);
    ans.pop_back();
    while (ind < n - 1 && v[ind] == v[ind + 1])
    {
        ind++;
    }
    printPowerSets(v, n, ind + 1, ans);
    return;
}
int main()
{
    int n;
    cin >> n;
    vector<int> v(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }
    vector<int> ans;
    // sort(v.begin(),v.end()); for powerset 2 to eliminate duplicate sub sets
    printPowerSets(v, n, 0, ans);
}