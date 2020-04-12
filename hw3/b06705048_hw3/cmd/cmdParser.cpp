/****************************************************************************
  FileName     [ cmdParser.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command parsing member functions for class CmdParser ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "util.h"
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    External funcitons
//----------------------------------------------------------------------
void mybeep();


//----------------------------------------------------------------------
//    Member Function for class cmdParser
//----------------------------------------------------------------------
// return false if file cannot be opened
// Please refer to the comments in "DofileCmd::exec", cmdCommon.cpp
bool
CmdParser::openDofile(const string& dof)
{

   ifstream* tmp = new ifstream(dof.c_str());
   if(!tmp-> is_open())
   {
      if(_dofileStack.size()!= 0)  
        _dofile = _dofileStack.top();
      else 
        _dofile = 0;
      return false;         
   }
   _dofileStack.push(tmp);
   _dofile = tmp;
   return true;
  
}

// Must make sure _dofile != 0
void
CmdParser::closeDofile()
{

  assert(_dofile != 0);

   // TODO...
   _dofile->close();
   delete _dofile;   
   _dofileStack.pop();

   if(!_dofileStack.empty())
      _dofile = _dofileStack.top();
   else _dofile = NULL;
 
}

// Return false if registration fails
bool
CmdParser::regCmd(const string& cmd, unsigned nCmp, CmdExec* e)
{
   // Make sure cmd hasn't been registered and won't cause ambiguity
   string str = cmd;
   unsigned s = str.size();
   if (s < nCmp) return false;
   while (true) {
      if (getCmd(str)) return false;
      if (s == nCmp) break;
      str.resize(--s);
   }

   // Change the first nCmp characters to upper case to facilitate
   //    case-insensitive comparison later.
   // The strings stored in _cmdMap are all upper case
   //
   assert(str.size() == nCmp);  // str is now mandCmd
   string& mandCmd = str;
   for (unsigned i = 0; i < nCmp; ++i)
      mandCmd[i] = toupper(mandCmd[i]);
   string optCmd = cmd.substr(nCmp);
   assert(e != 0);
   e->setOptCmd(optCmd);

   // insert (mandCmd, e) to _cmdMap; return false if insertion fails.
   return (_cmdMap.insert(CmdRegPair(mandCmd, e))).second;
}

// Return false on "quit" or if excetion happens
CmdExecStatus
CmdParser::execOneCmd()
{
   bool newCmd = false;
   if (_dofile != 0)
      newCmd = readCmd(*_dofile);
   else
      newCmd = readCmd(cin);

   // execute the command
   if (newCmd) {
      string option;
      CmdExec* e = parseCmd(option);
      
      if (e != 0)
         return e->exec(option);
      
   }
   return CMD_EXEC_NOP;
}

// For each CmdExec* in _cmdMap, call its "help()" to print out the help msg.
// Print an endl at the end.
void
CmdParser::printHelps() const
{
   // TODO...

  for(auto i = _cmdMap.begin(); i != _cmdMap.end();i++)
     (*i).second->help(); 
  cout << endl;     
   
}

void
CmdParser::printHistory(int nPrint) const
{
   assert(_tempCmdStored == false);
   if (_history.empty()) {
      cout << "Empty command history!!" << endl;
      return;
   }
   int s = _history.size();
   if ((nPrint < 0) || (nPrint > s))
      nPrint = s;
   for (int i = s - nPrint; i < s; ++i)
      cout << "   " << i << ": " << _history[i] << endl;
}


//
// Parse the command from _history.back();
// Let string str = _history.back();
//
// 1. Read the command string (may contain multiple words) from the leading
//    part of str (i.e. the first word) and retrive the corresponding
//    CmdExec* from _cmdMap
//    ==> If command not found, print to cerr the following message:
//        Illegal command!! "(string cmdName)"
//    ==> return it at the end.
// 2. Call getCmd(cmd) to retrieve command from _cmdMap.
//    "cmd" is the first word of "str".
// 3. Get the command options from the trailing part of str (i.e. second
//    words and beyond) and store them in "option"
//
CmdExec*
CmdParser::parseCmd(string& option)// option contain no first space
{
  
   assert(_tempCmdStored == false);
   assert(!_history.empty());
   string str = _history.back();
   // TODO...
   assert(str[0] != 0 && str[0] != ' ');

   string token;
   myStrGetTok(str,token);
   CmdExec* back;
   back = getCmd(token);

   if(back == 0)
   {
      cerr << "Illegal command!! (" << token << ")" << endl;
      return NULL;
   }
   else
    option = str.erase(0,token.size()+1);

   return back;
}

void
CmdParser::listCmd(const string& str)
{
  // TODO...
  string cursor_str;
  size_t end = myStrGetTok(str,cursor_str);
  string s1;
  vector <string> multi_match;
  

  if(str.find_first_not_of(' ') == string::npos)//case 1  print-all
  {  
      cout << endl;
      cout << setw(12) << left << "DBAPpend";
      cout << setw(12) << left << "DBAVerage";
      cout << setw(12) << left << "DBCount";
      cout << setw(12) << left << "DBMAx";
      cout << setw(12) << left << "DBMIn" <<endl;
      cout << setw(12) << left << "DBPrint";
      cout << setw(12) << left << "DBRead";
      cout << setw(12) << left << "DBSOrt";
      cout << setw(12) << left << "DBSUm";
      cout << setw(12) << left << "DOfile" <<endl;     
      cout << setw(12) << left << "HELp";
      cout << setw(12) << left << "HIStory";
      cout << setw(12) << left << "Quit" ;
      _tabPressCount = 0;
      reprintCmd();    
  }
  else if (getCmd(cursor_str))//case 5 6
  {
    //already _tabPressCount++ on I donnot know where
    if(str.find(' ') != string::npos && _tabPressCount == 1)//case 5 usage
    {          
      cout << endl;                   
      getCmd(cursor_str)->usage(cout);
        reprintCmd(); 
    }
    else if(str.find(' ') != string::npos && _tabPressCount > 1) // case 6
    {



    }  
    

  }
  else //case 2 3 4 7 (4 & 7 beep)
  {
    bool case_sev = false;
    if(str.at(str.size()-1) == ' ')
      case_sev = true;

    for(CmdMap::iterator i = _cmdMap.begin(); i != _cmdMap.end(); i++)
		{    
      s1 = (*i).first + (*i).second -> getOptCmd(); 
      
      if(myStrNCmp(s1,cursor_str,cursor_str.size()) == 0)
        multi_match.push_back(s1);
    }

    if(multi_match.size() > 1) // case 2 multi match
    {
      for(size_t i = 0;i < multi_match.size();i++)
			{
        if(i % 5 == 0)
          cout << endl;
        cout << setw(12) << left << multi_match.at(i);
      }
      _tabPressCount = 0;
      reprintCmd();
    }
    else if(multi_match.size() == 1)// case 3 only one match // && case 7
    {
      if(!case_sev)
      {
        string sub_str =  multi_match.at(0).substr(cursor_str.size());
        for(size_t i = 0; i < sub_str.size(); i++)
          insertChar(sub_str[i]);
        insertChar(' ');
      }
      else // case 7
          mybeep();

    }
    else if((multi_match.size() == 0)) // case 4
      mybeep();

    multi_match.clear(); // need or not?
    s1.clear();
    _tabPressCount = 0;
  }
  
}


// cmd is a copy of the original input
//
// return the corresponding CmdExec* if "cmd" matches any command in _cmdMap
// return 0 if not found.
//
// Please note:
// ------------
// 1. The mandatory part of the command string (stored in _cmdMap) must match
// 2. The optional part can be partially omitted.
// 3. All string comparison are "case-insensitive".
//
CmdExec*
CmdParser::getCmd(string cmd)
{
   CmdExec* e = 0;
   // TODO...
   string s1;
   string cmd_second;
   int count = 0;

  for(CmdMap::iterator i = _cmdMap.begin(); i != _cmdMap.end();i++)
  {
    s1 = (*i).first + (*i).second -> getOptCmd();
    count = (*i).first.size();
    if(!myStrNCmp(s1, cmd, count))
      e = (*i).second; 
    s1.clear();
  }
  return e;
}


//----------------------------------------------------------------------
//    Member Function for class CmdExec
//----------------------------------------------------------------------
// return false if option contains an token
bool
CmdExec::lexNoOption(const string& option) const
{
   string err;
   myStrGetTok(option, err);
   if (err.size()) {
      errorOption(CMD_OPT_EXTRA, err);
      return false;
   }
   return true;
}

// Return false if error options found
// "optional" = true if the option is optional XD
// "optional": default = true
//
bool
CmdExec::lexSingleOption
(const string& option, string& token, bool optional) const
{
   size_t n = myStrGetTok(option, token);
   if (!optional) {
      if (token.size() == 0) {
         errorOption(CMD_OPT_MISSING, "");
         return false;
      }
   }
   if (n != string::npos) {
      errorOption(CMD_OPT_EXTRA, option.substr(n));
      return false;
   }
   return true;
}

// if nOpts is specified (!= 0), the number of tokens must be exactly = nOpts
// Otherwise, return false.
//
bool
CmdExec::lexOptions
(const string& option, vector<string>& tokens, size_t nOpts) const
{
   string token;
   size_t n = myStrGetTok(option, token);
   while (token.size()) {
      tokens.push_back(token);
      n = myStrGetTok(option, token, n);
   }
   if (nOpts != 0) {
      if (tokens.size() < nOpts) {
         errorOption(CMD_OPT_MISSING, "");
         return false;
      }
      if (tokens.size() > nOpts) {
         errorOption(CMD_OPT_EXTRA, tokens[nOpts]);
         return false;
      }
   }
   return true;
}

CmdExecStatus
CmdExec::errorOption(CmdOptionError err, const string& opt) const
{
   switch (err) {
      case CMD_OPT_MISSING:
         cerr << "Error: Missing option";
         if (opt.size()) cerr << " after (" << opt << ")";
         cerr << "!!" << endl;
      break;
      case CMD_OPT_EXTRA:
         cerr << "Error: Extra option!! (" << opt << ")" << endl;
      break;
      case CMD_OPT_ILLEGAL:
         cerr << "Error: Illegal option!! (" << opt << ")" << endl;
      break;
      case CMD_OPT_FOPEN_FAIL:
         cerr << "Error: cannot open file \"" << opt << "\"!!" << endl;
      break;
      default:
         cerr << "Error: Unknown option error type!! (" << err << ")" << endl;
      exit(-1);
   }
   return CMD_EXEC_ERROR;
}

