#ifndef RANGETREE
#define RANGETREE

#include <bits/stdc++.h>

using namespace std;

const long long maxCities = 1e4+1;

int n;

struct Node {
  pair<int, int> xyCoordinates;
  Node * m_pSon[2];
  Node * nextNode;
  Node * prevNode;
  Node * rootOfOwnRT;
  Node * parent;
  Node(int x, int y) {
    xyCoordinates = make_pair(x, y);
    m_pSon[0] = 0;
    m_pSon[1] = 0; 
    nextNode = nullptr;
    prevNode = nullptr;
    rootOfOwnRT = nullptr;
    parent = nullptr;
  }
  ~Node(){
    ++n;
    // cout << "destruyendo\n";
    delete m_pSon[0];
    delete m_pSon[1];
  }
};

bool sortbysec(const pair<int,int> &a,  const pair<int,int> &b) { 
    return (a.second < b.second); 
}

pair<Node *, Node *> create_range_tree(pair<int, int> v[], int l, int h, Node* lastLeafNode, bool firstDimension) {
  cout << "A\n";
  if (!firstDimension) {
    cout << "create_range_tree\n";
  }
  if (l==h) {
    if (!firstDimension) {
      cout << "l==h\n";
    }
    // cout << "l==h\n";
    Node * parent = new Node(v[l].first, v[l].second);
    parent -> m_pSon[0] = new Node(v[l].first, v[l]. second);
    parent->m_pSon[0]->parent = parent;
    if (lastLeafNode) {
      lastLeafNode -> nextNode = parent -> m_pSon[0]; 
    }
    parent -> m_pSon[0] -> prevNode = lastLeafNode;
    if (!firstDimension) {
      cout << "returning from l==h";
    }
    return {parent, parent -> m_pSon[0]};
  }

  if (l+1==h) {
    if (!firstDimension) {
      cout << "l+1==h\n";
    }
    // cout << "l+1==h\n";
    Node * parent = new Node((v[l].first + v[h].first)/2, (v[l].second + v[h].second)/2);
    parent->m_pSon[0] = new Node(v[l].first, v[l]. second);
    parent->m_pSon[0]->parent = parent;
    parent->m_pSon[1] = new Node(v[h].first, v[h]. second);
    parent->m_pSon[1]->parent = parent;
    if (lastLeafNode) { 
      lastLeafNode -> nextNode = parent -> m_pSon[0]; 
    }
    parent -> m_pSon[0] -> nextNode = parent -> m_pSon[1];
    parent -> m_pSon[1] -> prevNode = parent -> m_pSon[0];
    if (!firstDimension) {
      cout << "returning from l+1==h";
    }
    return {parent, parent -> m_pSon[1]};
  }
  // TODO: Create range tree for each internal node
  
  int m = (l + h)/2;
  if (!firstDimension) {
    cout << "v[0]: " << v[0].first << ", " << v[0].second << "\n";
    cout << "m: " << m << "\n";
    cout << "h: " << h << "\n";
    cout << "l: " << l << "\n";
  }
  // cout << "m=" << m << '\n';
  pair<Node *, Node *> pairLeft = create_range_tree(v, l, m, lastLeafNode, firstDimension);
  if (!firstDimension) {
    cout << "created left range tree\n";
  }
  pair<Node *, Node *> pairRight = create_range_tree(v, m+1, h, pairLeft.second, firstDimension);
  if (!firstDimension) {
    cout << "created both range trees\n";
  }
  Node * parent = new Node((pairLeft.first -> xyCoordinates.first + pairRight.first -> xyCoordinates.first)/2, (pairLeft.second -> xyCoordinates.second + pairRight.second -> xyCoordinates.second)/2);
  parent -> m_pSon[0] = pairLeft.first; 
  parent->m_pSon[0]->parent = parent;
  parent -> m_pSon[1] = pairRight.first;
  parent->m_pSon[1]->parent = parent;

  if (!firstDimension) {
    cout << "parent created\n";
  }

  if (firstDimension) {
    cout << "if(firstDimension)\n";
    pair<int, int> nodesInRangeArray[maxCities];
    for (int i = l; i <= h; i++)
      nodesInRangeArray[i-l] = v[i];

    sort(nodesInRangeArray, nodesInRangeArray+(h-l), sortbysec);
    cout << "sorted\n";
    parent->rootOfOwnRT = create_range_tree(nodesInRangeArray, 0, h-l, nullptr, false).first;
    cout << "exited if(firstDimension)\n";
  }

  return {parent, pairRight.second};
}

vector< pair<int, int> > oneDimensionalQuery(Node * root, int l, int h) {
  cout << "onedimensionalquery\n";
  vector< pair<int, int> > elements;
  Node * tempNode = root;
  while (tempNode -> m_pSon[0]) {
    cout << tempNode->xyCoordinates.first << ", " << tempNode->xyCoordinates.second << "\n";
    cout << "ﬁ\n";
    if (l < tempNode->xyCoordinates.second)
      tempNode = tempNode->m_pSon[0];
    else { 
      tempNode = tempNode->m_pSon[1];
    }
    // no encontro elementos en el rango
    if (!tempNode)
      return elements;
  }
  cout << "found left node\n";
  while (tempNode->nextNode && tempNode -> xyCoordinates.second <= h) {
    elements.push_back(tempNode -> xyCoordinates);
    tempNode = tempNode -> nextNode;
  }
  cout << "found right node\n";
  cout << tempNode->xyCoordinates.first << ", " << tempNode->xyCoordinates.second << "\n";
  cout << "returning\n";
  return elements;
}

vector< pair<int, int> > twoDimensionalQuery(Node* root, int l1, int h1, int l2, int h2) {
  // l1, h1 es en X
  // l2, h2 es en Y
  Node* left;
  Node* right;

  cout << "twodimensionalquery\n";
  Node * tempNode = root;
  while(tempNode -> m_pSon[0]){
    if(l1 < tempNode -> xyCoordinates.first)
      tempNode = tempNode -> m_pSon[0];
    else 
      tempNode = tempNode -> m_pSon[1];
  }
  left = tempNode;
  cout << "found left node\n";
  
  while(tempNode->nextNode && tempNode -> xyCoordinates.first <= h1) {
    tempNode = tempNode -> nextNode;
  }
  right = tempNode;
  cout << "found right node\n";

  // LCA
  while (left != right) {
    left = left->parent;
    right = right->parent;
  }
  cout << "found LCA\n";
  cout << "calling onedimensional query on inner st\n";

  return oneDimensionalQuery(left->rootOfOwnRT, l2, h2);
}
    
void print(Node * r) {
  if(!r) return;
  print(r->m_pSon[0]);
  if(!r->m_pSon[0] && !r->m_pSon[1])
    cout << "(" << r->xyCoordinates.first << "," << r -> xyCoordinates.second << ") ";
  print(r->m_pSon[1]);
}

#endif
