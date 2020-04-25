/****************************************************************************
  FileName     [ cirDef.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic data or var for cir package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_DEF_H
#define CIR_DEF_H

#include <vector>
#include "myHashMap.h"


using namespace std;



// TODO: define your own typedef or enum

class CirGate;
class CirMgr;
class SatSolver;

//copy from hw6
typedef vector<CirGate*>           GateList;
typedef vector<unsigned>           IdList;
typedef vector<vector<unsigned>>   fecGrps;
typedef vector<unsigned>           fecGrp;


typedef size_t                   CirSimData;
const size_t sim_bits          = sizeof(CirSimData) << 3;
const CirSimData sim_hightest = (CirSimData)1 << (sim_bits - 1);

enum GateType
{
   UNDEF_GATE = 0,
   PI_GATE    = 1,
   PO_GATE    = 2,
   AIG_GATE   = 3,
   CONST_GATE = 4,

   TOT_GATE
};

#endif // CIR_DEF_H
