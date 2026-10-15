// divide and conquor (merge sort)
#include <bits/stdc++.h>
using namespace std;

int mergeArray(vector<int> &v, int s, int mid, int e)
{

    int l = s, r = mid + 1, c = 0;
    vector<int> temp;
    while (l <= mid && r <= e)
    {
        if (v[l] < v[r])
        {
            temp.push_back(v[l]);
            l++;
        }
        else
        {

            // [2,3,5] [1,4] , if we see 2 is greater than 1 and since left array is sorted all the elements
            // after 2 will also be greater than 1 , so we can add the length to the ans if we dont do that
            // some combinations will be missed
            // trigger : if a element is greater than a element all the elements right of it will also be greater than it
            c += (mid - l + 1);
            temp.push_back(v[r]);
            r++;
        }
    }
    while (l <= mid)
    {
        temp.push_back(v[l]);
        l++;
    }
    while (r <= e)
    {
        temp.push_back(v[r]);
        r++;
    }
    for (int i = s; i <= e; i++)
    {
        v[i] = temp[i - s];
    }
    return c;
}

int countInversions(vector<int> &v, int s, int e)
{

    if (s >= e)
        return 0;
    int mid = (s + e) / 2;
    int l = countInversions(v, s, mid);
    int r = countInversions(v, mid + 1, e);
    return l + r + mergeArray(v, s, mid, e);
}

int main()
{

    int n;
    cin >> n;
    vector<int> v(n, -1);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }
    cout << countInversions(v, 0, n - 1) << endl;
    for (int i = 0; i < n; i++)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}