/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include <algorithm>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;




// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{  
   
   fstream fstr(fileName.c_str());
   if (!fstr.is_open()) 
   {
      fstr.close();
      cerr << "Cannot open design \"" << fileName << "\"!!" << endl;
      return false;
   }
    
   
   string get;
   
   
   string space;
  
   fstr >> get;
   if(get!= "aag")
      return false;
   
   fstr >> maxnum >> inputs >> latch >> outputs >> ands;
   getline(fstr,space);
   
   unsigned line = 2;
   unsigned id;
   unsigned id2;
   unsigned id3;
   _total = new CirGate*[maxnum+outputs+2];

   CirGate* id0 = new ConstGate();
   _totalList.push_back(id0);
   _total[0] = id0;
   //RUN first
   //PI
   for(unsigned i = 0; i < inputs; i++)
   {
      fstr >> id;
      CirGate* pi = new PiGate(id/2, line);
      _piList.push_back(pi);
      _totalList.push_back(pi);
      _total[id/2] = pi;
  
      line++;
   }
   //PO
   for(unsigned i = 0; i < outputs; i++)
   {
      fstr >> id;
      line++;
   }
   //AIG

   for(unsigned i = 0; i < ands; i++)
   {
      fstr >> id >> id2 >> id3;
      CirGate* aig = new AigGate(id/2, line);
      _totalList.push_back(aig);
      _total[id/2] = aig;
    //  _gates[id/2] = aig;
      //   cout << "i = " << line << "  " << id << " " << id2 << " " << id3<< endl;
      line++;
   }
   getline(fstr,space);
   //symbol && comment
   while(getline(fstr,get)) //store or not??
   {
   //   cout << "i = " << line << "    ;" << get << endl;
      line++;
   }
   if (!fstr.eof()) throw std::runtime_error("Invalid data from file");
   

  
 // QuickSort(&_totalList,0,n);

   //RUN second
   
  // cout << "run two" << endl;
   line = 2;
   bool const_exist = false;

   fstream ifs(fileName.c_str());
   getline(ifs,space);
   //PI
   for(unsigned i = 0; i < inputs; i++)
   {
      ifs >> id;
   //   cout << "i = " << line << "  " << id << endl;
      line++;
   }
   //PO
   for(unsigned i = 0; i < outputs; i++)
   {
      ifs >> id;
      bool inv = false;

      unsigned gid = maxnum + _poList.size() + 1;
       CirGate* po = new PoGate(gid, line);
      _poList.push_back(po);
      _totalList.push_back(po);
      _total[gid] = po;

      CirGate* get;
      get = getGate(id/2);
      if(id%2 == 1)
         inv = true;

      if(get!=  NULL)
      {
         _poList.at(i)->addFanin(get,inv);
         get->addFanout(_poList.at(i));
         //at retrun??
      }
      else
      {
         CirGate* un = new UndefGate(id/2);
         _undefList.push_back(un);
         _poList.at(i)->addFanin(un,inv);
      }
   
      line++;
   }
   //AIG

   for(unsigned i = 0; i < ands; i++)
   {
      ifs >> id >> id2 >> id3;
     
   //   cout << "i = " << line << "  " << id << " " << id2 << " " << id3<< endl;
      CirGate* aig = getGate(id/2);
      CirGate* fin1;
      CirGate* fin2 = getGate(id3/2); 
      bool inv1 = false;
      bool inv2 = false;
      if(id2%2 == 1)
         inv1 = true;
      if(id3%2 == 1)
         inv2 = true;

      if(id2/2 == 0 && const_exist == false)
      {   
        // fin1 = new ConstGate();
         fin1 = _totalList.at(0);
         const_exist = true;
      }
      else 
      {
        fin1 = getGate(id2/2);
        if(fin1 == NULL)
        { 
           fin1 = new UndefGate(id2/2);
         _undefList.push_back(fin1);
        }
      }
      if(id3/2 == 0 && const_exist == false)
      {  
         fin2 = _totalList.at(0);
         
         const_exist = true;
      }
      else
      {
         fin2 = getGate(id3/2);
         if(fin2 == NULL)
         {
            fin2 = new UndefGate(id3/2);
            _undefList.push_back(fin2);
         }
      }
         
        aig->addFanin(fin1,inv1);
        aig->addFanin(fin2,inv2);
        fin1->addFanout(aig);
        fin2->addFanout(aig);

      line++;
   }
   getline(ifs,space);
   string id_str;
   int id_int;

   while(getline(ifs,get)) //store or not??
   {
      if(get == "c")
         break;
      
      
      myStrGetTok(get,id_str);
      string type;
      type = id_str.substr(0,1);
     
      id_str.erase(0,1);
      myStr2Int(id_str, id_int);
      get.erase(0,id_str.size()+2);
      
     
      if(type == "i")
         _piList.at(id_int)->add_name(get);
      else if(type == "o")
         _poList.at(id_int)->add_name(get);

      

    //  cout << "id = " << id_int << endl;
    //  cout << "name = " << get << endl;
    //  cout << "i = " << line << "    ;" << get << endl;
      line++;
   }
   while(getline(ifs,get))
   {
      line++;
   }

   if (!ifs.eof()) throw std::runtime_error("Invalid data from file");


   return true;
}


void 
CirMgr::dfs(GateList* l = 0) const 
{
//   cout << "!!" << endl;
   CirGate::setGlobalRef();//clearmarked
 //  cout << "!!" << endl;
   for (GateList::const_iterator i = _poList.begin(); i != _poList.end(); i++)
      (*i)->traversal(l);
//   cout << "ok" << endl;
}
/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
   unsigned int sum = inputs + outputs + ands;
   cout << endl;
   cout << "Circuit Statistics" << endl;
   cout << "==================" << endl;
   cout << "  PI    " << setw(8) << right << inputs << endl;
   cout << "  PO    " << setw(8) << right << outputs << endl;
   cout << "  AIG   " << setw(8) << right << ands << endl;
   cout << "------------------" << endl;
   cout << "  Total " << setw(8) << right << sum << endl;
}

void
CirMgr::printNetlist() const
{

   cout << endl;
   GateList dfs_list;
//   cout << "??" << endl;
   dfs(&dfs_list);
  // cout << "??" << endl;
   CirGate* curr;
   for (size_t i = 0, n = dfs_list.size(); i < n; i++) 
   {
   //   cout << "??" << endl;
      curr = dfs_list[i];
      if (!curr) 
         continue;
      cout << "[" << i << "] "
           << setw(3) << left << curr->getTypeStr();
      if(curr->getTypeStr()!="CONST")     
         cout << " ";
      else
         cout << "0";

      if (curr->getTypeStr() == "UNDEF")
         cout << "*";
      if(curr->getTypeStr()!="CONST")  
         cout << curr->get_id();
      

      if (curr->getTypeStr() == "AIG") 
      {
         cout << " ";
         if (curr->_faninList[0]->getTypeStr() == "UNDEF")
            cout << "*";
         if (curr->get_inv(0)) 
            cout << "!";
         cout << curr->_faninList[0]->get_id() << " ";
         if (curr->_faninList[1]->getTypeStr() == "UNDEF")
            cout << "*";
         if (curr->get_inv(1)) 
            cout << "!";
         cout << curr->_faninList[1]->get_id();
      } 
      else if (curr->getTypeStr()== "PO") 
      {
         cout << " ";
         if (curr->get_inv(0)) 
            cout << "!";
         cout << curr->_faninList[0]->get_id();
      }

      if (!curr->_name.empty())
         cout << " (" << curr->_name << ")";

      cout << endl;
   }
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   for (size_t i = 0, n = _piList.size(); i < n; i++) {
      cout << " " << _piList[i]->get_id();
   }
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   for (size_t i = 0, n = _poList.size(); i < n; i++) {
      cout << " " << _poList[i]->get_id();
   }
   cout << endl;
}
bool comp(const unsigned &a,const unsigned &b)
{
    return a>b;
}
void
CirMgr::printFloatGates() const
{
   vector<unsigned> floating;
   vector<unsigned> notUsed;

   for(GateList::const_iterator i = _undefList.begin(); i != _undefList.end(); i++)
   {
      if((*i)-> getTypeStr()=="UNDEF")
      {
         for(GateList::const_iterator j = (*i)->_fanoutList.begin(); j != (*i)->_fanoutList.end(); j++)
         {
            floating.push_back((*j)->get_id());
         }
      }
   }
   

   if(floating.size()!= 0)
   {
      cout << "Gate with floating fanin(s): " ;
      sort(floating.begin(),floating.end(),comp);
      while(floating.size()!= 0)
      {
         cout << floating.at(floating.size()-1);
         floating.pop_back();  
         if(floating.size()!= 0)
            cout << " ";
      }
      cout << endl;
   }
   

   for(GateList::const_iterator i = _totalList.begin(); i != _totalList.end(); i++)
   {
      if((*i)-> getTypeStr() =="PI" || (*i)-> getTypeStr() =="AIG")
      {
         if((*i)->_fanoutList.size()== 0)
         {
            notUsed.push_back((*i)->get_id());
         }
      }
   }
    
   if(notUsed.size()!= 0)
   {
      cout << "Gates defined but not used  : ";
      sort(notUsed.begin(),notUsed.end(),comp);
      while(notUsed.size()!= 0)
      {
         cout << notUsed.at(notUsed.size()-1);
         notUsed.pop_back();  
         if(notUsed.size()!= 0)
            cout << " ";
      }
       cout << endl;
   }

}

void
CirMgr::writeAag(ostream& outfile) const
{
/*
   outfile << "cirr sim01.aag" << endl;

	for(int i = 1; i < 17; i++)
	{
		outfile << "cirp" << endl
			<< "cirp -ne" << endl
			<< "cirp -pi" << endl
			<< "cirp -po" << endl
			<< "cirp -fl" << endl;
		for(int j = 0; j < 100 ; j++)
		{
			outfile << "cirg " << j << endl; 
			
         for(int k = 1; k < 50; k++) 
			{
            
				outfile << "cirg " << j << " -fanin " << k << endl;
			}
				
		}
		outfile << "cirr -rep sim0" << i <<".aag" << endl;
	}	

   
*/
  
   //maxnum >> inputs >> latch >> outputs >> ands;
   outfile << "aag "
           << maxnum << " "
           << inputs << " "
           << latch << " "
           << outputs << " "
           << ands << endl;

   for(GateList::const_iterator i = _piList.begin(); i != _piList.end(); i++)
   {
      outfile << (*i)->get_id()*2 << endl;
   }
   for(GateList::const_iterator i = _poList.begin(); i != _poList.end(); i++)
   {
      if((*i)->get_inv(0) == true)
         outfile << (*i)->_faninList[0]->get_id()*2 +1 << endl;
      else  
         outfile << (*i)->_faninList[0]->get_id()*2  << endl;
   }
   GateList dfs_list;
   dfs(&dfs_list);
   unsigned fan1;
   unsigned fan2;
  
   CirGate* curr;
   for (size_t i = 0, n = dfs_list.size(); i < n; i++) 
   {
      curr = dfs_list[i];
      if (!curr) 
         continue;
    
      if (curr->getTypeStr() == "AIG") 
      {
         if(curr->get_inv(0) == true)
            fan1 = curr-> _faninList[0]->get_id()*2 + 1;
         else  
            fan1 = curr-> _faninList[0]->get_id()*2;

         if(curr->get_inv(1) == true)
            fan2 = curr-> _faninList[1]->get_id()*2 + 1;
         else  
            fan2 = curr-> _faninList[1]->get_id()*2;

         outfile << curr->get_id()*2 << " " << fan1 << " " << fan2 << endl;
         
      } 
 
   }

   



}
