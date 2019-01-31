#ifndef STORE_H_
#define STORE_H_

#include "list.h"

typedef struct store_t* Store;
typedef struct item_t* Item;

typedef enum {
	STORE_SUCCESS,
	STORE_NULL_ARG,
	STORE_OUT_OF_MEM,
	STORE_INVALID_ARG,
	STORE_OUT_OF_STOCK

} StoreResult;

/*----------------------------------------------------------------------------*/

/**
* Creates a new store.
*
* @param none
* @return
* NULL if memory allocation failed.
* new allocated store otherwise
*/
Store storeCreate(StoreResult* error);

/*----------------------------------------------------------------------------*/
/**
* Creates a copy of store.
*
* @param store to copy
* @return
* NULL if memory allocation failed.
* new allocated store copy otherwise
*/
Store storeCopy(Store store, StoreResult* error);

/*----------------------------------------------------------------------------*/
/**
* Frees all memory allocated to store
*
* @param store
*/
void storeDestroy(Store store);

/*----------------------------------------------------------------------------*/
/**
* Adds items to the store
*
* @param store, name of item, value of item, quantity to add
* @return result
*/
StoreResult storeAddItems(Store store, char* name, int value, int quantity);

/*----------------------------------------------------------------------------*/
/**
* Removes an item from the store
*
* @param store, name of item, value of item
* @return result
*/
StoreResult storeRemoveItem(Store store, char* name, int value);

/*----------------------------------------------------------------------------*/
/**
* Creates a new item.
*
* @param value and quantity
* @return
* NULL if memory allocation failed or NULL sent.
* new allocated item otherwise
*/
Item itemCreate(int value, int quantity);

/*----------------------------------------------------------------------------*/
/**
* Creates a copy of item.
*
* @param item
* @return
* NULL if memory allocation failed or NULL sent.
* new allocated item copy otherwise
*/
Item itemCopy(Item item);

/*----------------------------------------------------------------------------*/
/**
 * Returns candies list of a given Store.
*/
List storeGetCandies (Store store);

/*----------------------------------------------------------------------------*/
/**
 * Returns potions list of a given Store.
*/
List storeGetPotions (Store store);

/*----------------------------------------------------------------------------*/
/**
* Frees all memory allocated to item
*
* @param item
*/
void itemDestroy(Item item);

/*----------------------------------------------------------------------------*/
/**
* Returns the value of an item
*
* @param item
*/
int itemGetValue(Item item);

/*----------------------------------------------------------------------------*/
/**
* Returns the quantity of an item
*
* @param item
*/
int itemGetQuantity(Item item);

/*----------------------------------------------------------------------------*/
#endif /* STORE_H */