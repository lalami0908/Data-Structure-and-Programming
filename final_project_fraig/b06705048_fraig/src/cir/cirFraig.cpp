/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/


#include <cassert>
#include <algorithm>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "util.h"
#include <utility>

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
// _floatList may be changed.
// _unusedList and _undefList won't be changed


class OnePart
{
  public:
    OnePart(int one, int two)
    {
      _one = std::min(one, two);
      _two = std::max(one, two);
    }
    size_t operator()()const
    {
      return hash<int>()(_one) ^ hash<int>()(_two); 
    }
    bool operator== (const OnePart& other)const 
    {
      if(_one == other._one && _two == other._two)
        return true;
      return false;
    }
  private:
    int _one;
    int _two;
};

static HashMap<OnePart, int> strashMap;

void
CirMgr::strash()
{
  GateList dfs_list;
  dfs(&dfs_list,0);
  strashMap.init((maxnum+2));
  for (size_t i = 0,n = dfs_list.size() ; i < n; i++) 
  {
    if(dfs_list[i]->getTypeStr() == "AIG")
    {
      
      CirGate* _now = dfs_list[i];
      int fin0 = _now->_faninList[0]->get_id()*2;
      int fin1 = _now->_faninList[1]->get_id()*2;
      if(_now->get_inv(0))
        fin0 +=1;
      if(_now->get_inv(1))
        fin1 +=1;
      
      OnePart p_now (fin0,fin1);

      int get = strashMap.find(p_now);
     
      if(get != -1)//exist
      {
        if(_total[_now->get_id()]->getTypeStr() == "AIG")
          ands--;
        merge(get, _now->get_id());
        cout << "Strashing: " << get << " merging " << _now->get_id() << "..." << endl;     
      }
      else
      {
        strashMap.insert(p_now, _now->get_id());
      }
    }
  }
}

void
CirMgr::fraig()
{
  
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
