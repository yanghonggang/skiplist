/*
* Copyright (C) 2015, Leo Ma <begeekmyfriend@gmail.com>
*/
#include <stdio.h>
#include <stdlib.h>
#if defined(__MACH__) && !defined(CLOCK_REALTIME)
#include <sys/time.h>

#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1

int clock_gettime(int clk_id, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#else
#include <time.h>
#endif

#include "skiplist.h"

#define N 16 // 2 * 1024 * 1024
#define SKIPLIST_DEBUG

int
main(void)
{
    int i;
    struct timespec start, end;

    int *key = malloc(N * sizeof(int));
    if (key == NULL) {
        exit(-1);
    }

    struct skiplist *list = skiplist_new();
    if (list == NULL) {
        exit(-1);
    }

    printf("Test start!\n");
    printf("Add %d nodes...\n", N);

    /* Insert test */
    srandom(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < N; i++) {
        int value = key[i] = (int)random() % 20;
        skiplist_insert(list, key[i], value);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("time span: %ldms\n", (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000);
    #ifdef SKIPLIST_DEBUG
    skiplist_dump(list);
    #endif

    /* Search test */
    printf("Now search each node...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < N; i++) {
        struct skipnode *node = skiplist_search(list, key[i]);
        if (node != NULL) {
            #ifdef SKIPLIST_DEBUG
            printf("%d ", node->key);
            #endif
        } else {
            printf("Not found:%d\n", key[i]);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("\ntime span: %ldms\n", (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000);

    /* Delete test */
    printf("Now remove all nodes...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < N; i++) {
        skiplist_remove(list, key[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("time span: %ldms\n", (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000);
    #ifdef SKIPLIST_DEBUG
    skiplist_dump(list);
    #endif

    printf("End of Test.\n");
    skiplist_delete(list);

    free(key);

    return 0;
}
