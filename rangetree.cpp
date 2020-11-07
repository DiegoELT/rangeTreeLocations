#include <bits/stdc++.h>

using namespace std;

struct Node {
  pair<int, int> xyCoordinates;
  Node * m_pSon[2];
  Node * nextNode;
  Node * prevNode;
  Node(int x, int y) {
    xyCoordinates = make_pair(x, y);
    m_pSon[0]=0;
    m_pSon[1]=0; 
    nextNode = nullptr;
    prevNode = nullptr;
  }
};

pair<Node *, Node *> create_range_tree(pair<int, int> v[],int l, int h, Node * lastLeafNode) {
  if(l==h) {
    Node * parent = new Node(v[l].first, v[l].second);
    parent -> m_pSon[0] = new Node(v[l].first, v[l]. second);
    if(lastLeafNode){ 
      lastLeafNode -> nextNode = parent -> m_pSon[0]; 
    }
    parent -> m_pSon[0] -> prevNode = lastLeafNode;
    return make_pair(parent, parent -> m_pSon[0]);
  } 

  if(l+1==h) {
    Node * parent = new Node((v[l].first + v[h].first)/2, (v[l].second + v[h].second)/2);
    parent->m_pSon[0] = new Node(v[l].first, v[l]. second);
    parent->m_pSon[1] = new Node(v[h].first, v[h]. second);
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
  
  Node * parent = new Node((pairLeft.first -> xyCoordinates.first + pairRight.first -> xyCoordinates.first)/2, (pairLeft.second -> xyCoordinates.second + pairRight.second -> xyCoordinates.second)/2);
  parent -> m_pSon[0] = pairLeft.first; 
  parent -> m_pSon[1] = pairRight.first;

  return make_pair(parent, pairRight.second);
}

void print(Node * r)
{
  if(!r) return;
  print(r->m_pSon[0]);
  if(!r->m_pSon[0] && !r->m_pSon[1])
    cout << "(" << r->xyCoordinates.first << "," << r -> xyCoordinates.second << ") ";
  print(r->m_pSon[1]);
}

int main()
{
    pair<int,int> v[] = {make_pair(36,37), make_pair(42,51), make_pair(62,72), make_pair(80, 80), make_pair(90, 90), make_pair(100, 100)};
    Node * root = create_range_tree(v,0,5, nullptr).first;

    cout << "Testing both ways of printing.\n";
    
    print(root);

    cout << "\n";
    
    return 0;
}
