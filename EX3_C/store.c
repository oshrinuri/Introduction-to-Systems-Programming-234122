#include "store.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

//-------------------------- Defines & Declarations ----------------------------

#define LIST_CANDY 0
#define LIST_POTION 1

struct store_t {
    List items[2];
};

struct item_t {
    int value;
    int quantity;
};

//-------------------------- Auxiliary Functions -------------------------------

/**
* Copies an item
* to pass to List
* @param item
* @return an item element for list
*/
static ListElement itemListCopy(ListElement item) {
    Item copy = itemCopy(item);
    if (!copy) {
        return NULL;
    }
    return copy;
}
/*----------------------------------------------------------------------------*/
/**
* Frees memory of item in list
* to pass to List
* @param item
*/
void itemListDestroy(ListElement item) {
    itemDestroy((Item) item);
}

//-------------------------- Main Functions ------------------------------------

List storeGetCandies(Store store) {
    if (!store) {
        return NULL;
    }
    return store->items[LIST_CANDY];
}
/*----------------------------------------------------------------------------*/
List storeGetPotions(Store store) {
    if (!store) {
        return NULL;
    }
    return store->items[LIST_POTION];
}
/*----------------------------------------------------------------------------*/
Store storeCreate(StoreResult* error) {
    *error = STORE_OUT_OF_MEM;
    Store store = malloc(sizeof(*store));
    if (!store) {
        return NULL;
    }
    List potions = listCreate(itemListCopy, itemListDestroy);
    if (!potions) {
        storeDestroy(store);
        return NULL;
    }
    List candies = listCreate(itemListCopy, itemListDestroy);
    if (!candies) {
        storeDestroy(store);
        return NULL;
    }
    store->items[LIST_CANDY] = candies;
    store->items[LIST_POTION] = potions;
    *error = STORE_SUCCESS;
    return store;
}
/*----------------------------------------------------------------------------*/
Store storeCopy(Store store, StoreResult* error) {
    *error = STORE_OUT_OF_MEM;
    if (!store) {
        return NULL;
    }
    Store copy = storeCreate(error);
    if (*error = STORE_OUT_OF_MEM) {
        return NULL;
    }
    *error = STORE_SUCCESS;
    return copy;
}
/*----------------------------------------------------------------------------*/
void storeDestroy(Store store) {
    if (!store) {
        return;
    }
    listDestroy(store->items[LIST_CANDY]); // Destroy list of candies
    listDestroy(store->items[LIST_POTION]); // Destroy list of potions
    free(store);
}
/*----------------------------------------------------------------------------*/
StoreResult storeAddItems(Store store, char *name, int value, int quantity) {
    if (!store || !name) {
        return STORE_NULL_ARG;
    }
    if (value < 0 || quantity < 0 ||
        (strcmp(name, "candy") != 0 && strcmp(name, "potion") != 0)) {
        return STORE_INVALID_ARG;
    }
    int list_type = strcmp(name, "candy") == 0 ? LIST_CANDY : LIST_POTION;
    LIST_FOREACH(Item, item_iterator, store->items[list_type]) {
        if (item_iterator->value == value) {
            item_iterator->quantity += quantity;
            return STORE_SUCCESS;
        }
    }
    listInsertLast(store->items[list_type], itemCreate(value, quantity));
    return STORE_SUCCESS;
}
/*----------------------------------------------------------------------------*/
StoreResult storeRemoveItem(Store store, char *name, int value) {
    if (!store || !name) {
        return STORE_NULL_ARG;
    }
    if (value < 0 ||
        (strcmp(name, "candy") != 0 && strcmp(name, "potion") != 0)) {
        return STORE_INVALID_ARG;
    }
    int list_type = strcmp(name, "candy") == 0 ? LIST_CANDY : LIST_POTION;
    LIST_FOREACH(Item, item_iterator, store->items[list_type]) {
        if (item_iterator->value == value) {
            item_iterator->quantity--;
            if (item_iterator->quantity == 0) {
                listRemoveCurrent(store->items[list_type]);
            }
            return STORE_SUCCESS;
        }
    }
    return STORE_OUT_OF_STOCK;
}
/*----------------------------------------------------------------------------*/
Item itemCreate(int value, int quantity) {
    if (value < 1 || quantity < 1) {
        return NULL;
    }
    Item item = malloc(sizeof(*item));
    if (!item) {
        return NULL;
    }
    item->quantity = quantity;
    item->value = value;
    return item;
}
/*----------------------------------------------------------------------------*/
Item itemCopy(Item item) {
    if (!item) {
        return NULL;
    }
    Item copy = itemCreate(item->value, item->quantity);
    if (!copy) {
        return NULL;
    }
    return copy;
}
/*----------------------------------------------------------------------------*/
void itemDestroy(Item item) {
    assert(item!=NULL);
    free(item);
}
/*----------------------------------------------------------------------------*/
int itemGetValue(Item item) {
    assert(item!=NULL);
    return item->value;
}
/*----------------------------------------------------------------------------*/
int itemGetQuantity(Item item) {
    assert(item!=NULL);
    return item->quantity;
}
/*----------------------------------------------------------------------------*/