/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(0) 
   {
      _isSorted = false;

   }
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const
      { 
         return (*_node); //(*this) 
      }
      T& operator * () { return (*_node); }
      iterator& operator ++ ()  //++i
      { 
         _node ++;
         return (*this); 
      }
      iterator operator ++ (int) 
      {
         iterator original = iterator(*this);
         _node ++; 
         return original;  
          
      }
      iterator& operator -- () 
      { 
         _node --; //--_node?
         return (*this); 
      }
      iterator operator -- (int) 
      {
         iterator original = iterator(*this);
         _node --; 
         return original;   
      }

      iterator operator + (int i) const  //not sure what it mean
      { 
         iterator res = iterator(*this);
         res._node = (res._node) +i; //(T*) + i or for i times??
         return res;
      }
      iterator& operator += (int i) 
      {
         _node = _node + i; 
         return (*this); 
      }

      iterator& operator = (const iterator& i) 
      { 
         _node = i._node;
         return (*this); 
      }

      bool operator != (const iterator& i) const 
      {
          if(this->_node != i._node)//compare node??
            return true;
         return false;  
      }
      bool operator == (const iterator& i) const 
      { 
         if(this->_node == i._node)
            return true;
         return false; 
      }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const 
   {
      return iterator(_data); 
   }
   iterator end() const 
   { 
      iterator res = begin() + _size;
      return res; 
      //?? or iterator(_data + _size) 
   }
   bool empty() const 
   { 
      if(_size == 0)//if(begin() == end())
         return true;
      return false;
   }
   size_t size() const 
   { 
      return _size;
   }

   T& operator [] (size_t i) 
   { 
      return _data[i]; 
   }
   const T& operator [] (size_t i) const 
   { 
      return _data[i]; 
   }

   void push_back(const T& x) 
   { 
      if(_capacity == 0)
      {
         _data = new T[1];
         _capacity = 1;
      }
      else if(_size == _capacity)
      {
          T* temp  =_data;
         _data = new T[_capacity*2];

         for(size_t i = 0;i < _size;i++)
            _data[i] = temp [i];

         _capacity = _capacity * 2;
         delete[] temp;
         temp = 0;
      }
 
      _data[_size] = x;
      _size++;
      _isSorted = false;
   }
   void pop_front() 
   {
      if(empty()!= true)
      {
         _data[0] = _data[_size-1];
         _size--;    
      } 
      _isSorted = false;
   }
   void pop_back() //delete or just change size??
   { 
      if(_size != 0)
      {
         _size--;    
      } 
   }

   bool erase(iterator pos) 
   {
      iterator first = begin();
      size_t index = pos._node - first._node;
   //   cout << "a = " << index << endl;
      
      if(index < _size)//check range??
      {
         if(*(pos._node) == _data[_size-1])
            pos._node = NULL;
         else
            *(pos._node) = _data[_size-1];
         _size--;
         return true;
      }
      return false;
     /* for(iterator i = begin(); i != end(); i++)
      {
         if(i == pos)
         {  
           *(i._node) = _data[_size-1]; //?? really change the data? 
            _size--;
            if(i != (--end()))
       //        _isSorted = false;
            return true;
         }
      }
      return false; */
   }
   bool erase(const T& x) 
   {
     
   /*   if(find(x) != end()) //if find faster or not
      {
         *(i._node) = _data[_size-1];
         _size--;
         return true;
      }
      return false; */ 
      for(iterator i = begin(); i != end(); i++)
      {
         if(*(i._node) == x)
         {  
            *(i._node) = _data[_size-1]; //?? really change the data? 
            _size--;
          //  if(i != end()-1)
               _isSorted = false;
            return true;
         }
      }
      return false; 
   }

   iterator find(const T& x) 
   {

      for(iterator i = begin(); i != end(); i++)
      {
         if(*(i._node) == x)
         {  
            return i;
         }
      }
      return end(); //?
   }

   void clear() {_size = 0; }

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()??
   void sort() const { if (!empty()) ::sort(_data, _data+_size); }

   // Nice to have, but not required in this homework...
   // void reserve(size_t n) { ... }
   // void resize(size_t n) { ... }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions

};

#endif // ARRAY_H
