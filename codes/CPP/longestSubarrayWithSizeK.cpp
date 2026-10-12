#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void withoutNegativeOrZeros()
{
    int n, k;
    cin >> n >> k;
    vector<int> arr(n);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    int s = 0, ans = 0, maxl = -1;

    for (int e = 0; e < n; e++)
    {
        ans += arr[e];

        while (ans > k && s <= e)
        {
            ans -= arr[s];
            s++;
        }

        if (ans == k)
        {
            maxl = max(maxl, e - s + 1);
        }
    }

    cout << maxl << endl;
}
void withNegativeOrZeros()
{

    int n, k;
    cin >> n >> k;
    vector<int> v(n);
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }

    unordered_map<int, int> o;
    o[0] = -1; // initilize with -1 to handle 0 sum case , n=4 ,k=5 ,v=[ 1 2 -1 3 ] ans = 4
    int sum = 0, maxL = 0;
    for (int i = 0; i < n; i++)
    {
        sum += v[i];
        if (o.find(sum - k) != o.end())
        {
            maxL = max(maxL, i - o[sum - k]);
        }
        if (o.find(sum) == o.end()) // if sum is not present in the map then only add it to the map to keep the first occurence
        {
            o[sum] = i;
        }
    }

    cout << maxL << endl;
}

int main()
{
    // Call the function to find the longest subarray with sum equal to k without negative numbers or zeros
    withoutNegativeOrZeros();
    // Call the function to find the longest subarray with sum equal to k with negative numbers or zeros
    withNegativeOrZeros();

    return 0;
}