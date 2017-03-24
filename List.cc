/******************************************************************************
 *
 * Name: Matt Jenkins
 *
 * Date: 2 March 2017
 *
 * Assignment: Project 3
 *
 * Purpose: Write a dynamic double linked list class.
 *
 *****************************************************************************/

#include <iostream>
#include <assert.h>
//#include <cs111.h>
#include "list.h"
using namespace std;

//HERE BE PRIVATE FUNCTIONS

// Purpose: Recursively deletes Node objects and Head_ptr.
// (used for destructing).
//
// Pre: None.
// Post: None.
void List::deleteNodes(List::Node::Node* Head_ptr) {
  if (Head_ptr == NULL) {
    return;
  }
  deleteNodes(Head_ptr->Next_ptr);
  delete Head_ptr;
}

//HERE BE PUBLIC FUNCTIONS

// Purpose: Creates a node class.
//
// Pre: None.
// Post: Node is created.
List::Node::Node(const Item& value, Node* prev_ptr, Node* next_ptr) {
  Value = value;
  Prev_ptr = prev_ptr;
  Next_ptr = next_ptr;
}
  
// Purpose: Constructor
//
// Pre: None.
// Post: An empty list is created.
List::List() {
  Head_ptr = NULL;
  Current_ptr = NULL;
}

// Purpose: Destructor
//
// Pre: None.
// Post: All memory associated with the list has been freed.
List::~List() {
  deleteNodes(Head_ptr);
}

// Purpose: Inserts an item before the current item 
//
// Pre: Current item is defined, or list is empty
// Post: The item has been placed in the list and becomes the current item
void List::insertBefore(const Item& item) {
  if (empty()) {
    Current_ptr = new Node(item);
    Head_ptr = Current_ptr;
    return;
  }
  Node* new_ptr = new Node(item);
  if (currentDefined()) {
    if (Current_ptr->Prev_ptr != NULL) {
      Node* currentPrev_ptr = Current_ptr->Prev_ptr;
      Current_ptr->Prev_ptr = new_ptr;
      currentPrev_ptr->Next_ptr = new_ptr;
      new_ptr->Next_ptr = Current_ptr;
      new_ptr->Prev_ptr = currentPrev_ptr;
      if (Current_ptr == Head_ptr) {
        Head_ptr = new_ptr;
      }
      Current_ptr = new_ptr;
    }
    else {
      Current_ptr->Prev_ptr = new_ptr;
      new_ptr->Next_ptr = Current_ptr;
      if (Current_ptr == Head_ptr) {
        Head_ptr = new_ptr;
      }
      Current_ptr = new_ptr;
    }
  }
}

// Purpose: Inserts an item after the current item 
//
// Pre: Current item is defined, or list is empty
// Post: The item has been placed in the list and becomes the current item
void List::insertAfter(const Item& item) {
  if (empty()) {
    Current_ptr = new Node(item);
    Head_ptr = Current_ptr;
    return;
  }
  Node* new_ptr = new Node(item);
  if (currentDefined()) {
    if (Current_ptr->Next_ptr != NULL) {
      Node* currentNext_ptr = Current_ptr->Next_ptr;
      Current_ptr->Next_ptr = new_ptr;
      currentNext_ptr->Prev_ptr = new_ptr;
      new_ptr->Prev_ptr = Current_ptr;
      new_ptr->Next_ptr = currentNext_ptr;
      Current_ptr = new_ptr;
    }
    else {
      Current_ptr->Next_ptr = new_ptr;
      new_ptr->Prev_ptr = Current_ptr;
      Current_ptr = new_ptr;
    }
  }
}

// Purpose: Access the current item in the list
//
// Pre: Current item is defined
// Post: The current item is returned
List::Item List::retrieve() const {
  assert(currentDefined());
  return Current_ptr->Value;
}

// Purpose: Remove an item from the list
//
// Pre: Current item is defined
// Post: The current item is removed. Current is set to the next item
//       following the removed one (if any). Otherwise, current item
//       is undefined
void List::remove() {
  assert(currentDefined());
  Node* Victim_ptr = Current_ptr;
  if (Victim_ptr->Next_ptr == NULL && Victim_ptr->Prev_ptr == NULL) {
    delete Victim_ptr;
    Head_ptr = NULL;
    Current_ptr = NULL;
    return;
  }
  else if (Victim_ptr->Next_ptr == NULL) {
    Current_ptr->Prev_ptr->Next_ptr = NULL;
    Current_ptr = NULL;
    delete Victim_ptr;
    return;
  }
  else if (Victim_ptr->Prev_ptr == NULL) {
    Current_ptr->Next_ptr->Prev_ptr = NULL;
    Head_ptr = Current_ptr->Next_ptr;
    Current_ptr = Head_ptr;
    delete Victim_ptr;
    return;
  }
  else {
    Current_ptr->Next_ptr->Prev_ptr = Current_ptr->Prev_ptr;
    Current_ptr->Prev_ptr->Next_ptr = Current_ptr->Next_ptr;
    Current_ptr = Current_ptr->Next_ptr;
    delete Victim_ptr;
    return;
  }
}

// Purpose: Set the current location to the beginning of the list
//
// Pre: None.
// Post: If the list is not empty, the current location points to the first
//       item in the list. Otherwise, current is undefined.
void List::head() {
  if (!empty()) {
    Current_ptr = Head_ptr;
    return;
  }
  Current_ptr = NULL;
}

// Purpose: Advance the current location to the next location in the list
//
// Pre: Current item is defined
// Post: The current location points to the next location in the list
void List::advance() {
  assert(currentDefined());
  if (Current_ptr->Next_ptr == NULL) {
    head();
  }
  Current_ptr = Current_ptr->Next_ptr;
}

// Purpose: Checks if current points to a valid item
//
// Pre: None.
// Post: Returns true if the current location is defined, false otherwise.
bool List::currentDefined() const {
  return Current_ptr != NULL;
}

// Purpose: Returns the number of items in the list
//
// Pre: None.
// Post: The number of items in the list is returned
size_t List::length() const {
  size_t count = 0;
  for (Node* ptr = Head_ptr; 
       ptr != NULL; 
       ptr = ptr->Next_ptr) {
    count++;
  }
  return count;
}

// Purpose: Returns true if the list is empty, false otherwise.
//
// Pre: None.
// Post: None.
bool List::empty() const {
  return Head_ptr == NULL;
}

// Purpose: Copies a stack from another stack.
//
// Pre: None.
// Post: This list becomes a copy of list
void List::copy(const List& list) {
  deleteNodes(Head_ptr);
  Head_ptr = NULL;
  Current_ptr = NULL;
  for (Node* temp = list.Head_ptr; temp != NULL; temp = temp->Next_ptr) {
    insertAfter(temp->Value);
  }
}

// Purpose: Output the list to the stream.
//
// Pre: None.
// Post: None.
void List::display(ostream& s) const {
  s << "[";
  if (empty()) {
    s << "]";
    s << '\n';
    return;
  }
  for (Node* ptr = Head_ptr; ptr != NULL; ptr = ptr->Next_ptr) {
    if (ptr->Next_ptr == NULL) {
      s << ptr->Value << "]";
    }
    else {
      s << ptr->Value << ", ";
    }
  }
  s << '\n';
}

