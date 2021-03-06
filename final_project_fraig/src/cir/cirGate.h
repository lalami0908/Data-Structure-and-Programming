/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "cirDef.h"

#include "sat.h"

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
public:
   CirGate(){}
   CirGate(unsigned id = 0, GateType type = UNDEF_GATE, int line = 0):
      _id(id), _type(type), _line(line),_ref(_globalRef()) 
      {
        _faninList[0] = NULL;
        _faninList[1] = NULL;
        _faninList[2] = NULL;
        _isUsed = false;
        visited_sim = false;
        value_sim = false;
        group_exist = false; 
        last = 0;
      }
   virtual ~CirGate() {}

   CirGate* _faninList[3];
   GateList _fanoutList;
   string _name;
   bool _isUsed;
   bool visited_sim;
   bool value_sim;
   bool group_exist;

   // Basic access methods
   string getTypeStr() const 
   { 
    switch (_type) {
         case PI_GATE:    return "PI";
         case PO_GATE:    return "PO";
         case AIG_GATE:   return "AIG";
         case CONST_GATE: return "CONST";
         case UNDEF_GATE: return "UNDEF";
         default:         return "";
      }
   }
   
    virtual bool addFanin(CirGate* gate, bool inv) 
   { 
     
  //  _faninList.push_back(gate); 
      return true; 
   }
   virtual bool addFanout(CirGate* gate) 
   { 
     _fanoutList.push_back(gate); 
      return true; 
   }

   unsigned getLineNo() const { return _line; }
   virtual bool isAig() const { return false; } //??
  // own
   unsigned get_id() const { return _id;}
   virtual bool get_inv(int i)const { return _inv[i];}
   void add_name(const string name) {_name = name;}


  //DFS
  static unsigned& _globalRef()
  {
    static unsigned _globalRef = 0;
    return _globalRef;
  }

   bool isGlobalRef(){ return (_ref == _globalRef()); }//isMarked
   void setToGlobalRef(){ _ref = _globalRef(); }//mark
   static void setGlobalRef() { _globalRef()++; }//clearMark


   // for making DFS list
   void traversal(GateList* l, bool isSweep);
   void reportRecursive(set<unsigned>& visited, int index, unsigned limit, bool _inv )const;
   void reportRecursive_out(set<unsigned>& visited, int index, unsigned limit, bool _inv,bool is_first) const;

   // Printing functions
   virtual void printGate() const {}
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;
   
   size_t last;
   
   
   void change_inv(int i, bool inv)
   {
     this->_inv[i] = inv;
   }
  
private:
  
protected:
  unsigned _id; 
  GateType _type;
  int _line;
  unsigned _ref;
  bool _inv[2];
};

class AigGate : public CirGate
{
  public:
    AigGate(unsigned id, int line): CirGate(id,AIG_GATE,line) {}
    ~AigGate(){}

  bool addFanin(CirGate* fin, bool inv)
  {
    if(_faninList[0] == NULL)
    {
      _faninList[0] = fin;
      _inv[0] = inv;
    }
    else
    {
      _faninList[1] = fin;
      _inv[1] = inv;

    }
    return true;
    
  }
  
  void printGate() const{}

  private:

};

class PiGate : public CirGate
{
  public:
    PiGate(unsigned id, int line): CirGate(id, PI_GATE, line) {}

    ~PiGate(){}
    bool addFanin(CirGate* fin, bool inv)
    {
      return false; 
    }
    void printGate() const {}
  private:

};
class PoGate : public CirGate
{
  public:
    
    PoGate(unsigned id, int line): CirGate(id, PO_GATE, line) {}
    ~PoGate(){}
    bool addFanin(CirGate* fin, bool inv)
    {
      
      _faninList[0] = fin;
      _inv[0] = inv;
     
      return false; 
    }
    bool addFanout(CirGate* fin) 
    { 
      return false; 
    }
    void printGate() const{}
  private:

};

class UndefGate: public CirGate 
{
  public:
  
    UndefGate(unsigned id): CirGate(id, UNDEF_GATE, 0) {};// line = 0;
    ~UndefGate(){}
    bool addFanin(CirGate* fin, bool inv)
    {
      return false;
    
    }
    void printGate() const{}
  private:

};

class ConstGate: public CirGate 
{
   public:
    ConstGate(): CirGate(0, CONST_GATE, 0) {};// id = 0 ; line = 0
    ~ConstGate(){}
    bool addFanin(CirGate* fin, bool inv)
    {
      return false;
    }
    void printGate() const{}
   
  private:

};

#endif // CIR_GATE_H
