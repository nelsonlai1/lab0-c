#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harness.h"
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL)
        return NULL;
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;
    while (q->head != NULL) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
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
    if (q == NULL)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    snprintf(newh->value, strlen(s) + 1, "%s", s);
    if (q->head != NULL)
        newh->next = q->head;
    else {
        q->tail = newh;
        newh->next = NULL;
    }
    q->head = newh;
    q->size++;
    return true;
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
    if (q == NULL)
        return false;
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (newt == NULL)
        return false;
    newt->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newt->value == NULL) {
        free(newt);
        return false;
    }
    snprintf(newt->value, strlen(s) + 1, "%s", s);
    newt->next = NULL;
    if (q->tail != NULL)
        q->tail->next = newt;
    else
        q->head = newt;
    q->tail = newt;
    q->size++;
    return true;
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
    if (q == NULL || q->head == NULL)
        return false;
    if (sp != NULL)
        snprintf(sp, bufsize, "%s", q->head->value);
    list_ele_t *rm = q->head;
    q->head = q->head->next;
    free(rm->value);
    free(rm);
    q->size--;
    if (q->size == 0)
        q->tail = NULL;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL)
        return 0;
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
    if (q == NULL || q->head == q->tail)
        return;
    list_ele_t *cursor = NULL;
    q->tail = q->head;
    while (q->head != NULL) {
        list_ele_t *next = q->head->next;
        q->head->next = cursor;
        cursor = q->head;
        q->head = next;
    }
    q->head = cursor;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (q == NULL || q->head == q->tail)
        return;
    q->head = q->tail = merge_sort(q->head);
    while (q->tail->next)
        q->tail = q->tail->next;
}
list_ele_t *merge_sort(list_ele_t *head)
{
    if (head == NULL || head->next == NULL)
        return head;
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *l1 = merge_sort(head);
    list_ele_t *l2 = merge_sort(fast);
    return merge(l1, l2);
}

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t *head = NULL;
    list_ele_t **tmp = &head;
    while (l1 != NULL && l2 != NULL) {
        if (strcmp(l1->value, l2->value) < 0) {
            *tmp = l1;
            tmp = &(*tmp)->next;
            l1 = l1->next;
        } else {
            *tmp = l2;
            tmp = &(*tmp)->next;
            l2 = l2->next;
        }
    }
    if (l1 != NULL)
        *tmp = l1;
    if (l2 != NULL)
        *tmp = l2;
    return head;
}

/*
list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    if (l2 == NULL)
        return l1;
    if (l1 == NULL)
        return l2;
    if (strcmp(l1->value, l2->value) < 0) {
        l1->next = merge(l1->next, l2);
        return l1;
    }
    else {
        l2->next = merge(l1, l2->next);
        return l2;
    }
}
*/
/*
queue_t *merge_sort(queue_t *q)
{
    queue_t *q1 = q_new();
    queue_t *q2 = malloc(sizeof(queue_t));
    q2 = q;
    for (int i = 0; i <= (q->size) / 2; i++) {
        char *s = malloc(sizeof(char) * (strlen(q2->head->value) + 1));
        q_remove_head(q2, s, strlen(q2->head->value) + 1);
        q_insert_tail(q1, s);
    }
    q1 = merge_sort(q1);
    q2 = merge_sort(q2);
    return merge(q1, q2);
}
queue_t *merge(queue_t *q1, queue_t *q2)
{
    queue_t *qt = q_new();
    while (q1->head != NULL || q2->head != NULL) {
        if (q2->head == NULL || strcmp(q1->head->value, q2->head->value) < 0) {
            char *s = malloc(sizeof(char) * (strlen(q1->head->value) + 1));
            q_remove_head(q1, s, strlen(q1->head->value) + 1);
            q_insert_tail(qt, s);
        }
        else if (q1->head == NULL || strcmp(q1->head->value, q2->head->value) >
0) { char *s = malloc(sizeof(char) * (strlen(q2->head->value) + 1));
            q_remove_head(q2, s, strlen(q2->head->value) + 1);
            q_insert_tail(qt, s);
        }
    }
    return qt;
}
*/
