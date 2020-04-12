/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include <algorithm>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
void
CirMgr::sweep()
{
  GateList dfs_list;
  dfs(&dfs_list,1);

  for (int i = 0; i < maxnum + outputs +1; i++)
  {
    if(_total[i]!= NULL)
    {
      if(_total[i]->_isUsed == false && _total[i]->getTypeStr() != "CONST" &&_total[i]->getTypeStr() !="PI")
      {
        cout << "Sweeping: " <<_total[i]->getTypeStr() <<"(" << _total[i]->get_id() <<") removed..." << endl;      
        for(int j = 0; j < 2; j++)
        {
          
          if(_total[i]->_faninList[j] == NULL)
            break;

          CirGate* pre = _total[i]->_faninList[j];
          GateList::iterator iter = find(pre->_fanoutList.begin(), pre->_fanoutList.end(),_total[i]);
           
          if(iter != pre->_fanoutList.end())
          {  
            pre->_fanoutList.erase(iter);
            //break;
          }
        }
        
        if(_total[i]->_fanoutList.size() != 0)
        {
          CirGate* next; 
          for(int j = 0; j < _total[i]->_fanoutList.size(); j++)
          {
            next = _total[i]->_fanoutList.at(j);
            if(next->_faninList[0] == _total[i])  
              next->_faninList[0] = NULL;
          
            else if(next->_faninList[1] == _total[i])
              next->_faninList[1] = NULL;
        
          }
        }
        if(_total[i]->getTypeStr() == "AIG")
          ands--;
        delete _total[i];
        _total[i]= NULL;
      }
    }
  }
}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
  GateList dfs_list;
  dfs(&dfs_list,0);
 
  for (size_t i = 0, n = dfs_list.size(); i < n; i++) 
  {
    if(dfs_list[i]->getTypeStr() == "AIG")
    {    
      int merge = -1;
      CirGate* rep;
      bool inv = false;
      CirGate* fin0 = dfs_list[i]->_faninList[0];
      CirGate* fin1 = dfs_list[i]->_faninList[1];
      if(fin0->get_id() == 0)
      {
        if(dfs_list[i]->get_inv(0) == true) //1?
        {
        //   cout << "case a1" << endl;
          merge = fin1->get_id();
          rep = fin1;
          inv = dfs_list[i]->get_inv(1);
        }
        else //0?
        {
        //  cout << "case b1" << endl;
          merge = 0;
          rep = fin0;
        //  inv = dfs_list[i]->get_inv(0);
        }
      }
      else if(fin1->get_id() == 0) 
      {
        if(dfs_list[i]->get_inv(1) == true) //?1
        {
        //   cout << "case a2" << endl;
          merge = fin0->get_id();
          rep = fin0;
          inv = dfs_list[i]->get_inv(0);
        }
        else //?0
        {
        //  cout << "case b2" << endl;
          merge = 0;
          rep = fin1;
        //  inv = dfs_list[i]->get_inv(1);
        }
      }
      else if(fin0->get_id() == fin1->get_id()) 
      {
        if(dfs_list[i]->get_inv(0) == dfs_list[i]->get_inv(1)) // same
        {
        //  cout << "case c" << endl;
          merge = fin0->get_id();
          rep = fin0;
          inv = dfs_list[i]->get_inv(0);
        }
        else //inversion
        {
        //  cout << "case d" << endl;
          merge = 0;
          rep = _total[0];
          //inv = false;
        }
      }
      if(merge != -1) //any of case
      {
          //deal  with delection's fanin
          GateList::iterator iter = find(fin0->_fanoutList.begin(), fin0->_fanoutList.end(),dfs_list[i]);
          if(iter != fin0->_fanoutList.end())
            fin0->_fanoutList.erase(iter);
          iter = find(fin1->_fanoutList.begin(), fin1->_fanoutList.end(),dfs_list[i]);
          if(iter != fin1->_fanoutList.end())
            fin1->_fanoutList.erase(iter);

          
          

          //deal  with delection's fanout
          if(dfs_list[i]-> _fanoutList.size() != 0)
          {
            CirGate* next; 
            for(int j = 0; j < dfs_list[i]->_fanoutList.size(); j++)
            {
              next = dfs_list[i]->_fanoutList.at(j);
              iter = find(rep->_fanoutList.begin(), rep->_fanoutList.end(),next);
              if(iter != rep->_fanoutList.end())
                rep->_fanoutList.push_back(next);//??
              if(next->_faninList[0] == dfs_list[i])  
              {  
                next->_faninList[0] = rep;
                if(inv)
                {
                  if(next->get_inv(0) == true)
                    next ->change_inv(0,false);
                  else
                    next ->change_inv(0,true);
                }   
                //  next ->change_inv(0,inv);
              }
              else if(next->_faninList[1] == dfs_list[i])
              {  
                next->_faninList[1] = rep;
                if(inv) 
                {
                  if(next->get_inv(1) == true)
                    next ->change_inv(1,false);
                  else
                    next ->change_inv(1,true);

                } 
                //  next ->change_inv(1,inv);
              }
            }
          }
          cout << "Simplifying: " << merge <<  " merging ";

          if(inv == true)
            cout << "!";
          cout << dfs_list[i]->get_id() << "..." << endl;

          if(_total[dfs_list[i]->get_id()]->getTypeStr() == "AIG")
            ands--;
          delete _total[dfs_list[i]->get_id()];
          _total[dfs_list[i]->get_id()]= NULL;
      }
    }
  }
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/
