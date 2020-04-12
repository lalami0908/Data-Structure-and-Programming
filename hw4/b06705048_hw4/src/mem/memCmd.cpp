/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
//TODO
try{
        int temp = 0;
        int times = 0;
        vector<string>options;
        if (!CmdExec::lexOptions(option, options, 0))
                return CMD_EXEC_ERROR;
        if(options.size() == 0)
                return CmdExec::errorOption(CMD_OPT_MISSING,"");

   if(myStr2Int(options[0],temp) && temp > 0)
   {
        
        if(options.size() == 1)
        {
                mtest.newObjs(temp);
                return CMD_EXEC_DONE;
        }
        times = temp;

        if(!myStrNCmp("-array", options[1],2))
        {
                if(options.size() == 2)
                {
                        cerr << "Error: Missing option after (" << options[1] << ")!!"<<  endl;
                        return CMD_EXEC_ERROR;
                }
                if(myStr2Int(options[2],temp) && temp > 0)
                {
                   if(options.size()== 3)
                   {    
                        mtest.newArrs(times,temp);
                        return CMD_EXEC_DONE;
                   }
                   else
                   {
                        return CmdExec::errorOption(CMD_OPT_EXTRA,options[3]); 
                   }
                }
                else
                {
                   return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[2]);
                }
        }
        else
        {
                return CmdExec::errorOption(CMD_OPT_EXTRA ,options[1]);
        }

   }
   else if(!myStrNCmp("-array", options[0],2))
   {
        if(options.size() == 1)
        {
                cerr << "Error: Missing option after (" << options[0] << ")!!"<<  endl;
                return CMD_EXEC_ERROR;
        }
        if(myStr2Int(options[1],temp) && temp > 0)
        {
                if(options.size()==2)
                        return CmdExec::errorOption(CMD_OPT_MISSING,"");
                if(myStr2Int(options[2],times) && times >= 0)
                {
                        if(options.size() == 3)
                        {
                                mtest.newArrs(times,temp);
                                return CMD_EXEC_DONE;
                        }
                        else   
                                return CmdExec::errorOption(CMD_OPT_EXTRA,options[3]);
                }
                else
                {
                        return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[2]);       
                }
        }
        else
        {
                return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[1]);
        }
   }
   else
        return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[0]);
}
catch(const bad_alloc& e)
{
  return CMD_EXEC_ERROR;
}
return CMD_EXEC_DONE;
//////////////////////
  

/*
  int numObj= 0;
  int temp = 0;
  int arrSize = 0;
  bool IsArray = false;
  

  else 
  {
        for (int i = 0;i < options.size();i++)
        {
                if (!myStrNCmp("-Array", options[i],2) &&!IsArray)
                        IsArray = true;
                else if (myStr2Int(options[i],temp)&& temp > 0)
                {
                
                        if (IsArray && arrSize == 0)
                        arrSize = temp;
                        else
                        numObj = temp;
                }
                else if (temp == 0) //???
                 return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
                else
                 return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
        }
        try
        {
                if(IsArray)
                {
                  mtest.newArrs(numObj, arrSize);
                }
                else
                  mtest.newObjs(numObj);
                
        }
        catch(bad_alloc)
        {
                return CMD_EXEC_ERROR;
        }
   }*/
//return CMD_EXEC_DONE;

   // Use try-catch to catch the bad_alloc exception
   
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{

// TODO
  int temp;
  vector<string>options;
   if (!CmdExec::lexOptions(option, options, 0))
      return CMD_EXEC_ERROR;
  //cout << endl << options.size() << endl;
   if(options.size() == 0)
        return CmdExec::errorOption(CMD_OPT_MISSING,"");
   
   if(!myStrNCmp("-Random", options[0],2))
   {
        if(options.size()== 1)
        {
             cerr << "Error: Missing option after (" << options[0] << ")!!"<<  endl;
             return CMD_EXEC_ERROR;
        }
        if(myStr2Int(options[1],temp) && temp >= 0)
        {
           if(options.size() == 2) //-r 2
           {
                if(mtest.getObjListSize() == 0)
                {
                        cerr << "Size of object list is 0!!" << endl;
                        return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);

                }
                else
                {
                        for(int i = 0; i < temp; i++)
                        {
                           int r = rnGen(((int)mtest.getObjListSize()));
                           mtest.deleteObj(r); 
                        }
                        return CMD_EXEC_DONE;
                }
           }
           else
           {
                if(!myStrNCmp("-array", options[2],2)) //-r 2 -a
                {
                        if(mtest.getArrListSize() == 0)
                        {
                           cerr << "Size of array list is 0!!" << endl;
                           return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);

                        }
                        else
                        {
                                for(int i = 0; i < temp; i++)
                                {
                                        int r = rnGen(((int)mtest.getArrListSize()));
                                        mtest.deleteArr(r); 
                                }
                        }
                           //randommmmmmm array ??
                }
                else // -r 23 -r
                   return CmdExec::errorOption(CMD_OPT_EXTRA,options[2]);
           }           
        }
        else
           return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[1]);
   }
   else if(!myStrNCmp("-Index", options[0],2))
   {
        if(options.size()== 1)
        {
             cerr << "Error: Missing option after (" << options[0] << ")!!"<<  endl;
             return CMD_EXEC_ERROR;
        }
        if(myStr2Int(options[1],temp) && temp >= 0)
        {
           if(options.size() == 3)
           {
                if(!myStrNCmp("-array", options[2],2))
                {
                        if(temp < (int)mtest.getArrListSize())
                                mtest.deleteArr(temp);
                        else//?? = 0
                        {
                           cerr << "Size of array list (" << mtest.getArrListSize() << ") is <= " << temp << "!!" << endl;
                           return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[1]);
                        }
                }
                else
                   return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[2]);
           }       
           else if(options.size() == 2) // -i 3
           {
                if(temp < (int)mtest.getObjListSize())
                        mtest.deleteObj(temp);
                else
                {
                        cerr << "Size of object list (" << mtest.getObjListSize() << ") is <= " << temp << "!!" << endl;
                        return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[1]);
                }
           
           }
           else
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[3]);
        }
        else
           return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[1]);
   }        
   else if(!myStrNCmp("-Array", options[0],2))
   {
        if(options.size()== 1)
           return CmdExec::errorOption(CMD_OPT_MISSING,"");  

        if(!myStrNCmp("-Index", options[1],2))
        {
           if(options.size()== 2)
           {        
             cerr << "Error: Missing option after (" << options[1] << ")!!"<<  endl;
             return CMD_EXEC_ERROR;
           }
           if(myStr2Int(options[2],temp) && temp >= 0)
           {
                if(options.size()== 3)
                {
                                
                        if(temp < (int)mtest.getArrListSize())// 0 ! < 0 
                                mtest.deleteArr(temp);
                        else
                        {
                           cerr << "Size of array list (" << mtest.getArrListSize() << ") is <= " << temp << "!!" << endl;
                           return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[2]);
                        }
                        
                }
                else
                   return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[3]);


           }
           else
              return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[2]);
        }
        else if(!myStrNCmp("-Random", options[1],2))
        {
                if(options.size() == 2)
                {
                        cerr << "Error: Missing option after (" << options[1] << ")!!"<<  endl;
                        return CMD_EXEC_ERROR;

                }

                if(myStr2Int(options[2],temp) && temp >= 0)
                {
                
                        
                        if(options.size() == 3)//-a -r 3
                        {
                           if(mtest.getArrListSize() == 0)
                           {
                               cerr << "Size of array list is 0!!" << endl;
                                return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
                           }
                                //-a -r temp
                           for(int i = 0; i < temp; i++)
                           {
                               int r = rnGen(((int)mtest.getArrListSize()));
                               mtest.deleteArr(r);
                                
                           }
                        }
                        else
                        {
                           return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[3]);       
                        }
                }
                else
                {
                        return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[2]);
                }
        }
        else
           return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[1]); 
   }
   else
        return CmdExec::errorOption(CMD_OPT_ILLEGAL ,options[0]);

  

   return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


