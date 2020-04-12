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
#include <algorithm>
#include <stdarg.h>
#include <cassert>
#include <set>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"


using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void CirGate::traversal(GateList* l = 0) 
{
  
 /*  for (GateList::const_iterator i = _faninList.begin(); i != _faninList.end(); i++)
      if (!(*i)->isGlobalRef())//isMarked
      {
         //setToGlobalRef();??
         (*i)->traversal(l);     
      }
   if (l) 
      l->push_back(this);*/  
   if(_faninList[0] != NULL && !_faninList[0]->isGlobalRef() )
   {
      if(_faninList[0]->getTypeStr()!= "UNDEF")
      {
         _faninList[0]->setToGlobalRef();
         _faninList[0]->traversal(l);
      }
   }
   if(_faninList[1] != NULL && !_faninList[1]->isGlobalRef())
   { 
       if(_faninList[1]->getTypeStr()!= "UNDEF")
      {
         _faninList[1]->setToGlobalRef();
         _faninList[1]->traversal(l); 
      }
   }
   if (l) 
   {
      l->push_back(this);
   //   cout << "push" << this->get_id() << endl;
   }
}






void CirGate::reportRecursive(set<unsigned>& visited, int index, unsigned limit, bool _inv) const
{
   static unsigned now = 0;
//   reportRecursive(visited,0, level 2);   

   
   
   // print current
  
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
      //if(_faninList[0]->getTypeStr()!= "UNDEF")
     // {
         _inv = this->get_inv(0);
      //   _faninList[0]->setToGlobalRef();
         _faninList[0]->reportRecursive(visited, 0, limit, _inv);
    //  }
   }
   
   
   if(_faninList[1] != NULL && !_faninList[1]->isGlobalRef()&& now != limit+1 )
   { 
    //  if(_faninList[1]->getTypeStr()!= "UNDEF")
    //  {
         _inv = this->get_inv(1);
      //  _faninList[1]->setToGlobalRef();
        _faninList[1]->reportRecursive(visited, 1, limit, _inv); 
    //  }
   }
   now --;

}


void CirGate::reportRecursive_out(set<unsigned>& visited, int index, unsigned limit, bool _inv, bool is_first)  const
{
   static unsigned now = 0;
   static unsigned pre = -1;
//   reportRecursive(visited,0, level 2, 0);   
// print current
   
   cout << setw(now * 2) << "";
   _inv = this->get_inv(index);
  
//   cout << "this "  << this->get_id() << endl;
   if(this->getTypeStr()!="PI" && this->getTypeStr()!="CONST")
   {
    //  cout << "compare" << endl;
      if(this->_faninList[0]->get_id() == pre)
      {
         _inv = this->get_inv(0);
      //   cout << "here" << endl;
      }
      else if(this->_faninList[1]->get_id() == pre)
      {   
         _inv = this->get_inv(1);
      //   cout << "HERE" <<endl;
         
      }  
   }

   if (_inv && is_first != 1) 
   	cout << '!';
   
   cout << this->getTypeStr() << " " << this->get_id();
   
   
 //  _inv = this->get_inv(index);
   
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
      visited.insert(this->get_id());  
   //   cout << "insert = " << this->get_id() << endl;
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





void
CirGate::reportGate() const
{
   static const string Line = "==================================================";
   stringstream ss;
   ss << getTypeStr() << "(" << _id << ")";
   if (_name.length())
      ss << "\"" << _name << "\"";
   ss << ", line " << getLineNo();

   cout << Line << endl;
   cout << "= " << setw(46) << left << ss.str() << " =" << endl;
   cout << Line << endl;
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

