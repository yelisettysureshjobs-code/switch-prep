#include <bits/stdc++.h>
using namespace std;

int mergeArray(vector<int> &v, int s, int mid, int e)
{
    vector<int> t;
    int l = s, r = mid + 1, c = 0;
    while (l <= mid && r <= e)
    {
        if (v[l] < v[r])
        {
            t.push_back(v[l]);
            l++;
        }
        else
        {
            c += (mid - l + 1);
            t.push_back(v[r]);
            r++;
        }
    }
    while (l <= mid)
    {
        t.push_back(v[l]);
        l++;
    }
    while (r <= e)
    {
        t.push_back(v[r]);
        r++;
    }
    for (int i = s; i <= e; i++)
    {
        v[i] = t[i - s];
    }
    return c;
}

int countInversions(vector<int> &v, int s, int e)
{
    int c = 0;
    if (s >= e)
    {
        return c;
    }
    int mid = (s + e) / 2;
    c += countInversions(v, s, mid);
    c += countInversions(v, mid + 1, e);
    c += mergeArray(v, s, mid, e);
    return c;
}

bool possibleKoko(vector<int> &v, int n, int k, int m)
{
    int c = 0;
    for (int i = 0; i < n; i++)
    {
        c += (v[i] + m - 1) / m;
    }
    return c <= k;
}

bool possibleBook(vector<int> &v, int n, int k, int mid)
{
    int s = 1, curPages = v[0];
    for (int i = 1; i < n; i++)
    {
        if (curPages + v[i] > mid)
        {
            s++;
            curPages = v[i];
        }
        else
        {
            curPages += v[i];
        }
    }
    return s <= k;
}

bool possibleBook2(vector<int> &v, int n, int k, int mid)
{
    int curPages = v[0], s = 1;
    for (int i = 1; i < n; i++)
    {
        if (curPages + v[i] > mid)
        {
            curPages = v[i];
            s++;
        }
        else
        {
            curPages += v[i];
        }
    }
    return s <= k;
}

void helper(int ind, int t, vector<int> v, int n, vector<int> &ds, vector<vector<int>> &ans)
{

    if (ind == n || t <= 0)
    {
        if (t == 0)
        {
            ans.push_back(ds);
        }
        return;
    }

    if (v[ind] <= t)
    {
        ds.push_back(v[ind]);
        helper(ind + 1, t - v[ind], v, n, ds, ans);
        ds.pop_back();
    }
    while (ind < n - 1 && v[ind] == v[ind + 1])
    {
        ind++;
    }
    helper(ind + 1, t, v, n, ds, ans);
}
vector<vector<int>> combination(vector<int> v, int n, int t)
{
    vector<vector<int>> ans;
    vector<int> ds;
    helper(0, t, v, n, ds, ans);
    return ans;
}

int main()
{
    // int n, k;
    // cin >> n;
    // vector<int> v(n, 0);
    // for (int i = 0; i < n; i++)
    // {
    //     cin >> v[i];
    // }
    // cin >> k;
    // int l = 0, r = 0, sum = 0, maxL = 0;
    // while (r < n)
    // {
    //     sum += v[r];
    //     while (sum > k && l <= r)
    //     {
    //         sum -= v[l];
    //         l++;
    //     }
    //     if (sum == k)
    //     {
    //         maxL = max(maxL, r - l + 1);
    //     }
    //     r++;
    // }
    // cout << maxL << endl;
    // unordered_map<int, int> o;
    // o[0] = -1;
    // int sum = 0, maxL = 0;
    // for (int i = 0; i < n; i++)
    // {
    //     sum += v[i];
    //     if (o.find(sum - k) != o.end())
    //     {
    //         maxL = max(maxL, i - o[sum - k]);
    //     }
    //     if (o.find(sum) == o.end())
    //     {
    //         o[sum] = i;
    //     }
    // }
    // cout << maxL << endl;
    // unordered_map<int, int> o;
    // vector<vector<int>> ans;
    // for (int i = 0; i < n; i++)
    // {
    //     if (o.find(k - v[i]) != o.end())
    //     {
    //         ans.push_back({o[k - v[i]], i});
    //     }
    //     o[v[i]] = i;
    // }
    // for (vector<int> t : ans)
    // {
    //     cout << t[0] << ' ' << t[1] << endl;
    // }
    // int z = 0, o = 0, t = n - 1;
    // while (o <= t) // = is there because , after the t is moved it can be on a diff element than 2 ,
    // // so o needs to check that element also after that element all will be 2's so no need to continue
    // {
    //     if (v[o] == 0)
    //     {
    //         swap(v[o], v[z]);
    //         o++, z++;
    //     }
    //     else if (v[o] == 2)
    //     {
    //         swap(v[o], v[t]);
    //         t--;
    //     }
    //     else
    //     {
    //         o++;
    //     }
    // }
    // for (int i : v)
    // {
    //     cout << i << " ";
    // }
    // int cs = 0, maxs = INT_MIN;
    // for (int i = 0; i < n; i++)
    // {
    //     cs += v[i];
    //     maxs = max(maxs, cs);
    //     cs = max(cs, 0);
    // }
    // cout << maxs << endl;
    // int mE = v[0], mc = 1;
    // for (int i = 1; i < n; i++)
    // {
    //     if (mE == v[i])
    //     {
    //         mc++;
    //     }
    //     else if (mc == 0)
    //     {
    //         mE = v[i];
    //         mc = 1;
    //     }
    //     else
    //     {
    //         mc--;
    //     }
    // }
    // cout << mE << ' ' << mc << endl;
    // int f = 0, fc = 0, s = 0, sc = 0;
    // for (int i = 0; i < n; i++)
    // {
    //     if (fc == 0)
    //     {
    //         f = v[i];
    //         fc = 1;
    //     }
    //     else if (sc == 0 && f != v[i])
    //     {
    //         s = v[i];
    //         sc = 1;
    //     }
    //     else if (f == v[i])
    //     {
    //         fc++;
    //     }
    //     else if (s == v[i])
    //     {
    //         sc++;
    //     }
    //     else
    //     {
    //         sc--, fc--;
    //     }
    // }
    // int ofc = 0, osc = 0;
    // for (int i = 0; i < n; i++)
    // {
    //     if (f == v[i])
    //     {
    //         ofc++;
    //     }
    //     else if (s == v[i])
    //     {
    //         osc++;
    //     }
    // }
    // if (ofc > floor(n / 3))
    // {
    //     cout << f << endl;
    // }
    // if (osc > floor(n / 3))
    // {
    //     cout << s << endl;
    // }
    // int cs = 0, maxs = INT_MIN, bestL, bestR, start;
    // for (int i = 0; i < n; i++)
    // {
    //     if (cs == 0)
    //         start = i;
    //     cs += v[i];
    //     if (maxs < cs)
    //     {
    //         bestL = start, bestR = i;
    //         maxs = cs;
    //     }
    //     cs = max(cs, 0);
    // }
    // for (int i = bestL; i <= bestR; i++)
    // {
    //     cout << v[i] << ' ';
    // }
    // unordered_map<int, int> o;
    // o[0] = 1; // if we are checking for max length then this should be -1 , else 1 if we want the count of sub array's
    // int sum = 0, cnt = 0;
    // cin >> k;
    // for (int i = 0; i < n; i++)
    // {
    //     sum += v[i];
    //     if (o.find(sum ^ k) != o.end())
    //     {
    //         cnt += o[sum ^ k];
    //     }
    //     o[sum]++;
    // }
    // cout << cnt << endl;
    // int maxc = 0;
    // unordered_set<int> o(v.begin(), v.end());
    // for (int i = 0; i < n; i++)
    // {
    //     if (o.find(v[i] - 1) == o.end())
    //     {
    //         int c = 0;
    //         int t = v[i];
    //         while (o.find(t) != o.end())
    //         {
    //             c++;
    //             t++;
    //         }
    //         maxc = max(maxc, c);
    //     }
    // }
    // cout << maxc << endl;
    // string s;
    // cin >> s;
    // unordered_map<char, int> o;
    // int l = 0, maxL = 0;
    // for (int i = 0; i < s.size(); i++)
    // {
    //     if (o.find(s[i]) != o.end())
    //     {
    //         if (o[s[i]] >= l)
    //         {
    //             l = o[s[i]] + 1;
    //         }
    //     }
    //     maxL = max(maxL, i - l + 1);
    //     o[s[i]] = i;
    // }
    // cout << maxL << endl;
    // int n;
    // cin >> n;
    // vector<int> v(n, 0);
    // for (int i = 0; i < n; i++)
    // {
    //     cin >> v[i];
    // }
    // cout << countInversions(v, 0, n - 1) << endl;
    // for (int i = 0; i < n; i++)
    // {
    //     cout << v[i] << ' ';
    // }
    // int n, m;
    // cin >> n >> m;
    // int k;
    // cin >> k;
    // vector<vector<int>> v(n, vector<int>(m, 0));
    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < m; j++)
    //     {
    //         cin >> v[i][j];
    //     }
    // }
    // int l = 0, r = m - 1;
    // while (l < n && r >= 0)
    // {
    //     if (v[l][r] == k)
    //     {
    //         cout << l << ' ' << r << endl;
    //         break;
    //     }
    //     else if (k<v[l][r])
    //     {
    //         r--;
    //     }
    //     else
    //     {
    //         l++;
    //     }
    // }
    // cout << -1 << endl;
    // int n, m;
    // cin >> n >> m;
    // int k;
    // cin >> k;
    // vector<vector<int>> v(n, vector<int>(m, 0));
    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < m; j++)
    //     {
    //         cin >> v[i][j];
    //     }
    // }
    // int l = 0, r = m - 1, f = 0;
    // while (l < n && r >= 0)
    // {
    //     if (v[l][r] == k)
    //     {
    //         cout << 'found' << endl;
    //         cout << l << ' ' << r << endl;
    //         f = 1;
    //         break;
    //     }
    //     else if (k < v[l][r])
    //     {
    //         r--;
    //     }
    //     else
    //     {
    //         l++;
    //     }
    // }
    // if (!f)
    // {
    //     cout << 'not found' << endl;
    // }
    // int n;
    // cin >> n;
    // vector<int> v(n, 0);
    // for (int i = 0; i < n; i++)
    // {
    //     cin >> v[i];
    // }
    // int h, f = 0;
    // cin >> h;
    // int l = 0, r = n - 1;
    // while (l <= r)
    // {
    //     int mid = (l + r) / 2;
    //     if (v[mid] == h)
    //     {
    //         cout << "found" << endl;
    //         cout << mid << endl;
    //         f = 1;
    //         break;
    //     }
    //     if (v[l] == v[mid] && v[mid] == v[r])
    //     {
    //         l++,r--;
    //         continue;
    //     }
    //     if (v[l] < v[mid])
    //     {
    //         if (v[l] < h && h < v[mid])
    //         {
    //             r = mid - 1;
    //         }
    //         else
    //         {
    //             l = mid + 1;
    //         }
    //     }
    //     else
    //     {
    //         if (v[mid] < h && h < v[r])
    //         {
    //             l = mid + 1;
    //         }
    //         else
    //         {
    //             r = mid - 1;
    //         }
    //     }
    // }
    // if (!f)
    // {
    //     cout << "not found" << endl;
    // }
    // int n, k;
    // cin >> n >> k;
    // vector<int> v(n, 0);
    // for (int i = 0; i < n; i++)
    // {
    //     cin >> v[i];
    // }
    // int l = 1, r = INT_MIN;
    // for (int a : v)
    // {
    //     r = max(r, a);
    // }
    // int ans = -1;
    // while (l <= r)
    // {
    //     int mid = (l + r) / 2;
    //     if (possibleKoko(v, n, k, mid))
    //     {
    //         r = mid - 1;
    //         ans = mid;
    //     }
    //     else
    //     {
    //         l = mid + 1;
    //     }
    // }
    // cout << ans << endl;
    // string s;
    // getline(cin, s);
    // unordered_map<char, int> o;
    // int l = 0, r = 0, ans = 0;
    // while (r < s.size())
    // {
    //     if (o.find(s[r]) != o.end())
    //     {
    //         if (l <= o[s[r]])
    //         {
    //             l = o[s[l]] + 1;
    //         }
    //     }
    //     o[s[r]] = r;
    //     ans = max(ans, r - l + 1);
    //     r++;
    // }
    // cout << ans << endl;
    // int n, k;
    // cin >> n >> k;
    // vector<int> v(n, 0);
    // for (int i = 0; i < n; i++)
    // {
    //     cin >> v[i];
    // }
    // int l = *max_element(v.begin(), v.end());
    // int r = accumulate(v.begin(), v.end(), 0);
    // if (k > n)
    // {
    //     cout << -1 << endl;
    // }
    // else
    // {
    //     while (l <= r)
    //     {
    //         int mid = (l + r) / 2;
    //         if (possibleBook2(v, n, k, mid))
    //         {
    //             r = mid - 1;
    //         }
    //         else
    //         {
    //             l = mid + 1;
    //         }
    //     }
    //     cout << l << endl;
    // }

    int n, k;
    cin >> n >> k;
    vector<int> v(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }
    sort(v.begin(), v.end());
    vector<vector<int>> ans = combination(v, n, k);
    for (vector<int> row : ans)
    {
        for (int element : row)
        {
            cout << element << " ";
        }
        cout << endl;
    }
}