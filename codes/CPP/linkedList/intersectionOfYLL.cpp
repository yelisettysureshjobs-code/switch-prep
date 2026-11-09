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
Node *findIntersection(Node *head1, Node *head2)
{
    Node *temp1 = head1, *temp2 = head2;
    while (temp1 != temp2 )
    {
        if (temp1 == temp2)
        {
            return temp1;
        }
        if (temp1 == nullptr)
        {
            temp1 = head2;
        }
        if (temp2 == nullptr)
        {
            temp2 = head1;
        }
        temp1 = temp1->next;
        temp2 = temp2->next;
    }
    return temp1;
}
int main()
{
}