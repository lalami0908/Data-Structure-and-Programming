/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir simulation functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <math.h>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include <queue>


using namespace std;

// TODO: Keep "CirMgr::randimSim()" and "CirMgr::fileSim()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/************************************************/
/*   Public member functions about Simulation   */
/************************************************/

static CirSimData randomSimData() 
{
   CirSimData ret(0);

   int b = 32;
   int count = 0;
   while (b) 
   {
    size_t a = rnGen(63);
    ret |= 1UL << a;
    //  b >>= ;
      b--;
   }
   return ret;
}

void
CirMgr::randomSim()
{
  int round = inputs / 2;
  int patterns = round * 64;

  if(!_pi ||!_all)
  {
    _pi = new vector<size_t> [inputs]; 
    _all = new vector<size_t> [maxnum + outputs + 1];
  }
  else
  {
    for(int i = 0; i < maxnum + outputs + 1; i++)
      _all[i].clear();
    for(int i = 0; i < inputs; i++)
      _pi[i].clear();
  }
 
  size_t a = 0;
  for(int i = 0; i < round; i++)
    for(int j = 0; j < maxnum + outputs + 1; j++)
      if(_total[j]->getTypeStr() != "PI") 
        _all[j].push_back(a);
      
  for(int i = 0; i < round; i++)
  {
    for(int j = 0; j < inputs; j++)
    {
      a = randomSimData();
      _pi[j].push_back(a);
      _all[_piList.at(j)->get_id()].push_back(a); 
    }  
  }
  
  run_simulation();
     
  if(_simLog)
  {
    for(int i = 0; i <_pi[0].size(); i++)
    {
      for(int j = 0; j < 64 ; j++)
      {
        for(int k = 0 ; k < inputs; k++)
          *_simLog << ((_pi[k].at(i) >> j) & 1U);
      
        *_simLog << " ";
        
        for(int k = 0; k < outputs; k++)
          *_simLog << ((_all[_poList.at(k)->get_id()].at(i) >> j) & 1U);

        *_simLog << endl;
      }
    }
  }
  cout << patterns << " patterns simulated." << endl;
  FEC();

}


void
CirMgr::run_simulation() 
{
  GateList dfs_list;
  dfs(&dfs_list,0);
 
  for (size_t i = 0, n = dfs_list.size(); i < n; i++)
  {
    for(int j = 0; j <_pi[0].size(); j++)
    {
      unsigned id = dfs_list[i]-> get_id();
      if(dfs_list[i]->getTypeStr() == "AIG")
      {
        unsigned pre0 = dfs_list[i]->_faninList[0]->get_id();
        unsigned pre1 = dfs_list[i]->_faninList[1]->get_id();

        for(int k = 0; k < 64 ; k++)
	      {
          if(dfs_list[i]->get_inv(0) == true) 
          {
            if(dfs_list[i]->get_inv(1) == true)
            {
              if(!((_all[pre0].at(j) >> k) & 1U)&&!((_all[pre1].at(j) >> k) & 1U))
		          {    
                _all[id].at(j) |= 1UL << k;
                _total[id]->last |= 1UL << k;
              }
            }
            else
            {
              if(!((_all[pre0].at(j) >> k) & 1U)&&((_all[pre1].at(j) >> k) & 1U))
		          {    
                _all[id].at(j) |= 1UL << k;
                 _total[id]->last |= 1UL << k;
              }
            }
          }
          else
          {
            if(dfs_list[i]->get_inv(1) == true)
            {
              if(((_all[pre0].at(j) >> k) & 1U)&&!((_all[pre1].at(j) >> k) & 1U))
		          {
                _all[id].at(j) |= 1UL << k;
                _total[id]->last |= 1UL << k;
              }
            }
            else
            {
              if(((_all[pre0].at(j) >> k) & 1U)&&((_all[pre1].at(j) >> k) & 1U))
		          {    
                _all[id].at(j) |= 1UL << k;
                _total[id]->last |= 1UL << k;
              }
            }
          }	
	      }   
      }
      else if(dfs_list[i]->getTypeStr() == "PO")
      {
        
        unsigned pre0 = dfs_list[i]->_faninList[0]->get_id();
        for(int k = 0; k < 64 ; k++)
	      {
          if(dfs_list[i]->get_inv(0) == true) 
          {
            if(!((_all[pre0].at(j) >> k) & 1U))
		            _all[id].at(j) |= 1UL << k;
            
          }
          else
          {
            if(((_all[pre0].at(j) >> k) & 1U))
		            _all[id].at(j) |= 1UL << k;
          }
        } 
      }
    }
  } 
}
void
CirMgr::separate()
{

  int n = fecGrps.size();

  for(int i = 0; i < n; n--)
  {
    if(fecGrps.at(i).size() > 1)
    {
      int m  = fecGrps.at(i).size();
      vector<int> zero;
      vector<int> one;

      for(int j = 0; j < m; j++)
      {
        int id = fecGrps.at(i).at(j);
        if(id >= 0)
        {
          if(_total[id]->value_sim == 0)
            zero.push_back(id);
          else
            one.push_back(id);
        }
        else
        {
          if(_total[-id]->value_sim == 0)
            one.push_back(id);
          else
            zero.push_back(id);
        } 
      }
      
      fecGrps.erase(fecGrps.begin() + i);
        
      if(zero.size() > 1)
        fecGrps.push_back(zero);
      if(one.size() > 1)
        fecGrps.push_back(one);
        
    }
    else
    {
      vector<int> ori = fecGrps.at(i);
      fecGrps.push_back(ori);
      fecGrps.erase(fecGrps.begin() + i);

    }
  }
}

void
CirMgr::FEC() 
{
  int num = ands;
  if(const_exist)
    num+= 1;

  if(fecGrps.size() == 0)
  {
    vector<int> none;
    fecGrps.push_back(none);
    for(int i = 0;i < maxnum + outputs + 1; i++)
      if(_total[i] != NULL)
        if(_total[i]->getTypeStr() == "AIG" || _total[i]->getTypeStr() == "CONST")
        {
          int inver = _total[i]->get_id() * -1;
          (fecGrps.at(0)).push_back(_total[i]->get_id());

          if(_total[i]->getTypeStr() != "CONST")
            (fecGrps.at(0)).push_back(inver);
          
        }
  }
/*  for(int i = 0; i < _pi[0].size(); i++)
  {
    for(int j = 0; j < 64; j++)
    {
      for(int k = 0;k < maxnum + outputs + 1; k++)
      {
        if(_total[k]->getTypeStr() == "AIG" || _total[k]->getTypeStr() == "CONST")
          cout << ((_all[k].at(i) >> j) & 1U);

      }
      cout << endl;
    }
    cout << endl;
  } */       
  for(int i = 0; i < _pi[0].size(); i++)
  {
    for(int j = 0; j < 64; j++)
    {
      for(int k = 0;k < maxnum + outputs + 1; k++)
      {
        if(_total[k] != NULL)
        { 
          if(_total[k]->getTypeStr() == "AIG" || _total[k]->getTypeStr() == "CONST")
          {
            if(((_all[k].at(i) >> j) & 1U))
            {
              _total[k]->value_sim = true;
              _total[k]->last |= 1UL << j;
              
            }
            else
              _total[k]->value_sim = false;

          }
        }
      }
      
      separate();
      ////////////////
    
    //  if(fecGrps.size()== num)
    //    return;
 
    }
  }
  /*
  for(int l = 0;l < fecGrps.size(); l++)
  {
    cout << "[" << l << "] :";
    for(int m = 0;m < (fecGrps.at(l)).size(); m++)
    {
      cout << "(" << m << ") ";
      cout << fecGrps.at(l).at(m) << " ";
    }
    cout << endl;
  }*/
  
}

void
CirMgr::fileSim(ifstream& patternFile)
{ 
  string a;
  int patterns = 0;
  int round = -1; 
  size_t s = 0;

  if(!_pi ||!_all)
  {
    _pi = new vector<size_t> [inputs]; 
    _all = new vector<size_t> [maxnum + outputs + 1];
  }
  else
  {
    for(int i = 0; i < maxnum + outputs + 1; i++)
      _all[i].clear();
    for(int i = 0; i < inputs; i++)
      _pi[i].clear();
  }
  bool stop =false;

  queue<int> get;
  bool space_exist = false;
  
  int space_pos = 0;
  bool out = false;
  while(getline(patternFile, a))
  {
    int size = 0;
    int temp_i = 0;
    string temp_s;
    space_exist = false;
    space_pos = 0;

    for(int i = 0; i < a.size(); i++)
    {
      if(a.at(i)!=' ')
      {  
        temp_s = a.substr(i,1);
        stop = !myStr2Int(temp_s, temp_i);
        if(stop)
        {
          if(!out)
            cout << "Error: Pattern(" << a << ") contains a non-0/1 character(" << temp_s  <<  ")"  << endl
              << patterns / 64 << " patterns simulated." << endl;
          out = true;
          continue;
        }
        else
        {
          get.push(temp_i);
          size ++;
        }
      }
      if(a.at(i)==' ' || i == a.size() -1)
      {
        if(size != inputs && size != 0)
        {
          if(!out)
            cout << "Error: Pattern(" << a << ") length(" << a.size() <<") does not match the number of inputs("  << inputs << ") in a circuit!!" << endl
              << patterns / 64 << " patterns simulated." << endl;
          stop = true;
          out = true;
          continue;
        }
        size = 0;
      //  cout << "here" << endl;
      
            
        if(patterns  % 64 ==  0 )
        {
          for(int j = 0; j < inputs; j++)
          {
            _pi[j].push_back(s);
            _all[_piList.at(j)->get_id()].push_back(s);
          }
          round ++;
        } 
        if(!get.empty())
        {
            for(int j = 0; j < inputs; j++)
            {
            temp_i = get.front();
            if(temp_i == 1)
            {
              _pi[j].at(round) |= 1UL << (patterns % 64);
              _all[_piList.at(j)->get_id()].at(round) |= 1UL << (patterns % 64);
            }
            get.pop();
            }
          patterns ++;
        }
      }
    }
  }



  if(!stop)
    cout << patterns << " patterns simulated." << endl;
  s = 0;
  for(int i = 0; i < round + 1; i++)
    for(int j = 0; j < maxnum + outputs + 1; j++)
      if(_total[j]->getTypeStr() != "PI") 
        _all[j].push_back(s);
 

  run_simulation();
  
//  FEC();
  int stop_i = 0;
  if(patterns % 64 != 0)
    stop_i = patterns % 64;
  if(patterns / 64 == 0 && stop)
    return;
  if(_simLog)
  {
    for(int i = 0; i <_pi[0].size(); i++)
    {
      
      for(int j = 0; j < 64 ; j++)
      {
        if(i == _pi[0].size()-1 && stop_i != 0 && j == stop_i)
        {
          return;
        }

        for(int k = 0 ; k < inputs; k++)
          *_simLog << ((_pi[k].at(i) >> j) & 1U);
      
        *_simLog << " ";
        
        for(int k = 0; k < outputs; k++)
          *_simLog << ((_all[_poList.at(k)->get_id()].at(i) >> j) & 1U);

        *_simLog << endl;
      }
    }
  }
  
}

/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/
