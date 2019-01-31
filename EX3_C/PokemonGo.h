#ifndef POKEMON_GO_H
#define POKEMON_GO_H

#include "trainer.h"
#include "location.h"
#include "pokemon.h"
#include "store.h"
#include "print_utils.h"
#include "map.h"
#include "list.h"

typedef enum {
    POKEMON_GO_SUCCESS,
    POKEMON_GO_OUT_OF_MEM,
    POKEMON_GO_INVALID_COMMAND_LINE_PARAMETERS,
    POKEMON_GO_CANNOT_OPEN_FILE,
    POKEMON_GO_INVALID_ARG,
    POKEMON_GO_NULL_ARG,
    POKEMON_GO_TRAINER_NAME_ALREADY_EXISTS,
    POKEMON_GO_TRAINER_DOES_NOT_EXIST,
    POKEMON_GO_LOCATION_DOES_NOT_EXIST,
    POKEMON_GO_POKEMON_DOES_NOT_EXIST,
    POKEMON_GO_ITEM_OUT_OF_STOCK,
    POKEMON_GO_BUDGET_IS_NOT_SUFFICIENT,
    POKEMON_GO_HP_IS_AT_MAX,
    POKEMON_GO_NO_AVAILABLE_ITEM_FOUND,
    POKEMON_GO_LOCATION_IS_NOT_REACHABLE,
    POKEMON_GO_TRAINER_ALREADY_IN_LOCATION,
} PokemonGoResult;

typedef struct pokemon_go_t *PokemonGo;

/*----------------------------------------------------------------------------*/

/**
* Adds items by name, value and quantity to pokemonGo main store.
*
* @param pokemonGo, item name, value, quantity
* @return
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_INVALID_ARG if name of item is invalid,
*       value or quantity are 0 or less.
*   POKEMON_GO_OUT_OF_MEM if failed to add items.
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult pokemonGoAddItemsToStore(PokemonGo pokemonGo,
                                         char *item, int value, int quantity);

/*----------------------------------------------------------------------------*/
/**
* Updates a pokemons properties to it's evolution.
*
* @param pokemonGo, pokemon
*/
PokemonGoResult pokemonGoEvolve(PokemonGo pokemonGo, Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Creates a new pokemonGo
*
* @param pointer to result
* @return
* NULL if a NULL was sent or a memory allocation failed.
* new allocated pokemonGo otherwise
*/
PokemonGo pokemonGoCreate(PokemonGoResult *error);

/*----------------------------------------------------------------------------*/
/**
* Frees all memory allocated to pokemonGo
*
* @param pokemonGo
*/
void pokemonGoDestroy(PokemonGo pokemonGo);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemonGo pokedex map.
* @return NULL if sent NULL
* @param pokemonGo
*/
Map pokemonGoGetPokedex(PokemonGo pokemonGo);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemonGo locations map.
* @return NULL if sent NULL
* @param pokemonGo
*/
Map pokemonGoGetLocations(PokemonGo pokemonGo);


/*----------------------------------------------------------------------------*/
/**
* Adds a trainer to the pokemonGo trainers map in given location.
* the trainer catches the first pokemon in location if exists.
*
* @param pokemonGo, output channel, trainer name, budget, starting location.
* @return
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_INVALID_ARG if name of trainer or location is invalid, budget<0.
*   POKEMON_GO_LOCATION_DOES_NOT_EXIST if unknown location.
*   POKEMON_GO_OUT_OF_MEM if failed to allocate trainer memory.
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult pokemonGoAddTrainer(PokemonGo pokemonGo, FILE *output_channel,
                                    char *name, int budget, char *start_point);

/*----------------------------------------------------------------------------*/
/**
* Sets new location to trainer in pokemonGo.
* the trainer catches the first pokemon in new location if exists.
*
* @param pokemonGo, output channel, trainer name, new location.
* @return
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_TRAINER_DOES_NOT_EXIST if no such trainer.
*   POKEMON_GO_LOCATION_DOES_NOT_EXIST if unknown location.
*   POKEMON_GO_TRAINER_ALREADY_IN_LOCATION if current and new location
*       are the same.
*   POKEMON_GO_LOCATION_IS_NOT_REACHABLE if new location is not a nearby.
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult
pokemonGoTrainerMoveHunt(PokemonGo pokemonGo, FILE *output_channel, char *name,
                         char *go_to);

/*----------------------------------------------------------------------------*/
/**
* Trainer buys item from pokemonGo main store.
*
* @param pokemonGo, trainer name, item name and value.
* @return
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_INVALID_ARG if no such item or value < 0.
*   POKEMON_GO_TRAINER_DOES_NOT_EXIST if no such trainer.
*   POKEMON_GO_BUDGET_IS_NOT_SUFFICIENT if trainer doesn't have enough pokecoins.
*   POKEMON_GO_ITEM_OUT_OF_STOCK if store is out of stock.
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult
pokemonGoTrainerPurchase(PokemonGo pokemonGo, char *name, char *item,
                         int value);

/*----------------------------------------------------------------------------*/
/**
* Executes a battle between two trainers and their pokemons
*
* @param pokemonGo, output channel, trainers names, pokemons id's.
* @return
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_INVALID_ARG if trainer name is empty or id's <= 0.
*   POKEMON_GO_TRAINER_DOES_NOT_EXIST if no such trainer.
*   POKEMON_GO_POKEMON_DOES_NOT_EXIST if either pokemon does not exist
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult
pokemonGoTrainerBattle(PokemonGo pokemonGo, FILE *output_channel,
                       char *trainer1_name, int id1,
                       char *trainer2_name, int id2);

/*----------------------------------------------------------------------------*/
/**
* Trainer uses a potion on his pokemon by id.
*
* @param pokemonGo, trainer name, id of pokemon to heal.
* @return:
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_TRAINER_DOES_NOT_EXIST if no such trainer.
*   POKEMON_GO_POKEMON_DOES_NOT_EXIST if no such pokemon.
*   POKEMON_GO_NO_AVAILABLE_ITEM_FOUND if trainer has no potions.
*   POKEMON_GO_HP_IS_AT_MAX if pokemons hp is 100.
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult
pokemonGoTrainerHealPokemon(PokemonGo pokemonGo, char *name, int id);

/*----------------------------------------------------------------------------*/
/**
* Trainer uses a candy on his pokemon by id.
*
* @param pokemonGo, trainer name, id of pokemon to train.
* @return:
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_TRAINER_DOES_NOT_EXIST if no such trainer.
*   POKEMON_GO_POKEMON_DOES_NOT_EXIST if no such pokemon.
*   POKEMON_GO_NO_AVAILABLE_ITEM_FOUND if trainer has no candies.
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult
pokemonGoTrainerTrainPokemon(PokemonGo pokemonGo, char *name, int id);

/*----------------------------------------------------------------------------*/
/**
* Prints a full report of trainer:
*   name, location, budget, experience
*   candies, potions, pokemons,
*
* @param pokemonGo, trainer name, output channel.
* @return:
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_TRAINER_DOES_NOT_EXIST if no such trainer.
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult
pokemonGoReportTrainer(PokemonGo pokemonGo, char *name, FILE *output_channel);

/*----------------------------------------------------------------------------*/
/**
* Prints a full report of all locations:
*   by ascending lexicographic order, and for each:
*   -nearby locations
*   -pokemons.
*
* @param pokemonGo, trainer name, output channel.
* @return:
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult
pokemonGoReportLocations(PokemonGo pokemonGo, FILE *output_channel);

/*----------------------------------------------------------------------------*/
/**
* Prints a full report of store:
*   amount of candies, potions by ascending value
*
* @param pokemonGo, output channel.
* @return:
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult pokemonGoReportStock(PokemonGo pokemonGo, FILE *output_channel);

/*----------------------------------------------------------------------------*/
/**
* Adds a location to the pokemonGo locations map by given name.
*
* @param pokemonGo, output channel, trainer name, budget, starting location.
* @return
*   POKEMON_GO_NULL_ARG if sent NULL
*   POKEMON_GO_INVALID_ARG if name is invalid.
*   POKEMON_GO_OUT_OF_MEM if failed to allocate trainer memory.
*   POKEMON_GO_SUCCESS otherwise.
*/
PokemonGoResult pokemonGoAddLocation(PokemonGo pokemonGo, char *name);

/*----------------------------------------------------------------------------*/
#endif /* POKEMON_GO_H */