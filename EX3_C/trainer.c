#include "trainer.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//-------------------------- Defines & Declarations ----------------------------

#define START_EXP 1.0
#define CANDY "candy"
#define POTION "potion"

struct trainer_t {
    char *name;
    char *current_location;
    Store trainer_items;
    Map pokemons;
    double experience;
    int pokecoins;
    int number_of_pokemons;
};

//-------------------------- Auxiliary Functions -------------------------------

/**
* Copies a pokemon map data element
* to pass to Map
* @param pokemon
* @return a data element for Map
*/
static MapDataElement pokemonMapCopy(MapDataElement pokemon) {
    if (!pokemon) {
        return NULL;
    }
    PokemonResult error = POKEMON_SUCCESS;
    return pokemonCopy((Pokemon) pokemon, &error);
}
/*----------------------------------------------------------------------------*/
/**
* Frees memory of pokemon in Map
* to pass to Map
* @param pokemon
*/
static void pokemonMapDestroy(MapDataElement pokemon) {
    if (!pokemon) {
        return;
    }
    pokemonDestroy((Pokemon) pokemon);
}
/*----------------------------------------------------------------------------*/
/**
* Copies an integer map data element
* to pass to Map
* @param index
* @return a key element for Map
*/
MapKeyElement copyIdx(MapKeyElement idx) {
    if (!idx) {
        return NULL;
    }
    int *temp = malloc(sizeof(*temp));
    if (!temp) {
        return NULL;
    }
    *temp = *(int *) idx;
    return temp;
}
/*----------------------------------------------------------------------------*/
/**
* Frees memory of integer in Map
* to pass to Map
* @param index
*/
void freeIdx(MapKeyElement idx) {
    free(idx);
}
/*----------------------------------------------------------------------------*/
/**
* An integer compare function to pass to Map.
* @param index1, index2
* @return a positive value if arg2 is bigger than arg1,
*         0 if equal, negative value if arg1 is bigger than arg2.
*/
int compareIdx(MapKeyElement idx1, MapKeyElement idx2) {
    if (*(int *) idx1 > *(int *) idx2) {
        return 1;
    } else if (*(int *) idx1 < *(int *) idx2) {
        return -1;
    } else {
        return 0;
    }
}
/*----------------------------------------------------------------------------*/
/**
* An -item value- compare function to pass to List. (for ascending order sort)
* @param item1, item2
* @return a positive value if arg1 is bigger than arg2,
*         0 if equal, negative value if arg2 is bigger than arg1.
*/
static int itemSortByAscendingValue(ListElement item1, ListElement item2) {
    int value1 = itemGetValue((Item) item1);
    int value2 = itemGetValue((Item) item2);
    if (value2 > value1) {
        return -1;
    } else if (value2 < value1) {
        return 1;
    } else {
        return 0;
    }
}
/*----------------------------------------------------------------------------*/
/**
* An -item value- compare function to pass to List. (for descending order sort)
* @param item1, item2
* @return a positive value if arg2 is bigger than arg1,
*         0 if equal, negative value if arg1 is bigger than arg2.
*/
static int itemSortByDescendingValue(ListElement item1, ListElement item2) {
    int value1 = itemGetValue((Item) item1);
    int value2 = itemGetValue((Item) item2);
    if (value2 > value1) {
        return 1;
    } else if (value2 < value1) {
        return -1;
    } else {
        return 0;
    }
}

//-------------------------- Main Functions ------------------------------------

Trainer trainerCreate(char *name, int budget, char *starting_location,
                      TrainerResult* result) {
    *result = TRAINER_OUT_OF_MEM;
    if (!name || !starting_location) {
        *result = TRAINER_NULL_ARG;
        return NULL;
    }
    if (budget < 0) {
        *result = TRAINER_INVALID_ARG;
        return NULL;
    }
    Trainer trainer = malloc(sizeof(*trainer));
    if (!trainer) return NULL;
    trainer->name = malloc(strlen(name) + 1);
    strcpy(trainer->name, name);
    trainer->number_of_pokemons = 0;
    trainer->pokecoins = budget;
    trainer->experience = START_EXP;
    trainer->current_location = malloc(strlen(starting_location) + 1);
    if (!trainer->current_location) {
        trainerDestroy(trainer);
        return NULL;
    }
    strcpy(trainer->current_location, starting_location);
    StoreResult error;
    Store store = storeCreate(&error);
    if (error == STORE_OUT_OF_MEM) {
        trainerDestroy(trainer);
        return NULL;
    }
    trainer->trainer_items = store;
    Map pokemons = mapCreate(copyIdx, pokemonMapCopy, freeIdx,
                             pokemonMapDestroy, compareIdx);
    if (!pokemons) {
        free(trainer->name);
        free(trainer->current_location);
        storeDestroy(store);
        free(trainer);
        return NULL;
    }
    trainer->pokemons = pokemons;
    *result = TRAINER_SUCCESS;
    return trainer;
}
/*----------------------------------------------------------------------------*/
Trainer trainerCopy(Trainer trainer, TrainerResult* error) {
    if (!trainer) {
        *error = TRAINER_NULL_ARG;
        return NULL;
    }
    Trainer copy = trainerCreate(trainer->name, trainer->pokecoins,
                                 trainer->current_location, error);
    if (!copy) {
        *error = TRAINER_OUT_OF_MEM;
        return NULL;
    }
    *error = TRAINER_SUCCESS;
    return copy;
}
/*----------------------------------------------------------------------------*/
void trainerDestroy(Trainer trainer) {
    if (!trainer) {
        return;
    }
    free(trainer->name);
    free(trainer->current_location);
    storeDestroy(trainer->trainer_items);
    mapDestroy(trainer->pokemons);
    free(trainer);
}
/*----------------------------------------------------------------------------*/
Pokemon trainerGetPokemonByID(Trainer trainer, int id) {
    if (!trainer || id < 1 || id > trainer->number_of_pokemons ||
        !mapContains(trainer->pokemons, &id)) {
        return NULL;
    }
    return mapGet(trainer->pokemons, &id);
}
/*----------------------------------------------------------------------------*/
TrainerResult trainerAddPokemon(Trainer trainer, Pokemon pokemon) {
    if (!trainer || !pokemon) {
        return TRAINER_NULL_ARG;
    }
    trainer->number_of_pokemons++;
    if (mapPut(trainer->pokemons, &(trainer->number_of_pokemons), pokemon) !=
        MAP_SUCCESS) {
        trainer->number_of_pokemons--;
        return TRAINER_OUT_OF_MEM;
    }
    return TRAINER_SUCCESS;
}
/*----------------------------------------------------------------------------*/
TrainerResult trainerRemovePokemon(Trainer trainer, int id) {
    if (!trainer) {
        return TRAINER_NULL_ARG;
    }
    if (id < 1) {
        return TRAINER_INVALID_ARG;
    }
    if (!mapContains(trainer->pokemons, &id)) {
        return TRAINER_POKEMON_DOES_NOT_EXIST;
    }
    mapRemove(trainer->pokemons, &id);
    return TRAINER_SUCCESS;
}
/*----------------------------------------------------------------------------*/
TrainerResult trainerPokemonUsePotion(Trainer trainer, int id) {
    if (!trainer) return TRAINER_DOES_NOT_EXIST;
    Pokemon pokemon = trainerGetPokemonByID(trainer, id);
    if (!pokemon) return TRAINER_POKEMON_DOES_NOT_EXIST;
    if (pokemonGetHp(pokemon) == MAX_HP) return TRAINER_POKEMON_HP_IS_AT_MAX;
    List potions = storeGetPotions(trainer->trainer_items);
    int list_size = listGetSize(potions);
    if (list_size == 0) return TRAINER_NO_AVAILABLE_ITEM_FOUND;
    List sorted_potions = listCopy(potions);
    listSort(sorted_potions, itemSortByAscendingValue);
    int current_potion_num = 0;
    LIST_FOREACH(Item, potion_iterator, sorted_potions) {
        current_potion_num++;
        int current_potion_value = itemGetValue(potion_iterator);
        if (pokemonGetHp(pokemon) + current_potion_value < MAX_HP &&
            current_potion_num != list_size) {
            continue;
        }
        pokemonSetHp(pokemon, pokemonGetHp(pokemon) + current_potion_value);
        storeRemoveItem(trainer->trainer_items, POTION, current_potion_value);
        break;
    }
    listDestroy(sorted_potions);
    return TRAINER_SUCCESS;
}
/*----------------------------------------------------------------------------*/
TrainerResult trainerPokemonUseCandy(Trainer trainer, int id) {
    if (!trainer) {
        return TRAINER_NULL_ARG;
    }
    if (id < 1) {
        return TRAINER_INVALID_ARG;
    }
    Pokemon pokemon = trainerGetPokemonByID(trainer, id);
    if (!pokemon) {
        return TRAINER_POKEMON_DOES_NOT_EXIST;
    }
    List candies = storeGetCandies(trainer->trainer_items);
    if (listGetSize(candies) == 0) {
        return TRAINER_NO_AVAILABLE_ITEM_FOUND;
    }
    List sorted_candies = listCopy(candies);
    listSort(sorted_candies, itemSortByDescendingValue);
    int max_value = itemGetValue(listGetFirst(sorted_candies));
    StoreResult rem_result = storeRemoveItem(trainer->trainer_items, CANDY,
                                             max_value);
    if (rem_result == STORE_NULL_ARG) return TRAINER_NULL_ARG;
    if (rem_result == STORE_INVALID_ARG) return TRAINER_INVALID_ARG;
    pokemonAddCp(pokemon, max_value);
    pokemonAddBonusCp(pokemon, max_value);
    return TRAINER_SUCCESS;
}
/*----------------------------------------------------------------------------*/
char *trainerGetName(Trainer trainer) {
    assert(trainer!=NULL);
    return trainer->name;
}
/*----------------------------------------------------------------------------*/
char *trainerGetLocation(Trainer trainer) {
    assert(trainer!=NULL);
    return trainer->current_location;
}
/*----------------------------------------------------------------------------*/
void trainerSetLocation(Trainer trainer, char *new_location) {
    assert(trainer!=NULL);
    if (!trainer || !new_location || strlen(new_location) == 0) {
        return;
    }
    free(trainer->current_location);
    trainer->current_location = malloc(strlen(new_location) + 1);
    strcpy(trainer->current_location, new_location);
}
/*----------------------------------------------------------------------------*/
double trainerGetExp(Trainer trainer) {
    assert(trainer!=NULL);
    return trainer->experience;
}
/*----------------------------------------------------------------------------*/
int trainerGetPokecoins(Trainer trainer) {
    assert(trainer!=NULL);
    return trainer->pokecoins;
}
/*----------------------------------------------------------------------------*/
int trainerGetNumOfPokemons(Trainer trainer) {
    assert(trainer!=NULL);
    return trainer->number_of_pokemons;
}
/*----------------------------------------------------------------------------*/
void trainerSetExp(Trainer trainer, double exp) {
    assert(trainer!=NULL);
    if (exp < 0) {
        return;
    }
    trainer->experience = exp;
}
/*----------------------------------------------------------------------------*/
bool trainerPokemonExist(Trainer trainer, int id) {
    assert(trainer!=NULL);
    if (id < 1 || id > trainer->number_of_pokemons ||
        mapGet(trainer->pokemons, &id) == NULL) {
        return false;
    }
    return true;
}
/*----------------------------------------------------------------------------*/
Store trainerGetItems(Trainer trainer) {
    assert(trainer!=NULL);
    return trainer->trainer_items;
}
/*----------------------------------------------------------------------------*/
Map trainerGetPokemons(Trainer trainer) {
    assert(trainer!=NULL);
    return trainer->pokemons;
}
/*----------------------------------------------------------------------------*/
void trainerSetPokecoins(Trainer trainer, int pokecoins) {
    assert(trainer!=NULL);
    if (pokecoins < 0) {
        return;
    }
    trainer->pokecoins = pokecoins;
}
/*----------------------------------------------------------------------------*/