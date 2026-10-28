#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

// 3 1 2 4 , k=3
// r=0,l=0 sum=3 , maxL=1
// r=1,l=0 sum=4 -> sum = 1 , l=1
// r=2,l=1  sum = 3 , maxl=2
// r=3 , l=1 sum= 7 , sum = 6 , l = 2 , sum = 4 , l = 3
// ans = 2

void slidingWindow()
{
    int n;
    cin >> n;
    vector<int> arr;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin >> t;
        arr.push_back(t);
    }

    int k;
    cin >> k;
    int maxL = -1, sum = 0;

    int l = 0, r = 0;
    while (r < n)
    {
        sum += arr[r];
        while (sum > k && l < r)
        {
            sum -= arr[l];
            l++;
        }

        if (sum == k)
        {
            maxL = max(maxL, r - l + 1);
        }
        r++;
    }
    cout << maxL << endl;
}
void prefixSumForNegative()
{
    int n;
    cin >> n;
    vector<int> arr;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin >> t;
        arr.push_back(t);
    }

    int k;
    cin >> k;
    int sum = 0, maxl = -1;
    unordered_map<int, int> o;

    // 2 0 0 0 3 , k = 3

    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
        if (o.find(sum - k) != o.end())
        {
            maxl = max(maxl, i - o[sum - k]);
        }
        if (o.find(sum - k) == o.end())
        {
            o[sum - k] = i;
        }
    }
    cout << maxl << endl;
}

void next_permutation()
{

    int n;
    cin >> n;
    vector<int> v;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin >> t;
        v.push_back(t);
    }
    int pivot = -1;

    // Find pivot
    for (int i = n - 1; i > 0; i--)
    {
        if (v[i - 1] < v[i])
        {
            pivot = i - 1;
            break;
        }
    }

    // No pivot → already the largest permutation
    if (pivot == -1)
    {
        reverse(v.begin(), v.end());
    }
    else
    {
        // Find the rightmost element greater than pivot
        int maxR = n - 1;

        for (int i = maxR; i >= 0; i--)
        {
            if (v[maxR] >= v[pivot])
            {
                break;
            }
        }

        swap(v[pivot], v[maxR]);

        // Reverse suffix
        int l = pivot + 1;
        int r = n - 1;

        while (l < r)
        {
            swap(v[l], v[r]);
            l++;
            r--;
        }
    }

    for (int i = 0; i < n; i++)
    {
        cout << v[i] << ' ';
    }
}

// 4 2
// 3 -1 2 1
int main()
{
    int n, k;
    cin >> n >> k;
    vector<int> v;
    // for (int i = 0; i < n; i++)
    // {
    //     int t;
    //     cin >> t;
    //     v.push_back(t);
    // }
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
    //         maxL = max(maxL, r - l + 1);
    //     r++;
    // }
    // cout << maxL << endl;
    unordered_map<int, int> om;
    om[0] = 1;
    int sum = 0, maxL = 0;
    for (int i = 0; i < n; i++)
    {
        sum += v[i];
        if (om.find(sum - k) != om.end())
        {
            maxL = max(maxL, i - om[sum - k]);
        }
        if (om.find(sum) == om.end())
            om[sum] = i;
    }
    cout << maxL << endl;
}