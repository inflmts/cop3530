class Node
{
  public:
    int value;
    Node* next = nullptr;
};

float interQuartile(Node *head);

#ifndef SOLUTION_NO_IMPLEMENTATION

//  2:-----:     p12 p3
//  3: 23  :     p1  p2  p3
//  4:   mq:     p1  p2  p3  _
//  5: 23 q:     p1  _   p2  p3  _
//  6:1 3m :     _   p1  p2  _   p3  _
//  7: 23  :     _   p1  _   p2  _   p3  _
//  8:   mq:     _   p1  _   p2  _   p3  _   _
//  9: 23 q:     _   p1  _   _   p2  _   p3  _   _
// 10:1 3m :     _   _   p1  _   p2  _   _   p3  _   _
// 11: 23  :     _   _   p1  _   _   p2  _   _   p3  _   _
// 12:   mq:     _   _   p1  _   _   p2  _   _   p3  _   _   _
float interQuartile(Node* head)
{
  unsigned int i = 2;
  Node *p1 = head;
  //Node *p2 = head;
  Node *p3 = head = head->next;
  head = head->next;
  while (head) {
    ++i;
    if ((i & 3) == 2) p1 = p1->next;
    //if (i & 1) p2 = p2->next;
    if (i & 3) p3 = p3->next;
    head = head->next;
  }
  if (i & 2)
    return (float)p3->value - (float)p1->value;
  else
    return 0.5f * (((float)p3->value + (float)p3->next->value)
                 - ((float)p1->value + (float)p1->next->value));
}

#endif
