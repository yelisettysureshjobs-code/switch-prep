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

void mergeSort(vector<int> &v, int s, int e)
{

    if (s >= e)
        return;
    int mid = (s + e) / 2;
    mergeSort(v, s, mid);
    mergeSort(v, mid + 1, e);
    mergeArray(v, s, mid, e);
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
    mergeSort(v, 0, n - 1);
    for (int i = 0; i < n; i++)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}