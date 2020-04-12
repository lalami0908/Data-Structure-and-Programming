/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ==" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
   HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;

   public:
   
      iterator(){}
      iterator(vector <Data>* _buckets, size_t _numBuckets, size_t row, size_t col)
         :_buckets(_buckets), _numBuckets(_numBuckets), _row(row), _col(col){};
      ~iterator(){}
      Data& operator * () { return _buckets[_row][_col]; }
      const Data& operator * () const { return _buckets[_row][_col];}

      
      //??
      iterator& operator ++ () 
      { 
      /*   if(_row == _numBuckets)//??
         {

         }*/
         if(_col < _buckets[_row].size()-1)//?? -1
         {  
            _col++;
         }
         else
         {
            if(_row < _numBuckets)
            {
               _col = 0;
               while( _row < _numBuckets)
               {
                  _row++;
                  if(_buckets[_row].size()!= 0)
                     break;
               }  
            }          
         }
         return (*this); 
      }
      iterator operator++(int) 
      {
         iterator tmp = *this;
         ++*this;
         return tmp;
      }
      //??
      iterator& operator--()
      {
         if(_col > 0)
            _col--;
         else
         {
            while(_row > 0)
            {
               _row--;
               if(_buckets[_row].size()!= 0)
                  break;
            }
            _col = _buckets[_row].size()-1;
         }
         return *this;
      }

      iterator operator--(int) 
      {
         iterator tmp = *this;
         --*this;
         return tmp;
      }
      iterator& operator=(const iterator& rhs) 
      {
         _buckets = rhs._buckets;
         _numBuckets = rhs._numBuckets;
         _row = rhs._row;
         _col = rhs._col;
      }
      //??
      bool operator == (const iterator& rhs) const
      {
         return _buckets == rhs._buckets &&
                _numBuckets == rhs._numBuckets &&               
                _row == rhs._row &&
                _col == rhs._col;
      }
      bool operator != (const iterator& i) const 
      { 
         return !(*this == i); 
      }
   private:
      
      vector<Data>* _buckets;
      size_t _numBuckets;
      size_t _row;
      size_t _col;
   };

   void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const 
   {
      for(size_t i = 0;i < _numBuckets; i++)
      {
         if(_buckets[i].size())
            return iterator(_buckets, _numBuckets, i, 0);
      } 
   }
   // Pass the end
   iterator end() const 
   { 
      size_t i = _numBuckets - 1;
       return iterator(_buckets, _numBuckets, _numBuckets, 0); 
     // return iterator(_buckets, _numBuckets, i,_buckets[i].size()); 
   }
   // return true if no valid data
   bool empty() const
   { 
      if (size())
         return false;
      return true; 
   }
   // number of valid data
   size_t size() const 
   { 
      size_t s = 0; 
       
      for (iterator it = begin(); it != end(); it++) 
         s++;
      return s; 
   }

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const 
   { 
      size_t i = bucketNum(d);
      for (int j = 0; j < _buckets[i].size(); j++) 
      {
         if(_buckets[i][j] == d)
            return true;
         
      }
      return false; 

  /*    for (iterator it = begin(); it != end(); it++) 
      {
         if(*it == d)
            return true;
      }
      return false; */
      
   }

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const 
   {

      size_t i = bucketNum(d);
      for (int j = 0; j < _buckets[i].size(); j++) 
      {
         if(_buckets[i][j] == d)
         {
            d = _buckets[i][j];
            return true;
         }
      }
      return false;  
   /*   for (iterator it = begin(); it != end(); it++) 
      {
         if(*it == d) // == compare name
         {
            d = *it;
            return true;
         } 
      }
      return false; */
   }

   // update the entry in hash that is equal to d (i.e. == return true)
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) 
   { 
      size_t i = bucketNum(d);
      for (int j = 0; j < _buckets[i].size(); j++) 
      {
         if(_buckets[i][j] == d)
         {
            _buckets[i][j] = d;
            return true;
         }
      }
      return false;  
      
   /*   for (iterator it = begin(); it != end(); it++) 
      {
         if(*it == d) // == compare name
         {
            *it = d;
            return true;
         } 
      }
      this->insert(d);
      return false; */
   }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) 
   { 
      if(check(d))
         return false;
      _buckets[bucketNum(d)].push_back(d);
      return true; 
   }

   // return true if removed successfully (i.e. d is in the hash)
   // return fasle otherwise (i.e. nothing is removed)
   bool remove(const Data& d) 
   {
      size_t i = bucketNum(d);
      for (int j = 0; j < _buckets[i].size(); j++) 
      {
         if(_buckets[i][j] == d)
         {
            _buckets[i].erase(_buckets[i].begin() + j);
            return true;
         }
      }
      return false;  
   }

private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H