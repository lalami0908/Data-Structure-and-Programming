/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

#define num_p(x) ((x-1)/2)
#define num_l(x) ((x*2) +1)
#define num_r(x) ((x*2) +2)

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const 
   {
      if(_data.size() != 0) 
         return _data[0];
      //how about empty? 
   }
   void insert(const Data& d) 
   { 
      size_t i = size();
      _data.push_back(d);
      
      while (i > 0 && _data[i] < _data[num_p(i)]) 
      {
         swap(_data[i], _data[num_p(i)]);
         i = num_p(i);
      }
   }
   void delMin() {delData(0); }
   void delData(size_t i) 
   {
      swap(_data[i], _data[size()-1]);
      _data.pop_back();
      size_t s = size();


       while (num_r(i) < s) 
       {
         if (_data[num_r(i)] < _data[i] && _data[num_r(i)] < _data[num_l(i)]) 
         {
            swap(_data[i], _data[num_r(i)]);
            i = num_r(i);
         } 
         else if (_data[num_l(i)] < _data[i]) 
         {
            swap(_data[i], _data[num_l(i)]);
            i = num_l(i);
         } 
         else 
            break;
      }

      // special case: two iteams
      // only left child
      if (num_r(i) == s && _data[num_l(i)] < _data[i])
         swap(_data[i], _data[num_l(i)]);
   }
   

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
