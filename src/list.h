#pragma once

/**
 * This file provides an implementation of a singly linked list.
 *
 * The list can be used as a last-in-first-out stack by only using the
 * following functions:
 *  push, peek, pop, isEmpty
 *
 * It also provides an iterator to iterate over the list elements.
 */

struct ListItem;

/**
 * A list.
 *
 * Should only be created by calling mkList.
 */
typedef struct List {
    struct ListItem* head;
} List;

/**
 * Creates a new empty list.
 *
 * @return  the new list
 */
List mkList(void);

/**
 * Removes all elements from a list.
 *
 * The data of the list is not freed.
 *
 * @param s  a list
 */
void clearList(List* s);

/**
 * Adds a new element to the head of a list.
 *
 * @param s     a list
 * @param data  the data for the new element
 */
void push(List* s, void* data);

/**
 * Returns the data of the head element of a list without removing it.
 *
 * The list must not be empty.
 *
 * @param s  a list
 * @return   the data
 */
void* peek(List* s);

/**
 * Removes the head element of a list.
 *
 * The data of the removed element is not freed.
 * The list must not be empty.
 *
 * @param s  a list
 */
void pop(List* s);

/**
 * Checks whether a list is empty.
 *
 * @param s  a list
 * @return   1 if it is empty, 0 otherwise
 */
char isEmpty(List* s);

/**
 * A list iterator.
 *
 * An iterator points to a position in a list. Possible positions are all
 * elements of a list and the position directly after the last element. If an
 * iterator points to the latter, it is invalid.
 *
 * Should only be created by calling mkIterator.
 *
 * Example:
 * Let l be a list of elements of type T. To call the function void foo(T*) on
 * every contained element, the following code can be used:
 *
 *  ListIterator it = mkIterator(&l);
 *
 * while (isValid(&it)) {
 *    T* current = (T*) getCurr(&it);
 *    foo(current);
 *    next(&it);
 * }
 */
typedef struct ListIterator {
    struct List* list;
    struct ListItem* prev;
    struct ListItem* current;
} ListIterator;

/**
 * Creates a new iterator for a list, starting at the head element.
 *
 * @param list  a list
 * @return      the iterator
 */
ListIterator mkIterator(List* list);

/**
 * Returns the data of the element an iterator points to.
 *
 * The iterator must not be invalid.
 *
 * @param it  an iterator
 * @return    the data
 */
void* getCurr(ListIterator* it);

/**
 * Advances an iterator to the next element.
 *
 * The iterator must not be invalid.
 *
 * @param it  an iterator
 */
void next(ListIterator* it);

/**
 * Checks whether an iterator points to a valid element.
 *
 * @param it  an iterator
 * @return    0 if it points to the position after the last element,
 *            1 otherwise
 */
char isValid(ListIterator* it);
