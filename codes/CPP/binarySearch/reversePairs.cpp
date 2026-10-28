// divide and conquor (merge sort)

#include <bits/stdc++.h>
using namespace std;

void mergeArray(vector<int> &v, int s, int mid, int e)
{

    int l = s, r = mid + 1;
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
}
int countPairs(vector<int> &v, int s, int mid, int e)
{
    int r = mid + 1, c = 0;
    for (int i = s; i <= mid; i++)
    {
        while (r <= e && v[i] > 2 * v[r])
        {
            r++;
        }
        c += (r - (mid + 1));
    }
    return c;
}

int mergeSort(vector<int> &v, int s, int e)
{
    int c = 0;
    if (s >= e)
        return c;
    int mid = (s + e) / 2;
    c += mergeSort(v, s, mid);
    c += mergeSort(v, mid + 1, e);
    c += countPairs(v, s, mid, e);
    mergeArray(v, s, mid, e);
    return c;
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
    cout << mergeSort(v, 0, n - 1) << endl;
    for (int i = 0; i < n; i++)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}