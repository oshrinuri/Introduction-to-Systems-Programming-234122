#include "test_utilities.h"
#include <stdlib.h>
#include "../map_mtm/map_mtm.h"

/** Function to be used for copying an int as a key to the map */
static MapKeyElement copyKeyInt(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    int* copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int*)n;
    return copy;
}

/** Function to be used for copying a char as a data to the map */
static MapDataElement copyDataChar(MapDataElement n) {
    if (!n) {
        return NULL;
    }
    char* copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(char*)n;
    return (MapDataElement) copy;
}

/** Function to be used by the map for freeing elements */
static void freeInt(MapKeyElement n) {
    free(n);
}

/** Function to be used by the map for freeing elements */
static void freeChar(MapDataElement n) {
    free(n);
}

/** Function to be used by the map for comparing elements */
static int compareInts(MapKeyElement n1, MapKeyElement n2) {
    int* int_1 = (int*)n1;
    int* int_2 = (int*)n2;
    if (*int_1 > *int_2) {
        return 1;
    } else if (*int_2 > *int_1) {
        return -1;
    } else {
        return 0;
    }
}

bool testMapCreateDestroy() {
    Map map = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar, compareInts);
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapGetFirst(map) == NULL);
    mapDestroy(map);
    return true;

}

bool testMapAddAndSize() {
    Map map = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar, compareInts);
    for (int i=1; i<1000;++i){
        char j = (char) i;
        ++j;
        ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
        ASSERT_TEST(mapGetSize(map) == i);
    }
    mapDestroy(map);
    return true;
}

bool testMapGet() {
    Map map = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar, compareInts);
    for (int i=1; i<1000;++i){
        char j = (char) i;
        ++j;
        ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
        ASSERT_TEST(mapGetSize(map) == i);
    }

    for (int i=1; i<1000;++i){
        char j = (char) i;
        ++j;
        char* getVal = (char*)mapGet(map,&i);
        ASSERT_TEST(*getVal == j);
    }
    int i = 0;
    ASSERT_TEST(mapGet(map,&i) == NULL);
    i = 1000;
    ASSERT_TEST(mapGet(map,&i) == NULL);
    mapDestroy(map);
    return true;
}

bool testIterator() {
    Map map = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar, compareInts);
    for (int i = 1; i < 400; ++i) {
        char j = (char) i;
        ++j;
        ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
    }

    for (int i = 800; i >= 400; --i) {
        char j = (char) i;
        ++j;
        ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
    }

    for (int i = 801; i < 1000; ++i) {
        char j = (char) i;
        ++j;
        ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
    }

    int i = 1;
    MAP_FOREACH(MapKeyElement,iter,map) {
        ASSERT_TEST(*(int*)iter == i++);
    }

    mapDestroy(map);
    return true;
}

int main() {
    RUN_TEST(testMapCreateDestroy);
    RUN_TEST(testMapAddAndSize);
    RUN_TEST(testMapGet);
    RUN_TEST(testIterator);
    return 0;
}
