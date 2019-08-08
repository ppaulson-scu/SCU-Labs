// FILE: sequence.h
// Lab 8
// Thomas King and Asa Jacob
//
// PROVIDES: A template class for a sequence of entries. The first template parameter Item is
// the type of data in each entry. The second template parameter SizeType is the data type used to
// keep track of how many entries are in the sequence, and it defaults to size_t.
// This file also defines a template class iterator that is a forward iterator. 
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
//   SizeType size( ) const
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
//   	Provides an iterator that points to the first item in the sequence.
//   const_iterator begin( ) const
//   	Provides an const_iterator that points to the first item in the sequence.
//   iterator end( )
//   	Provides an iterator that points after the last item in the sequence.
//   const iterator end( ) const
//   	Provides an const_iterator that points after the last item in the sequence.
//
// VALUE SEMANTICS for the sequence class:
//    Assignments and the copy constructor may be used with sequence objects.

#ifndef COEN_70_SEQUENCE_H
#define COEN_70_SEQUENCE_H
#include <cstdlib>  // Provides size_t
#include <iostream>
#include <cassert> // For assert function
#include "node2.h"  // Provides node class

namespace scu_coen70_6B
{
	template <class Item, class SizeType = std::size_t>
    class sequence
    {
    public:
        // TYPEDEFS and MEMBER CONSTANTS
		typedef Item value_type;
		typedef SizeType size_type;
        typedef node_iterator<Item> iterator;
        typedef const_node_iterator<Item> const_iterator;
        // CONSTRUCTORS and DESTRUCTOR
        sequence( );
        sequence(const sequence<Item, SizeType>& source);
        ~sequence( );
        // MODIFICATION MEMBER FUNCTIONS
        void start( );
        void advance( );
        void insert(const Item& entry);
        void attach(const Item& entry);
        void operator =(const sequence<Item, SizeType>& source);
	    void remove_current( );
        // CONSTANT MEMBER FUNCTIONS
        SizeType size( ) const;
        bool is_item( ) const;
        Item current( ) const;
        // FUNCTIONS TO PROVIDE ITERATORS
        iterator begin( );
        const_iterator begin( ) const;
        iterator end( );
        const_iterator end( ) const;
    private:
    	node<Item> *head_ptr;
    	node<Item> *tail_ptr;
    	node<Item> *cursor;
    	node<Item> *precursor;
    	SizeType many_nodes;

        void init();
    };

	// IMPLEMENTATION
	//
	// Invariants:
	//  1. head_ptr points to the first entry in the sequence,
	//     or NULL if the sequence is empty.
	//  2. tail_ptr points to the last entry in the sequence,
	//     or NULL if the sequence is empty.
	//  3. cursor points to the current entry.
	//  4. precursor points to the entry before cursor.
	//  5. many_nodes is the number of entries in the sequence.

	// Default private member variable initialization function.
	template<class Item, class SizeType>
    void sequence<Item, SizeType>::init()
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
	template<class Item, class SizeType>
    sequence<Item, SizeType> :: sequence ()
    {
        init();
    }

    //Copy Constructor
	template<class Item, class SizeType>
    sequence<Item, SizeType> :: sequence(const sequence<Item, SizeType>& source)
    {
        init();
        *this = source;
    }

    //Destructor
	template<class Item, class SizeType>
    sequence<Item, SizeType> :: ~sequence<Item, SizeType>()
    {
        list_clear(head_ptr);//Returning data to the freestore
        many_nodes = 0;

        return;
    }

	template<class Item, class SizeType>
    void sequence<Item, SizeType> :: start()
    {
        cursor = head_ptr;
        precursor = NULL;

        return;
    }

	template<class Item, class SizeType>
    void sequence<Item, SizeType> :: advance()
    {
        //Testing that is_item returns true
        assert(is_item());

        precursor = cursor;
        cursor = cursor -> link();

        return;
    }

    //Inserting data before the current index value
	template<class Item, class SizeType>
    void sequence<Item, SizeType> :: insert(const Item& entry)
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
	template<class Item, class SizeType>
    void sequence<Item, SizeType> :: attach(const Item& entry)
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
	template<class Item, class SizeType>
    void sequence<Item, SizeType> :: remove_current()
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

	template<class Item, class SizeType>
    void sequence<Item, SizeType> :: operator =(const sequence<Item, SizeType>& source)
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
                list_piece(source.cursor, (node<Item>*) NULL, cursor, tail_ptr);
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
	template<class Item, class SizeType>
    typename sequence<Item, SizeType>::size_type sequence<Item, SizeType> :: size() const
    {
        return many_nodes;
    }

	template<class Item, class SizeType>
    bool sequence<Item, SizeType> :: is_item() const
    {
        return (cursor != NULL);
    }

	template<class Item, class SizeType>
    typename sequence<Item, SizeType> :: value_type sequence<Item, SizeType> :: current() const
    {
        assert (is_item());//Testing precondition of function

        return cursor -> data();
    }

	//ITERATOR FUNCTIONS
	template<class Item, class SizeType>
	typename sequence<Item, SizeType> :: iterator sequence<Item, SizeType> :: begin()
	{
		return iterator(head_ptr);
	}

	template<class Item, class SizeType>
	typename sequence<Item, SizeType> :: const_iterator sequence<Item, SizeType> :: begin() const
	{
		return const_iterator(head_ptr);
	}

	template<class Item, class SizeType>
	typename sequence<Item, SizeType> :: iterator sequence<Item, SizeType> :: end()
	{
		return iterator();
	
	}
	template<class Item, class SizeType>
	typename sequence<Item, SizeType> :: const_iterator sequence<Item, SizeType> :: end() const
	{
		return const_iterator();
	}
}

#endif
