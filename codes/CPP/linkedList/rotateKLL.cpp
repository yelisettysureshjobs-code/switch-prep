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
int main()
{

    int n, k;
    cin >> n >> k;
    Node *head = new Node(-1);
    Node *temp = head;

    for (int i = 0; i < n; i++)
    {
        int t;
        cin >> t;
        temp->next = new Node(t);
        temp = temp->next;
    }
    head = head->next;
    Node *slow = head, *fast = head;
    for (int i = 0; i < k; i++)
    {
        fast = fast->next;
    }
    while (slow->next != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next;
    }
    Node *newHead = slow->next;
    fast->next = head;
    slow->next = nullptr;
    temp = newHead;
    while (temp != nullptr)
    {

        cout << temp->val << " ";
        temp = temp->next;
    }
    cout << endl;
}