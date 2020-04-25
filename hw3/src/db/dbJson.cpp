/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream& operator >> (istream& is, DBJson& j)
{
    assert(j._obj.empty());//teacher's code first or last
    j.isRead = true;
    string key;
    string colon;
    string comma;
    int value = 0;	
    string curlyBrace;
    
    is >> curlyBrace;

    while(is >> key)
    {	
      if(key == "}")
        return is; 
      else
      {
        is >> colon >> value;
        //deal with ""
        key.erase(0,1);
        key = key.substr(0,key.length()-1);
        DBJsonElem* d;
        d = new DBJsonElem(key,value);
        j.add(*d);	
        is >> comma;
        if(comma == "}")
          break;
        else 
        {	
          comma.clear();
          key.clear();
          colon.clear();
          value = 0;
          delete d;
        }
      }
    }
    return is;
  
}


ostream& operator << (ostream& os, const DBJson& j)
{
   // TODO
  j.print();
   return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()
{
   // TODO
   _obj.clear();
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
   // TODO
  for(size_t i = 0; i < this-> size(); i++)
    if(_obj.at(i).key() == elm.key())
      return false;

   this->_obj.push_back(elm);
   
   return true;
}
//float double??
// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO

  if(this-> empty())
    return NAN;

	float ave = 0 ;
	float sum = 0;
	float count = 0;
  sum = this-> sum();
	count = this->_obj.size();


	ave = sum / count;
  return ave;
 //  return 0.0;  teacher's code
}
//??what does set idx to size() mean
// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   // TODO
  int maxN = INT_MIN;
 
  if(this-> empty())
  {
    idx = _obj.size();
    return INT_MIN;
  }

  int count = 0;	
	count = _obj.size();

	for(int i = 0; i < count; i++)
		if(_obj.at(i).value() > maxN)
		{	
      maxN = _obj.at(i).value();
      idx = i;
    }
  
   return  maxN;
}

//return INT_MIN(??)
// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::min(size_t& idx) const
{
   // TODO
  int minN = INT_MAX;
 
  if(this->empty())
  {
    idx = _obj.size();
    return INT_MAX;
  }
  int count = 0;	
	count = _obj.size();
		
	for(int i = 0; i < count; i++)
		if(_obj.at(i).value() < minN)
		{	
      minN = _obj.at(i).value();
      idx = i;
    }
   return  minN;
}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
   // TODO
  int s = 0;
  if(_obj.empty())
    return 0;

  int count = _obj.size();
  for(int i = 0; i < count; i++)
		s = this->_obj.at(i).value() + s;
	
   return s;
}

bool
DBJson::read(const string& jsonFile)
{
	ifstream inFile;
	inFile.open(jsonFile);
	if(inFile)
  {
   		isRead = true;
      return true;
  }
  return false; 
}

void 
DBJson::print() const
{
	cout << "{" << endl;
	if( _obj.empty() != true)
	{	
		
		int count =  _obj.size();
		for(int i = 0;i < count; i++)
		{
			cout << "  " << _obj.at(i); 
			if( i != (count-1))
				cout << "," << endl;
			else
				cout << endl;
		}

	}
	cout << "}" << endl;
  cout << "Total JSON elements: " << _obj.size() << endl;
};