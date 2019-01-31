#include "map_dry.h"
#include <stdlib.h>
#include <stdio.h>

/**
* Generic Ordered Map Container
*
* Implements an ordered map container type.
* The map has an internal iterator for external use. For all functions
* where the state of the iterator after calling that function is not stated,
* it is undefined. That is you cannot assume anything about it.
*
* The map is ordered, in the sense that the iteration over the elements is done
* according to the key compare function from smallest to largest. Where
*
* keyCompare(key a, key b) ==  0     ======>     a = b
* keyCompare(key a, key b) ==  1     ======>     a > b
* keyCompare(key a, key b) == -1     ======>     a < b
*
* The following functions are available:
*   mapCreate		- Creates a new empty map.
*   mapDestroy	- Deletes an existing map and frees all resources.
*   mapCopy	  	- Copies an existing map.
*   mapGetSize  - Returns the size of a given map.
*   mapContains	- Returns weather or not a key exists inside the map.
*   				      This resets the internal iterator.
*   mapPut      - Gives a specific key a given value.
*   				      If the key exists, the value is overridden.
*   				      This resets the internal iterator.
*   mapGet      - Returns the data paired to a key which matches the given key.
*					        Iterator status unchanged.
*   mapRemove		- Removes a pair of (key,data) elements for which the key
*                 matches a given element (by the key compare function).
*   				      This resets the internal iterator.
*   mapGetFirst	- Sets the internal iterator to the first key in the
*   				      map, and returns it.
*   mapGetNext  - Advances the internal iterator to the next key and
*   				      returns it.
*	  mapClear	 	- Clears the contents of the map. Frees all the elements of
*	 				        the map using the free function.
*   MAP_FOREACH - A macro for iterating over the map's elements.
*/

typedef struct node {
    MapKeyElement key;
    MapDataElement data;
    struct node* next;
} *Node;

struct Map_t {
    int map_size;
    Node head;
    Node iterator;
    copyMapKeyElements copyKey;
    freeMapKeyElements freeKey;
    copyMapDataElements copyData;
    freeMapDataElements freeData;
    compareMapKeyElements compareKeys;
};

void mapPrint(Map map) {
    Node iterator = map->head;
    while (iterator != NULL) {
        printf("%d -> ",(int)iterator->key);
        iterator = iterator->next;
    }
}

/**
* mapCreate: Allocates a new empty map.
*
* @param copyDataElement - Function pointer to be used for copying data elements into
*  	the map or when copying the map.
* @param copyKeyElement - Function pointer to be used for copying key elements into
*  	the map or when copying the map.
* @param freeDataElement - Function pointer to be used for removing data elements from
* 		the map
* @param freeKeyElement - Function pointer to be used for removing key elements from
* 		the map
* @param compareKeyElements - Function pointer to be used for comparing key elements
* 		inside the map. Used to check if new elements already exist in the map.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Map in case of success.
*/
Map mapCreate(copyMapKeyElements copyKeyElement,
              copyMapDataElements copyDataElement,
              freeMapKeyElements freeKeyElement,
              freeMapDataElements freeDataElement,
              compareMapKeyElements compareKeyElements) {
    if (!copyKeyElement || !copyDataElement || !freeKeyElement ||
        !freeKeyElement || !compareKeyElements) {
        return NULL;
    }
    Map new_map = malloc(sizeof(*new_map));
    if (!new_map) {
        return NULL;
    }
    new_map->map_size = 0;
    new_map->head = NULL;
    new_map->iterator = NULL;
    new_map->copyKey = copyKeyElement;
    new_map->freeKey = freeKeyElement;
    new_map->copyData = copyDataElement;
    new_map->freeData = freeDataElement;
    new_map->compareKeys = compareKeyElements;
    return new_map;
}

static Node nodeCreate(MapKeyElement key, MapDataElement data) {
    Node new_node = malloc(sizeof(*new_node));
    if (!new_node) {
        return NULL;
    }
    new_node->data = data;
    new_node->key = key;
    new_node->next = NULL;
    return new_node;
}

static void nodeDestroy(Map map, Node node) {
    if (!node) {
        return;
    }
    map->freeData(node->data);
    map->freeKey(node->key);
    free(node);
}

/**
* mapDestroy: Deallocates an existing map. Clears all elements by using the
* stored free functions.
*
* @param map - Target map to be deallocated. If map is NULL nothing will be
* 		done
*/
void mapDestroy(Map map) {
    if (!map) {
        return;
    }
    mapClear(map);
    free(map);
}

/**
* mapCopy: Creates a copy of target map.
* Iterator values for both maps is undefined after this operation.
*
* @param map - Target map.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Map containing the same elements as map otherwise.
*/
Map mapCopy(Map map) {
    if (!map) {
        return NULL;
    }
    Map copy = mapCreate(map->copyKey,map->copyData,map->freeKey,map->freeData,
                         map->compareKeys);
    if (!copy) {
        return NULL;
    }
    Node temp_node = map->iterator;
    MAP_FOREACH (Node,node_iterator,map) {
        if (mapPut(copy,node_iterator->key,node_iterator->data) != MAP_SUCCESS) {
            mapDestroy(copy);
            return NULL;
        }
    }
    map->iterator = temp_node;
    copy->iterator = temp_node;
    return copy;
}

/**
* mapGetSize: Returns the number of elements in a map
* @param map - The map which size is requested
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the number of elements in the map.
*/
int mapGetSize(Map map) {
    if (!map) {
        return -1;
    }
    return map->map_size;
}

/**
* mapContains: Checks if a key element exists in the map. The key element will be
* considered in the map if one of the key elements in the map it determined equal
* using the comparison function used to initialize the map.
*
* @param map - The map to search in
* @param element - The element to look for. Will be compared using the
* 		comparison function.
* @return
* 	false - if one or more of the inputs is null, or if the key element was not found.
* 	true - if the key element was found in the map.
*/
bool mapContains(Map map, MapKeyElement element) {
    MAP_FOREACH (Node,node_iterator,map) {
        if (map->compareKeys(element,node_iterator->key) == 0) {
            return true;
        }
    }
    return false;
}

/**
*	mapPut: Gives a specified key a specific value.
*  Iterator's value is undefined after this operation.
*
* @param map - The map for which to reassign the data element
* @param keyElement - The key element which need to be reassigned
* @param dataElement - The new data element to associate with the given key.
*      A copy of the element will be inserted as supplied by the copying function
*      which is given at initialization and old data memory would be
*      deleted using the free function given at initialization.
* @return
* 	MAP_NULL_ARGUMENT if a NULL was sent as map, key, or data
* 	MAP_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
* 	MAP_SUCCESS the paired elements had been inserted successfully
*/
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    if (!map || !keyElement || !dataElement) {
        return MAP_NULL_ARGUMENT;
    }
    if (!map->head) {
        map->head = nodeCreate(map->copyKey(keyElement),map->copyData(dataElement));
        if ( !map->head ){
            return MAP_OUT_OF_MEMORY;
        }
        map->iterator = map->head; 
        map->map_size++;
        return MAP_SUCCESS;
    }
    map->iterator = map->head;
    Node prev = map->head;
    while (map->iterator != NULL) {
        if (map->compareKeys(map->iterator->next, keyElement) == 0) {
            map->freeData(map->iterator->data);
            map->iterator->data = map->copyData(dataElement);
            if (!map->iterator->data) {
                return MAP_OUT_OF_MEMORY;
            }
        } else if (map->compareKeys(map->iterator->next->key,keyElement) > 0 &&
                   map->compareKeys(keyElement,map->iterator->key) > 0) {
            Node new_node = nodeCreate(map->copyKey(keyElement),map->copyData(dataElement));
            if ( !new_node ){
                return MAP_OUT_OF_MEMORY;
            }
            new_node->next = map->iterator->next;
            map->iterator->next = new_node; 
            map->map_size++;
            return MAP_SUCCESS;
        }
        map->iterator = map->iterator->next; 
        prev = prev->next;
    }
    map->map_size++;
    return MAP_SUCCESS;
}

/**
*	mapGet: Returns the data associated with a specific key in the map.
*			Iterator status unchanged
*
* @param map - The map for which to get the data element from.
* @param keyElement - The key element which need to be found and whos data
we want to get.
* @return
*  NULL if a NULL pointer was sent or if the map does not contain the requested key.
* 	The data element associated with the key otherwise.
*/
MapDataElement mapGet(Map map, MapKeyElement keyElement) {
    if (!map || !keyElement) {
        return NULL;
    }
    map->iterator = map->head;
    while (map->iterator != NULL) {
        if (map->compareKeys(map->iterator->key, keyElement) == 0) {
            return map->iterator->data;
        }
        map->iterator = map->iterator->next;
    }
    return NULL;
}

/**
* 	mapRemove: Removes a pair of key and data elements from the map. The elements
*  are found using the comparison function given at initialization. Once found,
*  the elements are removed and deallocated using the free functions
*  supplied at initialization.
*  Iterator's value is undefined after this operation.
*
* @param map -
* 	The map to remove the elements from.
* @param keyElement
* 	The key element to find and remove from the map. The element will be freed using the
* 	free function given at initialization. The data element associated with this key
*  will also be freed using the free function given at initialization.
* @return
* 	MAP_NULL_ARGUMENT if a NULL was sent to the function (either map or key)
*  MAP_ITEM_DOES_NOT_EXIST if an equal key item does not already exists in the map
* 	MAP_SUCCESS the paired elements had been removed successfully
*/
MapResult mapRemove(Map map, MapKeyElement keyElement) {
    if (!map || !keyElement) {
        return MAP_NULL_ARGUMENT;
    }
    if (mapGetSize(map) == 1) {
        nodeDestroy(map,mapGetFirst(map));
        map->iterator = NULL;
        return MAP_SUCCESS;
    }
    Node prev = mapGetFirst(map);
    MAP_FOREACH (Node,node_iterator,map) {
        if (map->compareKeys(keyElement,node_iterator->key) == 0) {
            nodeDestroy(map,node_iterator);
            prev->next = node_iterator->next;
            return MAP_SUCCESS;
        }
        prev = node_iterator;
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

/**
*	mapGetFirst: Sets the internal iterator (also called current key element) to
*	the first key element in the map.
*	Use this to start iterating over the map.
*	To continue iteration use mapGetNext
*
* @param map - The map for which to set the iterator and return the first
* 		key element.
* @return
* 	NULL if a NULL pointer was sent or the map is empty.
* 	The first key element of the map otherwise
*/
MapKeyElement mapGetFirst(Map map) {
    if (!map){
        return NULL;
    }
    map->iterator=map->head;
    return map->head;
}

/**
*	mapGetNext: Advances the map iterator to the next key element and returns it.
* @param map - The map for which to advance the iterator
* @return
* 	NULL if reached the end of the map, or the iterator is at an invalid state
* 	or a NULL sent as argument
* 	The next key element on the map in case of success
*/
MapKeyElement mapGetNext(Map map) {
    if (!map || !map->iterator) {
        return NULL;
    }
    map->iterator = map->iterator->next;
    return map->iterator->key;
}


/**
* mapClear: Removes all key and data elements from target map.
* The elements are deallocated using the stored free functions.
* @param map
* 	Target map to remove all element from.
* @return
* 	MAP_NULL_ARGUMENT - if a NULL pointer was sent.
* 	MAP_SUCCESS - Otherwise.
*/
MapResult mapClear(Map map) {
//    if (!map) {
//        return MAP_NULL_ARGUMENT;
//    }
//    if (mapGetSize(map) == 1) {
//        destroyNode(map,mapGetFirst(map));
//        return MAP_SUCCESS;
//    } else {
//        mapGetFirst(map);
//        Node current = map->head;
//        while (map)
//    }
    return MAP_SUCCESS;
}