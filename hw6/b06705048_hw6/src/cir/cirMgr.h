/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "cirDef.h"
#include "cirGate.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr(){}
   ~CirMgr() {}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   int binarySearch(const GateList* list, int l, int r, unsigned x) const
  { 
      if (r >= l) 
      { 
        unsigned mid = l + (r - l) / 2; 
  
        if (list->at(mid)->get_id() == x) 
            return mid; 
  
        if (list->at(mid)->get_id() > x) 
            return binarySearch(list, l, mid - 1, x); 
  
        return binarySearch(list, mid + 1, r, x); 
    } 
    return -1; 
  } 
   
   CirGate* getGate(unsigned gid) const 
   {
      if(gid < (maxnum+outputs+1))
        return _total[gid];
      else 
        return 0;
    }
     
/*
     int n = _totalList.size()-1;
     int l = 0;

     if(gid == 0 && _totalList.at(n)->get_id() == 0)
      return _totalList.at(n);

     int get = binarySearch(&(_totalList), l, n, gid);
     
     if(get == -1)
      return 0;
    else
      return _totalList.at(get);*/

    
  /*   for (GateList::const_iterator i = _totalList.begin(); i != _totalList.end(); i++) 
     {
       if((*i)-> get_id() == gid)
       {
         return *i;
       }
     }
     return 0; */
    

    

   // Member functions about circuit construction
   bool readCircuit(const string&);
  
   // DFS
   void dfs(GateList*) const;
   
   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;

  /*  
    bool addPi(unsigned id, int line) 
    { 
      CirGate* pi = new PiGate(id, line);
      _piList.push_back(pi);
      addTotal(id, pi);
      return true; // or return pi??
    }
    
    bool addPo(CirGate* src, int line, bool inv) 
    {
      unsigned id = maxnum + _poList.size() + 1;

      CirGate* po = new PoGate(id, line);

      connect(src, po, inv);
      
      _poList.push_back(po);
      addTotal(id, po);
      return true;
    }
    bool addAig(int line, unsigned id, CirGate* fin1, CirGate* fin2, bool inv1, bool inv2) 
    {
      AigGate* aig = new AigGate(id, line);
      connect(fin1, aig, inv1);
      connect(fin2, aig, inv2);
      addTotal(id, aig);
      return true;
    }

    bool addUndef(unsigned gid) 
    {
      UndefGate* undef = new UndefGate(gid);
      addTotal(gid, undef);
      return true;
    }

    bool addConst(unsigned gid) 
    {
      
      return true;
    }

    bool connect(CirGate* po, CirGate* pi, bool inv) 
    {
      if (!pi || !po) 
        return false;
      if (!pi-> addFanin(po, inv)) 
        return false;
      
      po-> addFanout(pi);
      return true;
    }
*/

   unsigned maxnum;
   unsigned inputs;
   unsigned latch;
   unsigned outputs;
   unsigned ands;

   void swap(CirGate* a, CirGate* b)
   {
      CirGate* temp = a;
      a = b;
      b = temp;
    }
  int Partition(GateList* arr, int front, int end, CirGate* temp)
  {
    int pivot = end;
   
    int i = front;
    i--;//
    

    for (int j = front; j < end; j++) 
    {
        if(arr->at(j)-> get_id() < arr->at(pivot)->get_id()) 
        {
            i++;
            temp = arr->at(i);
            arr->at(i) = arr->at(j);
            arr->at(j) = temp;     
        }
    }
    i++;

    temp = arr->at(i);
    arr->at(i) = arr->at(end);
    arr->at(end) = temp;
    
//    cout <<"change" <<  i << "end =  " << end <<endl;
 //   cout << "now = ";
 /*   for(GateList::const_iterator j = arr->begin(); j != arr->end(); j++)
    {
      cout << (*j)->get_id() << " ";
    }
      cout << endl;*/
    return i;
}
void QuickSort(GateList* arr, int front, int end)
{
    CirGate* temp;
    if (front < end) 
    {
        int pivot = Partition(arr, front, end,temp);
        QuickSort(arr, front, pivot - 1);
        QuickSort(arr, pivot + 1, end);
    }
}



private:
  GateList _piList;
  GateList _poList;
  GateList _undefList;
  GateList _totalList;

  map<unsigned, CirGate*> _gates;
  CirGate** _total;

  
   
};

#endif // CIR_MGR_H
