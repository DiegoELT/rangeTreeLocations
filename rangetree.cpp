

#include<iostream>
using namespace std;

struct Nodo
{
     int x;
     Nodo * m_pSon[2];
     Nodo(int n)
     {
        x = n;
        m_pSon[0]=0;
        m_pSon[1]=0; 
     }
};

Nodo * create_range_tree(int v[],int l, int h)
{
   if(l==h)
   {
     Nodo * padre = new Nodo(v[l]);
     padre->m_pSon[0] = new Nodo(v[l]);
     return padre;

   } 
   if( l+1==h)
   {
     Nodo * padre = new Nodo((v[l] + v[h])/2);
     padre->m_pSon[0] = new Nodo(v[l]);
     padre->m_pSon[1] = new Nodo(v[h]);
     return padre;
   }
   int m = (l + h)/2;
   Nodo * pl = create_range_tree(v,l, m);
   Nodo * pr = create_range_tree(v,m+1, h);
   
   Nodo * padre = new Nodo((pl->x + pr->x)/2);
   padre->m_pSon[0] = pl; 
   padre->m_pSon[1] = pr;
   return padre;
}

void print(Nodo * r)
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
    Nodo * root = create_range_tree(v,0,5);
    print(root);


    return 1;
}
