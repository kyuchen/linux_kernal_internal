#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        goto end;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

end:
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    list_ele_t *ptr = NULL, *temp = NULL;

    if (q == NULL) {
        return;
    }
    ptr = q->head;
    while (ptr != NULL) {
        temp = ptr->next;
        free(ptr->value);
        ptr->value = NULL;
        free(ptr);
        ptr = temp;
    }
    q->head = NULL;
    q->tail = NULL;
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    int ret = true;

    if (q == NULL || s == NULL) {
        ret = false;
        goto end;
    }
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        ret = false;
        goto end;
    }

    newh->value = strdup(s);
    if (newh->value == NULL) {
        free(newh);
        newh = NULL;
        ret = false;
        goto end;
    }
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    newh->next = q->head;
    if (q->tail == NULL) {
        q->tail = newh;
    }
    q->head = newh;
    q->size++;

end:
    return ret;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;
    int ret = true;

    if (q == NULL || s == NULL) {
        ret = false;
        goto end;
    }
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        ret = false;
        goto end;
    }

    newh->value = strdup(s);
    if (newh->value == NULL) {
        free(newh);
        newh = NULL;
        ret = false;
        goto end;
    }
    newh->next = NULL;
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (q->head == NULL) {
        q->head = newh;
    }
    if (q->tail == NULL) {
        q->tail = newh;
    } else {
        q->tail->next = newh;
        q->tail = q->tail->next;
    }
    q->size++;

end:
    return ret;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *tmp = NULL;
    bool ret = true;

    if (q == NULL || q->head == NULL) {
        ret = false;
        goto end;
    }

    if (sp != NULL && bufsize > 1) {
        strncpy(sp, q->head->value, bufsize);
        sp[bufsize - 1] = '\0';
    }

    tmp = q->head;
    q->head = q->head->next;

    free(tmp->value);
    tmp->value = NULL;
    free(tmp);
    tmp = NULL;
    q->size--;

end:
    return ret;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL) {
        return 0;
    }

    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    list_ele_t *ptr = NULL, *tmp = NULL;

    if (q == NULL || q->head == NULL) {
        return;
    }

    q->tail = q->head;
    ptr = q->tail;
    while (ptr != NULL && ptr->next != NULL) {
        tmp = ptr->next;
        ptr->next = ptr->next->next;
        tmp->next = q->head;
        q->head = tmp;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
static void swapValue(int *a, int *b)
{
    if (*a != *b) {
        *a = *a ^ *b;
        *b ^= *a;
        *a ^= *b;
    }
}

static void qsortImplement(list_ele_t *start, list_ele_t *end)
{
    list_ele_t *pivot = NULL, *ptr = NULL;

    if (start == NULL || end == NULL || start == end) {
        return;
    }
    pivot = start;
    ptr = start;

    do {
        ptr = ptr->next;

        if (strcmp(pivot->value, ptr->value) > 0) {
            swapValue((int *) &pivot->value, (int *) &ptr->value);
            pivot = pivot->next;
            swapValue((int *) &pivot->value, (int *) &ptr->value);
        }
    } while (ptr != NULL && ptr != end && pivot != NULL && pivot != end);

    qsortImplement(start, pivot);
    qsortImplement(pivot->next, end);
}

void q_sort(queue_t *q)
{
    if (q == NULL || q->size <= 1) {
        return;
    }
    qsortImplement(q->head, q->tail);
}
