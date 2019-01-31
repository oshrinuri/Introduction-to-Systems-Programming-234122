#include "../store.h"
#include "test_utilities.h"


static bool testStoreCreate() {
    StoreResult error;
    Store store = storeCreate(&error);
    ASSERT_TEST(store != NULL);
    ASSERT_TEST(error == STORE_SUCCESS);
    storeDestroy(store);
    return true;
}

static bool testStoreDestroy() {
    StoreResult error;
    Store store = storeCreate(&error);
    ASSERT_TEST(error == STORE_SUCCESS);
    storeDestroy(store);
    return true;
}

static bool testStoreCopy() {
    StoreResult error;
    Store store = storeCreate(&error);
    ASSERT_TEST(error == STORE_SUCCESS);
    Store store2 = storeCopy(store, &error);
    ASSERT_TEST(error == STORE_SUCCESS);
    ASSERT_TEST(store2 != NULL);
    storeDestroy(store);
    storeDestroy(store2);
    return true;
}

static bool testStoreGetCandies() {
    StoreResult error;
    Store store = storeCreate(&error);
    ASSERT_TEST(error == STORE_SUCCESS);
    storeAddItems(store, "candy", 1, 1);
    ASSERT_TEST(listGetSize(storeGetCandies(store)) == 1);
    storeDestroy(store);
    return true;
}

static bool testStoreGetPotions() {
    StoreResult error;
    Store store = storeCreate(&error);
    ASSERT_TEST(error == STORE_SUCCESS);
    storeAddItems(store, "potion", 1, 1);
    ASSERT_TEST(listGetSize(storeGetPotions(store)) == 1);
    storeDestroy(store);
    return true;
}

static bool testStoreAddItems() {
    StoreResult error;
    Store store = storeCreate(&error);
    ASSERT_TEST(error == STORE_SUCCESS);
    ASSERT_TEST(storeAddItems(store, "potn", 1, 1) == STORE_INVALID_ARG);
    ASSERT_TEST(storeAddItems(store, "potion", -1, 1) == STORE_INVALID_ARG);
    ASSERT_TEST(storeAddItems(store, "potion", 1, -1) == STORE_INVALID_ARG);
    ASSERT_TEST(storeAddItems(store, "potion", 1, 1) == STORE_SUCCESS);
    ASSERT_TEST(itemGetQuantity(listGetFirst(storeGetPotions(store))) == 1);
    ASSERT_TEST(itemGetValue(listGetFirst(storeGetPotions(store))) == 1);
    storeDestroy(store);
    return true;
}

static bool testStoreRemoveItem() {
    StoreResult error;
    Store store = storeCreate(&error);
    ASSERT_TEST(error == STORE_SUCCESS);
    storeAddItems(store, "potion", 1, 1);
    ASSERT_TEST(storeRemoveItem(store, "potn", 1) == STORE_INVALID_ARG);
    ASSERT_TEST(storeRemoveItem(store, "potion", -1) == STORE_INVALID_ARG);
    ASSERT_TEST(storeRemoveItem(store, "potion", 1) == STORE_SUCCESS);
    storeDestroy(store);
    return true;
}

static bool testitemCreate() {

    Item item = itemCreate(5, 5);
    ASSERT_TEST(item != NULL);
    ASSERT_TEST(itemGetQuantity(item) == 5);
    ASSERT_TEST(itemGetValue(item) == 5);
    itemDestroy(item);
    return true;
}

static bool testitemDestroy() {
    Item item = itemCreate(5, 5);
    itemDestroy(item);
    return true;
}

static bool testitemCopy() {

    Item item = itemCreate(5, 5);
    Item item2 = itemCopy(item);
    ASSERT_TEST(item2 != NULL);
    ASSERT_TEST(itemGetQuantity(item2) == 5);
    ASSERT_TEST(itemGetValue(item2) == 5);
    itemDestroy(item);
    itemDestroy(item2);
    return true;
}

static bool testitemGetValue() {

    Item item = itemCreate(5, 5);
    ASSERT_TEST(itemGetValue(item) == 5);
    itemDestroy(item);
    return true;
}

static bool testitemGetQuantity() {

    Item item = itemCreate(5, 5);
    ASSERT_TEST(itemGetQuantity(item) == 5);
    itemDestroy(item);
    return true;
}


int main() {
    RUN_TEST(testStoreCreate);
    RUN_TEST(testStoreDestroy);
    RUN_TEST(testStoreCopy);
    RUN_TEST(testStoreAddItems);
    RUN_TEST(testStoreRemoveItem);
    RUN_TEST(testStoreGetCandies);
    RUN_TEST(testStoreGetPotions);
    RUN_TEST(testitemCreate);
    RUN_TEST(testitemDestroy);
    RUN_TEST(testitemCopy);
    RUN_TEST(testitemGetValue);
    RUN_TEST(testitemGetQuantity);
    return 0;
}