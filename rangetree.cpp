#include <bits/stdc++.h>

using namespace std;

struct Node {
  int x;
  Node * m_pSon[2];
  Node * nextNode;
  Node * prevNode;
  Node(int n) {
    x = n;
    m_pSon[0]=0;
    m_pSon[1]=0; 
    nextNode = nullptr;
    prevNode = nullptr;
  }
};

pair<Node *, Node *> create_range_tree(int v[],int l, int h, Node * lastLeafNode) {
  if(l==h) {
    Node * parent = new Node(v[l]);
    parent -> m_pSon[0] = new Node(v[l]);
    if(lastLeafNode){ 
      lastLeafNode -> nextNode = parent -> m_pSon[0]; 
    }
    parent -> m_pSon[0] -> prevNode = lastLeafNode;
    return make_pair(parent, parent -> m_pSon[0]);
  } 

  if( l+1==h) {
    Node * parent = new Node((v[l] + v[h])/2);
    parent->m_pSon[0] = new Node(v[l]);
    parent->m_pSon[1] = new Node(v[h]);
    if(lastLeafNode){ 
      lastLeafNode -> nextNode = parent -> m_pSon[0]; 
    }
    parent -> m_pSon[0] -> nextNode = parent -> m_pSon[1];
    parent -> m_pSon[1] -> prevNode = parent -> m_pSon[0];
    return make_pair(parent, parent -> m_pSon[1]);
  }

  int m = (l + h)/2;
  pair<Node *, Node *> pairLeft = create_range_tree(v,l, m, lastLeafNode);
  pair<Node *, Node *> pairRight = create_range_tree(v,m+1, h, pairLeft.second);
  
  Node * parent = new Node((pairLeft.first -> x + pairRight.first -> x)/2);
  parent -> m_pSon[0] = pairLeft.first; 
  parent -> m_pSon[1] = pairRight.first;

  return make_pair(parent, pairRight.second);
}

void print(Node * r)
{
  if(!r) return;
  print(r->m_pSon[0]);
  if(!r->m_pSon[0] && !r->m_pSon[1])
    cout<<r->x<<" ";
  print(r->m_pSon[1]);
}

int main()
{
    int v[] = {36,37,42,51,62,72};
    Node * root = create_range_tree(v,0,5, nullptr).first;

    cout << "Testing both ways of printing.\n";
    
    print(root);

    cout << "\n";

    Node * testFirst = root -> m_pSon[0] -> m_pSon[0] -> m_pSon[0];

    while(testFirst){
      cout << testFirst -> x << " ";
      testFirst = testFirst -> nextNode;
    }
      
    return 0;
}
