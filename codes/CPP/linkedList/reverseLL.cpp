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

Node *reverseLL(Node *cur)
{
    if (cur == nullptr || cur->next == nullptr)
    {
        return cur;
    }

    Node *reverseRest = reverseLL(cur->next);

    Node *lastNode = cur->next;
    cur->next = nullptr;
    lastNode->next = cur;
    return reverseRest;
}

int main()
{
    int n;
    cin >> n;
    Node *head = new Node(-1);
    Node *temp = head;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin >> t;
        Node *next = new Node(t);
        temp->next = next;
        temp = temp->next;
    }
    head = head->next;

    // Node *cur = nullptr, *prev = nullptr, *next = nullptr;
    // cur = head;
    // while (cur != nullptr)
    // {
    //     next = cur->next;
    //     cur->next = prev;
    //     prev = cur;
    //     cur = next;
    // }
    // temp = prev;
    temp = reverseLL(head);
    while (temp != nullptr)
    {
        cout << temp->val << " ";
        temp = temp->next;
    }
    cout << endl;
}