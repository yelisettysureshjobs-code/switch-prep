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
Node *merge2SortedLists(Node *head1, Node *head2)
{
    Node *temp1 = head1, *temp2 = head2;
    Node *ans = new Node(-1);
    Node *tempAns = ans;
    while (temp1 != nullptr && temp2 != nullptr)
    {
        if (temp1->val < temp2->val)
        {
            tempAns->next = temp1;
            temp1 = temp1->next;
            tempAns = tempAns->next;
        }
        else
        {
            tempAns->next = temp2;
            temp2 = temp2->next;
            tempAns = tempAns->next;
        }
    }
    if (temp1 != nullptr)
    {
        tempAns->next = temp1;
        while (tempAns->next != nullptr)
        {
            tempAns = tempAns->next;
        }
    }
    if (temp2 != nullptr)
    {
        tempAns->next = temp2;
        while (tempAns->next != nullptr)
        {
            tempAns = tempAns->next;
        }
    }
    return ans->next;
}
int main()
{
}