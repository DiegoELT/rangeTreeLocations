#include <bits/stdc++.h>

using namespace std;

const long long maxCities = 1e5;

int n;

struct Node {
  pair<int, int> xyCoordinates;
  Node * m_pSon[2];
  Node * nextNode;
  Node * prevNode;
  Node * rootOfOwnRT;
  Node(int x, int y) {
    xyCoordinates = make_pair(x, y);
    m_pSon[0]=0;
    m_pSon[1]=0; 
    nextNode = nullptr;
    prevNode = nullptr;
    rootOfOwnRT = nullptr;
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

pair<Node *, Node *> create_range_tree(pair<int, int> v[],int l, int h, Node * lastLeafNode, bool firstDimension) {
  if(l==h) {
    // cout << "l==h\n";
    Node * parent = new Node(v[l].first, v[l].second);
    parent -> m_pSon[0] = new Node(v[l].first, v[l]. second);
    if(lastLeafNode){ 
      lastLeafNode -> nextNode = parent -> m_pSon[0]; 
    }
    parent -> m_pSon[0] -> prevNode = lastLeafNode;
    return {parent, parent -> m_pSon[0]};
  } 

  if(l+1==h) {
    // cout << "l+1==h\n";
    Node * parent = new Node((v[l].first + v[h].first)/2, (v[l].second + v[h].second)/2);
    parent->m_pSon[0] = new Node(v[l].first, v[l]. second);
    parent->m_pSon[1] = new Node(v[h].first, v[h]. second);
    if(lastLeafNode){ 
      lastLeafNode -> nextNode = parent -> m_pSon[0]; 
    }
    parent -> m_pSon[0] -> nextNode = parent -> m_pSon[1];
    parent -> m_pSon[1] -> prevNode = parent -> m_pSon[0];
    return {parent, parent -> m_pSon[1]};
  }
  // TODO: Create range tree for each internal node
  
  int m = (l + h)/2;
  // cout << "m=" << m << '\n';
  pair<Node *, Node *> pairLeft = create_range_tree(v,l, m, lastLeafNode, firstDimension);
  pair<Node *, Node *> pairRight = create_range_tree(v,m+1, h, pairLeft.second, firstDimension);
  
  Node * parent = new Node((pairLeft.first -> xyCoordinates.first + pairRight.first -> xyCoordinates.first)/2, (pairLeft.second -> xyCoordinates.second + pairRight.second -> xyCoordinates.second)/2);
  parent -> m_pSon[0] = pairLeft.first; 
  parent -> m_pSon[1] = pairRight.first;

  if (firstDimension) {
    vector< pair<int, int> > nodesInRange;
    pair<int, int> nodesInRangeArray[maxCities];
    for (int i = l; i <= h; i++)
    nodesInRange.push_back(v[i]);

    sort(nodesInRange.begin(), nodesInRange.end(), sortbysec);

    for (int i = l; i <= h; i++)
      nodesInRangeArray[i] = nodesInRange[i];

    parent -> rootOfOwnRT = create_range_tree(nodesInRangeArray, 0, h - l + 1, nullptr, false).first;
  } 

  return {parent, pairRight.second};
}

vector< pair<int, int> > oneDimensionalQuery(Node * root, int l, int h) {
  vector< pair<int, int> > elements;
  Node * tempNode = root;
  while(tempNode -> m_pSon[0]){
    if(l < tempNode -> xyCoordinates.first)
      tempNode = tempNode -> m_pSon[0];
    else 
      tempNode = tempNode -> m_pSon[1];
  }
  while(tempNode && tempNode -> xyCoordinates.first <= h) {
    elements.push_back(tempNode -> xyCoordinates);
    tempNode = tempNode -> nextNode;
  }
  return elements;
}
    
void print(Node * r) {
  if(!r) return;
  print(r->m_pSon[0]);
  if(!r->m_pSon[0] && !r->m_pSon[1])
    cout << "(" << r->xyCoordinates.first << "," << r -> xyCoordinates.second << ") ";
  print(r->m_pSon[1]);
}

int main()
{
    pair<int,int> v[1000];

    for (int i = 0; i < 2000; i += 2) {
      v[i/2] = make_pair(i, 2000 - i);
    }

    Node * root = create_range_tree(v,0,999, nullptr, false).first;

    cout << "Testing both ways of printing.\n";
    
    // print(root);

    delete root;

    cout << "num nodes: " << n << '\n';

    return 0;
}
