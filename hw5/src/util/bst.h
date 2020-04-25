/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
   //all private

   //why  BSTreeNode *parent =NULL cannot work
   BSTreeNode(const T& data,BSTreeNode* parent = NULL, BSTreeNode* left = NULL, BSTreeNode* right = NULL)
   :_data(data),_parent(parent),_left(left),_right(right) {}
          
        
   ~BSTreeNode(){//delete _left;_left = 0;delete _right;  _right = 0;
   }

   

   T               _data;
   BSTreeNode<T>*  _parent;
   BSTreeNode<T>*  _left;
   BSTreeNode<T>*  _right;
   

   bool isLeaf() const{return false;};//todo

   friend class BSTree<T>;
   friend class BSTree<T>:: iterator;
};


template <class T>
class BSTree
{
   public:
      // TODO: design your own class!!
      BSTree(): _root(NULL)
      {
         T dummy;
         _dummy = new BSTreeNode<T>(dummy,0,_root,0);
         _dummy->_parent = _dummy;
      }
      ~BSTree(){}

      class iterator 
      {
         public:

            friend class BSTree<T>;

            iterator(BSTreeNode<T>* n = 0,BSTreeNode<T>* d = 0) : _node(n), dummy(d) {}
            iterator(const iterator& i) : _node(i._node) {}

            ~iterator() {} // Should NOT delete _node
            
            // TODO: implement these overloaded operators
            const T& operator * () const { return _node->_data; }
            T& operator * () { return _node->_data; }
            iterator& operator ++ () //prefix
            { 
               _node = successor(_node);
                   return *this;
            }
            iterator operator ++ (int) 
            {
               iterator original = *this;
               _node = successor(_node);
               return original; 
            }

            iterator& operator -- () 
            {
               
               
               if(_node == leftmost(_node)) // empty???
               {
               //   iterator end = iterator(NULL);
               //   *this = end;
                  
      
               }
               else 
                  _node = predecessor(_node);
               return *this;
            }
            iterator operator -- (int) 
            { 
               iterator original = *this;
               if(_node == leftmost(_node))
               {
               //   return iterator(NULL);
               }
               else
                  _node = predecessor(_node);
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
            BSTreeNode<T>* _node;  
            BSTreeNode<T>* dummy;
      };
 
      // TODO: implement these functions
      iterator begin() const 
      {
         BSTreeNode<T>* min = leftmost(_root); 
         return iterator(min); 
      }
      iterator end() const 
      { 
      //   BSTreeNode<T>* max = rightmost(_root);
      //  return iterator(NULL,max);
      //   if(empty())
      //      return iterator(_root);
         return iterator(NULL);
      }
      bool empty() const 
      {
         if(_root == NULL)
            return true; 
         return false; 
      }
      size_t size() const 
      { 
         size_t res = 0;
      

         for(iterator i = begin(); i != end(); i++)
            res++;  
         
         return res;     
      }

   //   void push_back(const T& x) { }
      void pop_front()
      {
          iterator front = iterator(leftmost(_root));
          erase(front);

      }
      void pop_back() 
      {
      //   cout << "back" << endl;
         iterator back = iterator(rightmost(_root));
         erase(back);
      //   cout << "back_ finish" << endl;

      }

      // return false if nothing to erase

      bool erase(iterator pos) 
      {
         
         if(empty())
            return false;
         if(pos._node)
         {
            if(pos._node->_left == NULL && pos._node->_right == NULL)
            {
            //   cout << "here";
              
               if(pos._node->_data > pos._node-> _parent-> _data)
                  pos._node-> _parent->_right = NULL;
                
               else
                  pos._node-> _parent->_left = NULL;

               if(pos._node != _root)
               {
                  delete pos._node;
                  pos._node = NULL;

               }
               else
                  _root = NULL;
               

           
            }
            else if(pos._node->_left == NULL)
            {
               if(size()==2)
               {
                  _root = pos._node->_right;
                  _root-> _parent = NULL;
               }
                 
               else if(pos._node!=_root)
               {
                  if(pos._node->_data > pos._node-> _parent-> _data)
                  {   
                     pos._node->_parent->_right = pos._node->_right;
                     pos._node->_right->_parent = pos._node-> _parent;
                  }
                  else
                  {
                     pos._node-> _parent->_left = pos._node->_right;
                     pos._node->_right->_parent = pos._node-> _parent;
                  }
                  delete pos._node;
                  pos._node = NULL;
               }
               else 
               {
                  _root = pos._node->_right;
                  _root->_parent = NULL;
               }
               
            }
            else if(pos._node->_right == NULL)
            {
                  if(size()==2)
                  {
                     _root = pos._node->_left;
                     _root-> _parent = NULL;
                  }   
      
                  else if(pos._node!=_root)
                  {

                     if(pos._node->_data > pos._node-> _parent-> _data)
                     {   
                        pos._node->_parent->_right = pos._node->_left;
                        pos._node->_left->_parent = pos._node-> _parent;
                     }
                     else
                     {
                        pos._node-> _parent->_left = pos._node->_left;
                        pos._node->_left->_parent = pos._node-> _parent;
                     }
                     delete pos._node;
                     pos._node = 0; 
                  } 
                  else
                  {
                     _root = pos._node->_left;
                     _root->_parent = NULL;
                  }     
            }
            else  if(pos._node->_left != NULL && pos._node->_right != NULL)
            {
               
               iterator move = iterator(successor(pos._node));
               if(move._node->_right != NULL)
               {
                  pos._node->_data = move._node->_data;
                  move._node->_parent->_left->_data = move._node->_right->_data;
                  delete move._node;
                  move._node = NULL;       
               }
               else // move is leaf
               {
                  pos._node->_data = move._node->_data;
                  delete move._node->_parent->_left;
                  move._node->_parent->_left = NULL;
               }              
            }  
            return true;       
         }
         return false;
      }
      bool erase(const T& x) 
      {
         iterator get = find(x);
         if(get!= end())
         {
            erase(get);
            return true;
         }
   
         return false;  
      }

      iterator find(const T& x) 
      {
         iterator res(_root);

         while(true)
         {
            if(res._node->_data == x)
               return res;
            else if(x > res._node->_data && res._node->_right != 0)
               res._node = res._node->_right;
            else if(x < res._node->_data && res._node->_left != 0)
               res._node = res._node->_left;
            else 
               return end(); 
         }

      }

      static BSTreeNode<T>* leftmost(BSTreeNode<T>*cur) 
      {
         while (cur-> _left != NULL)
            cur = cur-> _left;
      
         return cur;
      }
      //cannot call member function without object
      static BSTreeNode<T>* successor(BSTreeNode<T>* cur)  //the next lager than cur
      {
         //have right
         if (cur-> _right != NULL)
            return leftmost(cur->_right);
      

         //only have left

         BSTreeNode<T>* succ = cur-> _parent;   
         while (succ != NULL && cur == succ-> _right) 
         {
            cur = succ;
            succ = succ-> _parent;
         }
         return succ;
      }

      static BSTreeNode<T>* rightmost(BSTreeNode<T>*cur) 
      {
         while (cur-> _right != NULL)
            cur = cur-> _right;
      
         return cur;
      }

      static BSTreeNode<T>* predecessor(BSTreeNode<T>* cur)  //the next smaller than cur
      {
         //have left
         if (cur-> _left != NULL)
            return leftmost(cur->_left);
      

         //only have right

         BSTreeNode<T>* pre = cur-> _parent; 

         while (pre != NULL && cur == pre-> _left) 
         {
            cur = pre;
            pre = pre-> _parent;
         }
         return pre;
      }

      void insert(const T& anEntry)
      {
      //   insert_re(anEntry, _root);

         if(_root == NULL) //empty
         {
            _root = new BSTreeNode<T>(anEntry,NULL,NULL,NULL);
            return;
         }

         BSTreeNode<T>* on = 0;        
         BSTreeNode<T>* now = 0;        
         BSTreeNode<T>* new_node = new BSTreeNode<T>(anEntry,on,NULL,NULL);   
         
         now = _root;

         while (now != NULL) 
         {                     
            on = now;                          
            if (anEntry < now->_data)
                  now = now->_left;
            else
                  now = now->_right;
         }                                   

         new_node->_parent = on;            
             
         if (new_node->_data < on->_data)
         {
            on->_left = new_node; 
         //   cout << "l" << endl;
         }     
         else
         {
            on->_right = new_node;
         //   cout << "r" << endl;
         }
      }

      void clear() {_root = NULL;}

      void sort() const {}

    
     
      void print()const 
      {
         if(this->_root) // print nothing if empty
         {
          
         }
         #if ADTP_DEBUG
            
         #endif // #if ADTP_DEBUG              
      }


   //   int getHeight() const{return 0;} 
   //   int getNumberOfNodes() const{return 0;}
   //   T getRootData() const{return _root;}//  throw(PrecondViolatedExcep); 
   //   void setRootData(const T& newData){} 
    
   //   T getEntry(const T& anEntry) const{} //throw(NotFoundException); 
   //   bool contains(const T& anEntry) const{return false;}


   

   private:
      BSTreeNode<T>* _root;
      BSTreeNode<T>* _dummy;
};

#endif // BST_H