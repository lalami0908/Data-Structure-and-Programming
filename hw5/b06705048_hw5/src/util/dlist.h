/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
      _isSorted = false;//??not sure
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }//?? original return *(this)
      T& operator * () { return _node->_data; }
      
      
      iterator& operator ++ () 
      {
         _node =  _node->_next;
         return *(this); 
      }
      iterator operator ++ (int) 
      {
         iterator original = *this;
         _node =  _node->_next;
         return original; 
      }
      iterator& operator -- () 
      { 
         _node =  _node->_prev;
         return *(this); 
      }
      iterator operator -- (int) 
      {
         iterator original = *this;
         _node =  _node->_prev; 
         return original; 
      }

      iterator& operator = (const iterator& i) 
      { 
         this->_node = i._node;
         return *(this); 
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
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const 
   { 
     // size_t a = size();
     // cout << "size = " << a << endl;
      return iterator(_head); 
   }
   iterator end() const //get dummy node
   {
      iterator res = begin();
      return --res; 
   }
   bool empty() const 
   {
    //  if(_head == 0)
    //     return true; 
    //  return false;
   
   if(begin() == end())
      return true;
   return false;

   }
   size_t size() const 
   {
      size_t res = 0;
     // bool test = false;
     // test = empty();
    //  cout << "empty = " <<  test << endl;
    //  if( empty())
    //     return res;

      for(iterator i = begin(); i != end(); i++)
         res++;  
      
      return res; 
   }

   void push_back(const T& x) 
   {
    /*  cout << "?";
      DListNode<T> add(x, tail()._node, end()._node);
      cout << "?";
      if(size() == 0)
         _head = &add;
      cout << "?";
      
      tail()._node->_next = &add;
      end()._node->_prev = &add;*/
      
      //dummy node
      
      DListNode<T>* lastNode = tail()._node;
      DListNode<T>* dummyNode = end()._node;
      DListNode<T>* newNode = new DListNode<T>(x, lastNode, dummyNode);
      if(empty())
         _head = newNode;
      dummyNode ->_prev = newNode;
      lastNode ->_next = newNode;

    
       
     
     
      
      
      _isSorted = false;
   }
   void pop_front() 
   { 
      if(empty() == true)
         return;

      DListNode<T>* first = begin()._node;
      DListNode<T>* second = begin()._node->_next;
      DListNode<T>* dummy = end()._node;

      _head = second;
      _head->_prev = dummy;
      dummy -> _next = _head;

      delete first;
      first = 0;
      
   }
   void pop_back() 
   { 
       if(empty() == true)
         return;
      DListNode<T>* get_tail = tail()._node;
      DListNode<T>* second_last = tail()._node ->_prev;
      DListNode<T>* dummy = end()._node;

      second_last-> _next = dummy;
      dummy -> _prev = second_last;
      
      delete get_tail;
      get_tail = 0;

   }

   // return false if nothing to erase
   bool erase(iterator pos) 
   { 
    //  for(iterator i = begin(); i != end(); i++)
    //  {
    //     if(i == pos)
    //     {
            // on the first 
            // ?? how about only one 
         if(pos._node)
         {
            if(pos == begin())
               _head = _head-> _next;

            //general

            DListNode<T>* get = pos._node;
            get->_prev->_next = get->_next;
            get->_next->_prev = get->_prev;
            
            delete get;
            get = 0;
            return true;
         }
    //     }
    //  }
      return false; 
   }
   bool erase(const T& x) 
   { 
      for(iterator i = begin(); i != end(); i++)
      {
         if(i._node->_data == x)
         {
            erase(i);
            return true;
         }
      }
      return false; 
   }

   iterator find(const T& x) 
   {
      for(iterator i = begin(); i != end(); i++)
      {
         if(i._node->_data == x)
            return i;
      }
      return end(); 
   }

   void clear() 
   { 
      // delete all nodes 
      // except for the dummy node ?? how
      iterator temp = begin();


      for(iterator i = begin(); i != end(); )
      {
         iterator temp = i;
         i++;
         delete temp._node;
         temp._node = 0;
      }

      //initialize
      _head = end()._node;
      _head->_prev = _head->_next = _head;
   }  

   void sort() const //choose which sort
   {   /*
          if(_isSorted == true)
         return;

      for(iterator i = begin();i != end(); i++)
      {
         for(iterator j = i; j != end(); j++)
         {
            if(i._node->_data > j._node-> _data)
            {
               T temp = i._node->_data;
               i._node->_data = j._node-> _data;
               j._node-> _data = temp;
            }
         }
      }
      _isSorted = true;
         */
      if(_isSorted == true)
         return;
      
      QuickSort(begin(),tail());
   /*   for(iterator i = begin();i != tail(); i++)
      {
       
         bool swapped = false;
         for(iterator j = begin(); (i._node -j._node) > 0; j++)
         {
            if(j._node->_data > j._node->_next-> _data)
            {
               //swap(i,j);
               swap(*(j), *(++j));
            //   T temp = j._node->_next-> _data;
            //   j._node->_next->_data = j._node-> _data;
            //   j._node-> _data = temp;
               swapped = true;
            }
         }
         if(!swapped)
            break;
      }*/
      _isSorted = true;
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions

   iterator tail() const //getthe last
   {
      
      iterator res = end();
      return --res; 
   }
/*   void swap(iterator a, iterator b)
{
    T temp = a._node->_data;
    a._node->_data = b._node-> _data;
    b._node-> _data = temp;
}*/
   void swap(T& a,T&  b)const
{
    T temp = a;
    a = b;
    b = temp;
}
   iterator Partition(iterator front, iterator end) const
   {
      iterator pivot = end;
      iterator i = front;
      i--;
    
      for (iterator j = front; j != end; j++) 
      {
         if (*j < *end) 
         {
            //   i = (i == this->end()) ? front : i->next;
               if(i == this->end())
                  i = front;
               else
                  i++; 
               swap(*i, *j);
              
         }
      }
      if(i == this->end())
         i = front;
      else
         i++; 
      
      swap(*i, *end);
      return i;
   }
   void QuickSort(iterator front, iterator end) const
   
   {//&& end != this->end()
   //&& front != this -> end()
    if(front != end && end != this->end() && front._node != end._node ->_next)
    {
        iterator pivot = Partition(front, end);
       
      
        QuickSort(front, iterator(pivot._node->_prev));
        QuickSort(iterator(pivot._node->_next), end);
    }
   }
};


#endif // DLIST_H
