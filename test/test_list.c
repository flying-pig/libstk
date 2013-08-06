/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_list.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: double list test.
 */

#include <stdlib.h>
#include "stk_list.h"

#include "stk_log.h"

struct my_data {
    int a;
    char b;
    stk_list_t list;
};

int main(int argc, char *argv[])
{
    stk_log_info("the test for stk_list.h");
    struct my_data data; /* the head of the list, just a head, no other use */
    stk_list_init(&(data.list));
    data.a = 200;
    data.b = 'w';
    stk_list_t *pos;
    /* there is no out here */
#if 0
    stk_list_for_each(pos, &(data.list)) {
        stk_log_debug("a = %d, b = %c", stk_list_entry(pos, struct my_data, list)->a,
                                        stk_list_entry(pos, struct my_data, list)->b);
    }
#endif

    struct my_data entry1 = {
        .a = 100,
        .b = 'a',
        .list = {NULL, NULL},
    };
    stk_list_add_tail(&(entry1.list), &(data.list));
#if 0
    stk_log_debug("entry1.a = %d, entry1.b = %c", entry1.a, entry1.b);
    stk_log_debug("entry1.a = %d, entry1.b = %c", stk_list_entry(&(entry1.list), struct my_data, list)->a,
                                                  stk_list_entry(&(entry1.list), struct my_data, list)->b);

    stk_log_debug("is list empty: %d", stk_list_empty(&(data.list)));
    stk_log_debug("data.list:%x", &(data.list));
    stk_log_debug("data.list.next:%x, entry1.list:%x", data.list.next, &(entry1.list));
    stk_log_debug("entry1.list.next:%x", entry1.list.next);
    stk_log_debug("a = %d, b = %c", stk_list_entry(&(data.list.next), struct my_data, list)->a,
                                    stk_list_entry(&(data.list.next), struct my_data, list)->b);
    stk_list_for_each(pos, &(data.list)) {
        stk_log_debug("a = %d, b = %c", stk_list_entry(pos, struct my_data, list)->a,
                                        stk_list_entry(pos, struct my_data, list)->b);
    }
#endif

    struct my_data entry2 = {
        .a = 200,
        .b = 'b',
    };
    struct my_data entry3 = {
        .a = 300,
        .b = 'c',
    };
    stk_list_add_tail(&entry2.list, &(data.list));
    struct my_data *p = &entry3;
    stk_list_add_tail(&p->list, &(data.list));
    stk_log_debug("is list empty: [%d]", stk_list_empty(&(data.list)));
    stk_list_for_each(pos, &(data.list)) {
        stk_log_debug("a = %d, b = %c", stk_list_entry(pos, struct my_data, list)->a,
                                        stk_list_entry(pos, struct my_data, list)->b);
    }
    stk_log_info("test list_for_each_entry");
    struct my_data *itr;
    stk_list_for_each_entry(itr, &(data.list), list) {
        stk_log_debug("a = %d, b = %c", itr->a, itr->b);
    }
    stk_log_info("test list_for_each_entry end!!!");
    stk_log_info("test delete entry");
    stk_list_del(&(entry2.list));
    stk_list_del(&(entry3.list));
    stk_list_del(&(entry1.list));
    stk_log_debug("is list empty: [%d]", stk_list_empty(&(data.list)));
    stk_list_for_each(pos, &(data.list)) {
        stk_log_debug("a = %d, b = %c", stk_list_entry(pos, struct my_data, list)->a,
                                        stk_list_entry(pos, struct my_data, list)->b);
    }
    stk_log_info("test list_pop_entry");
    stk_list_add(&entry1.list, &(data.list));
    stk_list_add(&entry2.list, &(data.list));
    stk_list_add(&entry3.list, &(data.list));
    stk_list_for_each(pos, &(data.list)) {
        stk_log_debug("a = %d, b = %c", stk_list_entry(pos, struct my_data, list)->a,
                                        stk_list_entry(pos, struct my_data, list)->b);
    }
    stk_log_info("first peek head.....");
    itr = stk_list_peek_head(&data.list, struct my_data, list);
    if (itr == NULL) {
        stk_log_debug("list empty, have no data");
    } else {
        stk_log_debug("head entry: a = %d, b = %c", itr->a, itr->b);
    }
    stk_log_info("list first entry.....");
    itr = stk_list_first_entry(&data.list, struct my_data, list);
    if (itr == NULL) {
        stk_log_debug("list empty, have no data");
    } else {
        stk_log_debug("first entry: a = %d, b = %c", itr->a, itr->b);
    }
    stk_log_info(">>>>");
    while (!stk_list_empty(&data.list)) {
        itr = stk_list_pop_entry(&data.list, struct my_data, list);
        stk_log_debug("a = %d, b = %c", itr->a, itr->b);
    }
    stk_log_debug("is list empty: [%d]", stk_list_empty(&(data.list)));
    return 0;
}
