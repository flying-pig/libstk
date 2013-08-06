/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_list.h:  Chen Huaying chenhuaying@sina.cn
 *
 *  simple tool kit: double list.
 */

#ifndef __STK_LIST_H
#define __STK_LIST_H

#include <stddef.h>
#if 0
#include <stdlib.h>
#endif

typedef struct stk_list_s stk_list_t;

struct stk_list_s {
    struct stk_list_s *next;
    struct stk_list_s *prev;
};

/* static define list */
#define STK_LIST_INIT(name) { &(name), &(name) }
#define STK_LIST_HEAD(name) \
    stk_list_t name = STK_LIST_INIT(name)

static inline void stk_list_init(stk_list_t *list)
{
    list->next = list;
    list->prev = list;
}

static inline int stk_list_is_last(const stk_list_t *entry,
                                   const stk_list_t *head)
{
    return entry->prev == head;
}

static inline int stk_list_empty(const stk_list_t *head)
{
    return head->next == head;
}

/* add a new entry between prev and next */
static inline void __list_add(stk_list_t *new,
                              stk_list_t *prev,
                              stk_list_t *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/* add a new entry after the specified head */
static inline void stk_list_add(stk_list_t *new, stk_list_t *head)
{
    __list_add(new, head, head->next);
}

/* add a new entry at the end of the list */
static inline void stk_list_add_tail(stk_list_t *new, stk_list_t *head)
{
    __list_add(new, head->prev, head);
}

static inline void __stk_list_del(stk_list_t *prev, stk_list_t *next)
{
    prev->next = next;
    next->prev = prev;
}

/* remove a entry from the list */
static inline void stk_list_del(stk_list_t *entry)
{
    __stk_list_del(entry->prev, entry->next); 
#if 0
    entry->prev = NULL;
    entry->next = NULL;
#endif
}

static inline stk_list_t *stk_list_pop(stk_list_t *head)
{
    stk_list_t *entry = head->next;
    stk_list_del(entry);
    return entry;
}

#ifdef STK_DEBUG_OFFSET
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({          \
            const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
            (type *)( (char *)__mptr - offsetof(type,member) );})
#endif

/*
 * stk_list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 */
#define stk_list_entry(ptr, type, member)   \
    ((type *)( (unsigned char *)ptr - offsetof(type, member) ))

/*
 * stk_list_pop_entry - get the struct for this entry
 * @head:   the head for your list.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 */
#define stk_list_pop_entry(head, type, member)      \
({                                                  \
    type        *entry;                             \
    stk_list_t  *ptr;                               \
    if (stk_list_empty(head)) {                     \
        entry = NULL;                               \
    } else {                                        \
        ptr = stk_list_pop(head);                   \
        entry = stk_list_entry(ptr, type, member);  \
    }                                               \
    entry;                                          \
})

#define stk_list_first_entry(head, type, member)    \
    stk_list_entry((head)->next, type, member)      \

#define stk_list_peek_head(head, type, member)      \
({                                                  \
    type        *entry;                             \
    stk_list_t  *ptr;                               \
    if (stk_list_empty(head)) {                     \
        entry = NULL;                              \
    } else {                                        \
        ptr = (head)->next;                         \
        entry = stk_list_entry(ptr, type, member);  \
    }                                               \
    entry;                                          \
})

/*
 * stk_list_for_each    -   iterate over a list
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:   the head for your list.
 */
#define stk_list_for_each(pos, head)    \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/*
 * stk_list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define stk_list_for_each_entry(pos, head, member)  \
    for (pos = stk_list_entry((head)->next, typeof(*pos), member);  \
         &(pos->member) != (head);  \
         pos = stk_list_entry(pos->member.next, typeof(*pos), member))


#endif /*__STK_LIST_H*/
