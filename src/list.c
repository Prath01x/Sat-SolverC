#include "list.h"

#include <assert.h>
#include <stdlib.h>

#include "err.h"
#include "util.h"

/**
 * Struct for encapsulating a single list element.
 */
typedef struct ListItem {
    struct ListItem* next;  // pointer to the next element (NULL if last)
    void* data;             // pointer to the data
} ListItem;

List mkList(void) {
    List res;
    res.head = NULL;
    return res;
}

void clearList(List* s) {
    if (s == NULL) {
        return;
    }

    struct ListItem* current = s->head;
    while (current != NULL) {
        struct ListItem* next = current->next;
        free(current);
        current = next;
    }

    s->head = NULL;
}

void push(List* s, void* data) {
    if (s == NULL) {
        return;
    }

    struct ListItem* newElement = malloc(sizeof(struct ListItem));
    if (newElement == NULL) {
        return;
    }

    newElement->data = data;
    newElement->next = s->head;
    s->head = newElement;
}

void* peek(List* s) {
    if (s == NULL || s->head == NULL) {
        return NULL;
    }

    return s->head->data;
}

void pop(List* s) {
    if (s == NULL || s->head == NULL) {
        return;
    }

    struct ListItem* topElement = s->head;
    s->head = topElement->next;
    free(topElement);
}

char isEmpty(List* s) { return s == NULL || s->head == NULL; }

ListIterator mkIterator(List* list) {
    ListIterator res;
    res.list = list;
    res.prev = NULL;
    res.current = list->head;

    return res;
}

void* getCurr(ListIterator* it) {
    assert(it->current != NULL);
    return it->current->data;
}

void next(ListIterator* it) {
    assert(isValid(it));
    it->prev = it->current;
    it->current = it->current->next;
}

char isValid(ListIterator* it) { return it->current != NULL; }
