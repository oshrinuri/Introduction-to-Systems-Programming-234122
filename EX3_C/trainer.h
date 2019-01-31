#ifndef TRAINER_H_
#define TRAINER_H_

#include "location.h"
#include "store.h"
#include "map.h"
#include "list.h"
#include "pokemon.h"
#include <stdio.h>

typedef struct trainer_t *Trainer;

typedef enum {
    TRAINER_SUCCESS,
    TRAINER_NULL_ARG,
    TRAINER_DOES_NOT_EXIST,
    TRAINER_INVALID_ARG,
    TRAINER_OUT_OF_MEM,
    TRAINER_EMPTY_LOCATION,
    TRAINER_OUT_OF_STOCK,
    TRAINER_BUDGET_IS_NOT_SUFFICIENT,
    TRAINER_ILLEGAL_MOVE,
    TRAINER_ALREADY_IN_LOCATION,
    TRAINER_POKEMON_DOES_NOT_EXIST,
    TRAINER_POKEMON_HP_IS_AT_MAX,
    TRAINER_NO_AVAILABLE_ITEM_FOUND
} TrainerResult;

/*----------------------------------------------------------------------------*/

/**
* Creates a new trainer.
*
* @param name, budget, starting location.
* @return
* A newly allocated Trainer.
* In case of invalid input or any kind of memory allocation failure -
* return NULL and a specific error result (via a pointer).
*/
Trainer trainerCreate(char *name, int budget, char *starting_location,
                      TrainerResult* result);

/*----------------------------------------------------------------------------*/
/**
* Creates a copy of trainer
*
* @param trainer - Trainer to copy
* @param error - error code
* @return
* In case of invalid input or any kind of memory allocation failure -
* return NULL and a specific error result (via a pointer).
*/
Trainer trainerCopy(Trainer trainer, TrainerResult* error);

/*----------------------------------------------------------------------------*/
/**
* Frees all memory allocated to trainer
*
* @param trainer
*/
void trainerDestroy(Trainer trainer);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemon in trainers pokemon map by given id.
* @return NULL if no pokemonn or invalid id.
* @param trainer, id.
*/
Pokemon trainerGetPokemonByID(Trainer trainer, int id);

/*----------------------------------------------------------------------------*/
/**
* Chooses highest value candy to use on given pokemon by id,
* trainer loses candy
* pokemon cp set higher
* @param trainer, id of pokemon to train
* @return:
*   TRAINER_POKEMON_DOES_NOT_EXIST if id is NULL.
*   TRAINER_NO_AVAILABLE_ITEM_FOUND if trainer has no candies.
*   TRAINER_SUCCESS if used candy.
*/
TrainerResult trainerPokemonUseCandy(Trainer trainer, int id);

/*----------------------------------------------------------------------------*/
/**
* Chooses best potion to use on given pokemon by id,
* based on the lowest value potion that will heal to 100 hp,
* or highest value if this doesn't exist.
* trainer loses potion
* pokemon hp set higher
* @param trainer, id of pokemon to heal
* @return:
*   TRAINER_POKEMON_DOES_NOT_EXIST if id is NULL
*   TRAINER_POKEMON_HP_IS_AT_MAX if pokemon hp is 100.
*   TRAINER_NO_AVAILABLE_ITEM_FOUND if trainer has no potions
*   TRAINER_SUCCESS if used potion
*/
TrainerResult trainerPokemonUsePotion(Trainer trainer, int id);

/*----------------------------------------------------------------------------*/
/**
* Returns trainer name.
* @return NULL if sent NULL
* @param trainer
*/
char *trainerGetName(Trainer trainer);

/*----------------------------------------------------------------------------*/
/**
* Returns trainer current location
* @return NULL if sent NULL
* @param trainer
*/
char *trainerGetLocation(Trainer trainer);

/*----------------------------------------------------------------------------*/
/**
* Sets trainer location to a given new location.
*
* @param trainer, location name
*/
void trainerSetLocation(Trainer trainer, char *new_location);

/*----------------------------------------------------------------------------*/
/**
* Returns trainer experience
* @return NULL if sent NULL
* @param trainer
*/
double trainerGetExp(Trainer trainer);

/*----------------------------------------------------------------------------*/
/**
* Returns trainer pokecoins
* @return NULL if sent NULL
* @param trainer
*/
int trainerGetPokecoins(Trainer trainer);

/*----------------------------------------------------------------------------*/
/**
* Returns trainer number of pokemons
* @return NULL if sent NULL
* @param trainer
*/
int trainerGetNumOfPokemons(Trainer trainer);

/*----------------------------------------------------------------------------*/
/**
* Returns true if a trainer holds a pokemon in given id,
* false otherwise
* @param trainer, id
*/
bool trainerPokemonExist(Trainer trainer, int id);

/*----------------------------------------------------------------------------*/
/**
* Returns trainer store of items.
* @return NULL if sent NULL
* @param trainer
*/
Store trainerGetItems(Trainer trainer);

/*----------------------------------------------------------------------------*/
/**
* Returns trainer map of pokemons.
* @return NULL if sent NULL
* @param trainer
*/
Map trainerGetPokemons(Trainer trainer);

/*----------------------------------------------------------------------------*/
/**
* Sets trainer pokecoins to a given value
*
* @param trainer, pokecoins
*/
void trainerSetPokecoins(Trainer trainer, int pokecoins);

/*----------------------------------------------------------------------------*/
/**
* Adds a pokemon to trainers map in first free index.
* @param trainer, pokemon
* @return:
*   TRAINER_OUT_OF_MEM if failed to add pokemon to map
*   TRAINER_NULL_ARG if sent NULL
*   TRAINER_SUCCESS otherwise
*/
TrainerResult trainerAddPokemon(Trainer trainer, Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Removes a pokemon from trainers map by given id
* @param trainer, id
* @return:
*   TRAINER_INVALID_ARG if id < 0.
*   TRAINER_NULL_ARG if sent NULL
*   TRAINER_POKEMON_DOES_NOT_EXIST if no pokemon at given id
*   TRAINER_SUCCESS otherwise
*/
TrainerResult trainerRemovePokemon(Trainer trainer, int id);

/*----------------------------------------------------------------------------*/
/**
* Sets trainer experience to a given value.
*
* @param trainer, experience.
*/
void trainerSetExp(Trainer trainer, double exp);

/*----------------------------------------------------------------------------*/
#endif /* TRAINER_H */