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
#include <string>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

#include "cirDef.h"
#include "cirGate.h"

extern CirMgr *cirMgr;


class CirMgr
{
public:
   CirMgr() {
    _pi = NULL; 
    _all = NULL;
    _patterns = 0;
    const_exist = false;
    
   }
   ~CirMgr() {} //??

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const 
   { 
      if(gid < (maxnum+outputs+1))
        return _total[gid];
      else 
        return 0; 
   }

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit optimization
   void sweep();
   void optimize();

   // Member functions about simulation
   void randomSim();
   void fileSim(ifstream&);
   void setSimLog(ofstream *logFile) { _simLog = logFile; }

   // Member functions about fraig
   void strash();
   void printFEC() const;
   void fraig();
  
   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void printFECPairs() const;
   void writeAag(ostream&) const;
   void writeGate(ostream&, CirGate*) const;
   
   void merge(int exist, int del);
   void run_simulation();
   void FEC(); 
   void separate();
   
  
   // DFS
   void dfs(GateList*, bool) const;

   unsigned maxnum;
   unsigned inputs;
   unsigned latch;
   unsigned outputs;
   unsigned ands;
   
   unsigned _patterns;
   vector<vector<int>> fecGrps;
   bool const_exist;
   

private:
   ofstream           *_simLog;
   GateList _piList;
   GateList _poList;
   GateList _undefList;
   vector<size_t>* _pi;
   vector<size_t>* _all;

   map<unsigned, CirGate*> _gates;
   CirGate** _total;
   
};

#endif // CIR_MGR_H
