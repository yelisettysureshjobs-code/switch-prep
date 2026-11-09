#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    int val;
    Node *next;
    Node(int val)
    {
        this->val = val;
        this->next = nullptr;
    }
    Node()
    {
        this->val = -1;
        this->next = nullptr;
    }
};

Node *reverse(Node *cur)
{
    Node *prev = nullptr, *next = nullptr;
    while (cur != nullptr)
    {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

int main()
{
    Node *n1 = new Node(-1), *n2 = new Node(-1);
    Node *t1 = n1, *t2 = n2;
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin >> t;
        t1->next = new Node(t);
        t1 = t1->next;
    }
    for (int i = 0; i < m; i++)
    {
        int t;
        cin >> t;
        t2->next = new Node(t);
        t2 = t2->next;
    }
    n1 = n1->next;
    n2 = n2->next;

    Node *ans = new Node(-1);
    Node *tans = ans;
    n1 = reverse(n1);
    n2 = reverse(n2);
    int carry = 0;
    while (n1 != nullptr && n2 != nullptr)
    {
        int sum = n1->val + n2->val + carry;
        if (sum > 9)
        {
            carry = 1;
            tans->next = new Node(sum % 10);
        }
        else
        {
            tans->next = new Node(sum);
            carry = 0;
        }
        tans = tans->next;
        n1 = n1->next;
        n2 = n2->next;
    }
    while (n1 != nullptr)
    {
        if (carry != 0)
        {
            int sum = n1->val + carry;
            if (sum > 9)
            {
                carry = 1;
                n1->val = (sum) % 10;
            }
            else
            {

                n1->val = sum;
                carry = 0;
            }
        }
        tans->next = n1;
        tans = tans->next;
        n1 = n1->next;
    }
    while (n2 != nullptr)
    {
        if (carry != 0)
        {
            int sum = n2->val + carry;
            if (sum > 9)
            {
                carry = 1;
                n2->val = (sum) % 10;
            }
            else
            {

                n2->val = sum;
                carry = 0;
            }
        }

        tans->next = n2;
        tans = tans->next;
        n2 = n2->next;
    }
    if (carry != 0)
    {
        tans->next = new Node(carry);
        tans = tans->next;
    }
    tans = reverse(ans->next);
    while (tans != nullptr)
    {
        cout << tans->val << " ";
        tans = tans->next;
    }
    cout << endl;
}