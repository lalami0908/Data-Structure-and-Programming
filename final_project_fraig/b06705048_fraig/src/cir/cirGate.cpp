/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "cirDef.h"
#include "util.h"

using namespace std;

// TODO: Keep "CirGate::reportGate()", "CirGate::reportFanin()" and
//       "CirGate::reportFanout()" for cir cmds. Feel free to define
//       your own variables and functions.

extern CirMgr *cirMgr;

void CirGate::traversal(GateList* l = 0, bool isSweep = 0) 
{
  
   if(_faninList[0] != NULL && !_faninList[0]->isGlobalRef() )
   {
      if(_faninList[0]->getTypeStr()!= "UNDEF")
      {
         _faninList[0]->setToGlobalRef();
         _faninList[0]->traversal(l,isSweep);
      }
      else if(isSweep  == true)
         _faninList[0]->_isUsed = true;

   }
   if(_faninList[1] != NULL && !_faninList[1]->isGlobalRef())
   { 
      if(_faninList[1]->getTypeStr()!= "UNDEF")
      {
         _faninList[1]->setToGlobalRef();
         _faninList[1]->traversal(l,isSweep); 
      }
      else if(isSweep  == true)
         _faninList[1]->_isUsed = true;  
         
   }
   if (l) 
   {
      l->push_back(this);
      if(isSweep  == true)
         this->_isUsed = true;
   }
}

void CirGate::reportRecursive(set<unsigned>& visited, int index, unsigned limit, bool _inv) const
{
   static unsigned now = 0;

   cout << setw(now * 2) << "";
   
   if (_inv) 
   	cout << '!';
   
   cout << this->getTypeStr() << " " << this->get_id();
   
   
   _inv = this->get_inv(index);
   
   if (visited.find(this->get_id()) != visited.end() && limit != now)
   {
      cout << " (*)" << endl;
      return;
   }
   else 
   {
      cout << endl;
      if (now == limit) 
      {
         return;
      }
      if(this->getTypeStr()!="PI" && this->getTypeStr()!= "CONST")
         visited.insert(this->get_id());   
   }

   now++;
   
   if(_faninList[0] != NULL && !_faninList[0]->isGlobalRef() && now != limit+1 )
   {
   
         _inv = this->get_inv(0);  
         _faninList[0]->reportRecursive(visited, 0, limit, _inv);
  
   }
   
   
   if(_faninList[1] != NULL && !_faninList[1]->isGlobalRef()&& now != limit+1 )
   { 
 
         _inv = this->get_inv(1);
         _faninList[1]->reportRecursive(visited, 1, limit, _inv); 

   }
   now --;

}


void CirGate::reportRecursive_out(set<unsigned>& visited, int index, unsigned limit, bool _inv, bool is_first)  const
{
   static unsigned now = 0;
   static unsigned pre = -1;

   
   cout << setw(now * 2) << "";
   _inv = this->get_inv(index);
  

   if(this->getTypeStr()!="PI" && this->getTypeStr()!="CONST")
   {

      if(this->_faninList[0]->get_id() == pre)
         _inv = this->get_inv(0);
      else if(this->_faninList[1]->get_id() == pre)   
         _inv = this->get_inv(1);
   }

   if (_inv && is_first != 1) 
   	cout << '!';
   
   cout << this->getTypeStr() << " " << this->get_id();
   
   
   if (visited.find(this->get_id()) != visited.end() && limit != now)
   {   
      cout << " (*)" << endl;
      return;
   }
   else 
   {
      cout << endl;
      if (now == limit) 
         return;
           
      visited.insert(this->get_id());  
      pre = this->get_id();              
   }

   now++;

   index = 0;
   if(this->getTypeStr()!="PO")
   {
      for (GateList :: const_iterator i = _fanoutList.begin(); i != _fanoutList.end(); i++)
      {
      
            (*i)->reportRecursive_out(visited,index, limit, _inv,0);
            pre = this->get_id(); 
            index++;
      }
   }

   index = 0;
   now --;

}


/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
   static const string Line = "================================================================================";
   stringstream ss;
   ss << getTypeStr() << "(" << _id << ")";
   if (_name.length())
      ss << "\"" << _name << "\"";
   ss << ", line " << getLineNo();

   cout << Line << endl;
   cout << "= " << setw(46) << left << ss.str()  << endl;
   cout << "= FECs:" << endl;
   cout << "= Value: ";
   for(int i = 0; i < sizeof(last)*8;i++)
   {
      cout << ((last >> i) & 1U);
      if(i != 0 && i != 63 && i % 8 == 7)
         cout << "_";
   }
   cout << endl << Line << endl;
}

void
CirGate::reportFanin(int level) const
{
    assert (level >= 0);
   setGlobalRef();
   set<unsigned> visited;
 //  cout << "level " <<  level << endl;
   if(this->getTypeStr()== "PI" || this->getTypeStr()== "CONST")
   {
      cout << this->getTypeStr() << " " << this->get_id() << endl;
      return;
   }
   reportRecursive(visited,0, level,0);
   
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
 
   setGlobalRef();
   set<unsigned> visited;
   if(this->getTypeStr()== "PO" && this->get_id()!= 0)
   {
      cout << this->getTypeStr() << " " << this->get_id() << endl;
      return;
   }
   
   reportRecursive_out(visited,0, level,0,1);
}

