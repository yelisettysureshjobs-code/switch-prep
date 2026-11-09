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

int cycleStart(Node *head)
{
    Node *slow = head, *fast = head;
    while (fast->next != NULL && fast->next->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
        {
            slow = head;
            while (slow != fast)
            {
                slow = slow->next;
                fast = fast->next;
            }
            return slow->val;
        }
    }
    return -1;
}
bool detectLoop(Node *head)
{
    Node *slow = head, *fast = head;
    while (fast->next != NULL && fast->next->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
        {
            return true;
        }
    }
    return false;
}
int lengthOfLoop(Node *head)
{
    Node *slow = head, *fast = head;
    while (fast->next != NULL && fast->next->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
        {
            int c = 1;
            slow = slow->next;
            while (slow != fast)
            {
                slow = slow->next;
                c++;
            }
            return c;
        }
    }
    return 0;
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
    temp->next = head->next->next;

    if (detectLoop(head))
    {
        cout << cycleStart(head) << " " << lengthOfLoop(head) << endl;
    }
}