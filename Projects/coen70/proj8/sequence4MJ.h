//
//  Created by Maggie Cai and Jeffrey Nguyen on 11/15/16.
//
//

// FILE: sequence4.h
// CLASS PROVIDED: sequence (part of the namespace main_savitch_5)
// This is the header file for the project described in Section 5.4
// of "Data Structures and Other Objects Using C++"
// This is called "sequence3" because some students already implemented
// sequence1 (with a fixed array) and sequence2 (with a dynamic array).
//
// TYPEDEFS and MEMBER CONSTANTS for the sequence class:
//   typedef ____ value_type
//     sequence<Item>::value_type is the data type of the items in the sequence. It
//     may be any of the C++ built-in types (int, char, etc.), or a class with a
//     default constructor, an assignment operator, and a copy constructor.
//
//   typedef ____ size_type
//     sequence<Item>::size_type is the data type of any variable that keeps track of
//     how many items are in a sequence.
//   
//   typedef node_iterator<Item> iterator
//     sequence<Item>::iterator is the forward iterator for a sequence
//
//   typedef const_node_iterator<Item> const_iterator
//     sequence<Item>::const_iterator is the forward iterator for a const sequence
//
// CONSTRUCTOR for the sequence class:
//   sequence( )
//     Postcondition: The sequence has been initialized as an empty sequence.
//
// MODIFICATION MEMBER FUNCTIONS for the sequence class:
//   void start( )
//     Postcondition: The precursor and cursor are adjusted such that the first
//     item in the sequence becomes the current item (but if the sequence is
//     empty, then there is no current item).
//
//   void advance( )
//     Precondition: is_item returns true.
//     Postcondition: If the current item was already the last item in the
//     sequence, then there is no longer any current item. Otherwise, the new
//     current item is the item immediately after the original current item.
//
//   void insert(const Item& entry)
//     Postcondition: A new copy of entry has been inserted in the sequence
//     before the current item. If there was no current item, then the new entry
//     has been inserted at the front of the sequence. In either case, the newly
//     inserted item is now the current item of the sequence.
//
//   void attach(const Item& entry)
//     Postcondition: A new copy of entry has been inserted in the sequence after
//     the current item. If there was no current item, then the new entry has
//     been attached to the end of the sequence. In either case, the newly
//     inserted item is now the current item of the sequence.
//
//   void remove_current( )
//     Precondition: is_item returns true.
//     Postcondition: The current item has been removed from the sequence, and
//     the item after this (if there is one) is now the new current item.
//
// CONSTANT MEMBER FUNCTIONS for the sequence class:
//   size_type size( ) const
//     Postcondition: The return value is the number of items in the sequence.
//
//   bool is_item( ) const
//     Postcondition: A true return value indicates that there is a valid
//     "current" item that may be retrieved by activating the current
//     member function (listed below). A false return value indicates that
//     there is no valid current item.
//
//   Item current( ) const
//     Precondition: is_item( ) returns true.
//     Postcondition: The item returned is the current item in the sequence.
//
// STANDARD ITERATOR MEMBER FUNCTIONS (provide a forward iterator):
//   iterator begin( )
//   const_iterator begin( ) const
//   iterator end( )
//   const iterator end( ) const
//
// VALUE SEMANTICS for the sequence class:
//    Assignments and the copy constructor may be used with sequence objects.

#ifndef COEN_70_SEQUENCE_H
#define COEN_70_SEQUENCE_H
#include <cstdlib>  // Provides size_t
#include "node2.h"  // Provides node class
#include <iterator>
#include <iostream>
#include <algorithm>//Provides copy function
#include <cassert>//For assert function
#include <fstream>//For files

namespace scu_coen70_6B
{
    template <class Item>
    class sequence
    {
    public:
        // TYPEDEFS and MEMBER CONSTANTS
        typedef Item value_type;
        typedef std::size_t size_type;
        typedef node_iterator<Item> iterator;
        typedef const_node_iterator<Item> const_iterator;
        // CONSTRUCTORS and DESTRUCTOR
        sequence( );
        sequence(const sequence& source);
        ~sequence( );
        // MODIFICATION MEMBER FUNCTIONS
        void start( );
        void advance( );
        void insert(const Item& entry);
        void attach(const Item& entry);
        void operator =(const sequence& source);
        void remove_current( );
        // CONSTANT MEMBER FUNCTIONS
        size_type size( ) const;
        bool is_item( ) const;
        Item current( ) const;
        // FUNCTIONS TO PROVIDE ITERATORS
        iterator begin( ) 
            { return iterator(head_ptr); }  //Iterator starts at the beginning of the sequence
        const_iterator begin( ) const 
            { return const_iterator(head_ptr); }    //Const iterator starts at the beginning of the sequence
        iterator end( )     
            { return iterator(); }  //Uses default constructor
        const_iterator end( ) const 
            { return const_iterator(); }    //Uses default constructor
    private:
        node<Item> *head_ptr;   //Head pointer for the sequence
        node<Item> *tail_ptr;   //Tail pointer for the sequence
        node<Item> *cursor;     //Pointer to the current item in the sequence
        node<Item> *precursor;  //Pointer to the previous item in the sequence
        size_type many_nodes;   //Number of nodes in the sequence
        
        void init();
    };

    
// *** IMPLEMENTATION ***
    
    //INVARIANT for the sequence class
    //  1. The number of items in the sequence is stored in the member variable "many_nodes"
    //
    //  2. For an empty sequence, we do not care what is stored in any of data and head_ptr
    //      and tail_ptr point to NULL; for a non-empty sequence, the items stored in the
    //      sequence can be accessed using head_ptr and tail_ptr and we don't care what is stored in the rest of the data.
    //
    //  3. If there is a current item, then it lies in *cursor; if there is no
    //     current item, then cursor* equals NULL.
    //
    //  4. If there is a previous item, then it lies in precursor*.  If there is no previous
    //       item, then precursor equals NULL.

    // Default private member variable initialization function.
    template <class Item>
    void sequence<Item>::init()
    {
        //Initializing pointers to NULL
        head_ptr = NULL;
        tail_ptr = NULL;
        cursor = NULL;
        precursor = NULL;
        //Initializiing many_nodes (or our counter) to 0
        many_nodes = 0;
    }
    
    //CONSTRUCTOR IMPLEMENTATION for default constructor
    template <class Item>
    sequence<Item>::sequence()
    {
        init();
    }
    
    //Copy Constructor
    template <class Item>
    sequence<Item>::sequence(const sequence<Item>& source)
    {
        init();
        *this = source;
    }
    
    //Destructor
    template <class Item>
    sequence<Item>::~sequence()
    {
        list_clear(head_ptr);//Returning data to the freestore
        many_nodes = 0;
        
        return;
    }
    
    template <class Item>
    void sequence<Item>::start()
    {
        //Sets cursor to first item
        cursor = head_ptr;
        precursor = NULL;
        
        return;
    }
    
    template <class Item>
    void sequence<Item>::advance()
    {
        //Testing that is_item returns true
        assert(is_item());
        
        precursor = cursor;
        cursor = cursor -> link();  //Sets cursor to next item
        
        return;
    }
    
    //Inserting data before the current index value
    template <class Item>
    void sequence<Item>::insert(const Item& entry)
    {
        //Testing precursor precondition, essentially at front of sequence if this is true
        if ((precursor == NULL) || (!is_item()))
        {
            //Inserts a value at the head if no current item
            list_head_insert(head_ptr, entry);
            cursor = head_ptr;
            precursor = NULL;
            
            //Setting tail pointer if adding a link to an empty list
            if (many_nodes == 0)
                tail_ptr = head_ptr;
        }
        else
        {
            //Inserts a value before the current item, if there is a current item
            list_insert(precursor, entry);
            //Setting current item to the new item
            cursor = precursor -> link();//Setting cursor to one link past precursor which should be the new item
        }
        
        ++many_nodes;
        
        return;
    }
    
    //Adding a value after the current item
    template <class Item>
    void sequence<Item>::attach(const Item& entry)
    {
        if (is_item())
        {
            // Check if tail_ptr needs to be updated
            bool wasAtEnd = (cursor == tail_ptr);
            //Attaches a value after the current item, if there is a current item
            precursor = cursor;
            list_insert(precursor, entry);
            cursor = cursor -> link();
            // Update tail_ptr if necessary
            if (wasAtEnd)
                tail_ptr = tail_ptr -> link();
        }
        // !is_item()
        else
        {
            //If list is empty, insert node at front of sequence
            if ((head_ptr == NULL) && (tail_ptr == NULL))
            {
                list_head_insert (head_ptr, entry);
                cursor = head_ptr;
                precursor = NULL;
                tail_ptr = head_ptr;
            }
            //if no current item, attach at end of list
            else
            {
                // Not necessary: precursor = list_locate (head_ptr, list_length (head_ptr));
                list_insert (precursor, entry);
                cursor = precursor -> link();
                tail_ptr = tail_ptr -> link();
            }
        }
        
        ++many_nodes;
        
        return;
    }
    
    //Removes the current_index value from the sequence
    template <class Item>
    void sequence<Item>::remove_current()
    {
        assert (is_item());//Checking precondition
        
        //Removing item from list if at head of list
        if (cursor == head_ptr)
        {
            if (many_nodes > 1)
            {
                list_head_remove(head_ptr);
                cursor = head_ptr;
                precursor = NULL;
            }
            else
            {
                list_head_remove(head_ptr);
                cursor = NULL;
                precursor = NULL;
                head_ptr = NULL;
                tail_ptr = NULL;
            }
        }
        
        //Removing item if not at head of list
        else
        {
            if (cursor == tail_ptr)
                tail_ptr = precursor;
            cursor = cursor -> link();//Advancing the cursor away from the node to be deleted
            list_remove(precursor);//Deleting target
        }
        
        --many_nodes;
        
        return;
    }
    
    template <class Item>
    void sequence<Item>::operator =(const sequence<Item>& source)
    {
        //Checking for self assignment
        if (this == &source)
            return;
        
        // Clear the list to free the memory and avoid memory leaks!
        list_clear(head_ptr);
        init();
        
        //If head_ptr = NULL implementation, copying using the null class member list_copy
        if (source.head_ptr == NULL)
        {
            list_copy(source.head_ptr, head_ptr, tail_ptr);
            cursor = NULL;
            precursor = NULL;
        }
        //If the cursor is on the first item on the list
        if (source.many_nodes == 1 || source.cursor == source.head_ptr)
        {
            list_copy(source.head_ptr, head_ptr, tail_ptr);
            // Figure out which cursor-precursor state the original, was in.
            if (source.cursor == source.head_ptr)
            {
                cursor = head_ptr;
                precursor = NULL;
            }
            else
            {
                cursor = NULL;
                precursor = head_ptr;
            }
            
        }
        //If the source cursor is not at the first item in the list and greater than 0
        else if (source.many_nodes > 1)
        {
            //Copying values in two pieces
            list_piece(source.head_ptr, source.cursor, head_ptr, precursor);
            if (source.cursor != NULL)
                list_piece(source.cursor, (node<Item>*)NULL, cursor, tail_ptr);
            else
            {
                cursor = NULL;
                tail_ptr = precursor;
            }
            //Putting the two separate pieces together
            precursor->set_link(cursor);
        }
        
        //Setting many_nodes variable
        many_nodes = source.many_nodes;
        
        return;
    }
    
    //CONSTANT MEMBER FUNCTIONS
    template <class Item>
    typename sequence<Item>::size_type sequence<Item>::size() const
    {
        return many_nodes;      //Returns number of nodes in sequence
    }
    
    template <class Item>
    bool sequence<Item>::is_item() const
    {
        return (cursor != NULL);    //Returns whether or not there currently is an item
    }
    
    template <class Item>
    Item sequence<Item>::current() const
    {
        assert (is_item());//Testing precondition of function
        
        return cursor -> data();    //Returns the current data
    }
    
}

#endif /* sequence4_hpp */





