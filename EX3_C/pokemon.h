#ifndef POKEMON_H_
#define POKEMON_H_

#define MIN_HP 0.0
#define MAX_HP 100.0
#define NO_EVOLUTION -1

#include "list.h"

typedef struct pokemon_t *Pokemon;

typedef enum {
    TYPE_NORMAL, TYPE_FIRE, TYPE_WATER,
    TYPE_GRASS, TYPE_ELECTRIC,
    TYPE_BUG, TYPE_FLYING, TYPE_FAIRY,
    TYPE_GHOST, TYPE_GROUND, TYPE_ICE,
    TYPE_POISON, TYPE_PSYCHIC, TYPE_ROCK
} PokemonType;

typedef enum {
    POKEMON_SUCCESS,
    POKEMON_NULL_ARG,
    POKEMON_INVALID_ARG,
    POKEMON_OUT_OF_MEM,
    POKEMON_INVALID_TYPE,
    POKEMON_INVALID_POWER_POINTS,
    POKEMON_INVALID_STRENGTH,
    POKEMON_INVALID_NAME,
    POKEMON_INVALID_HP,
    POKEMON_NO_POWER_POINTS,
    POKEMON_NO_HEALTH_POINTS,
    POKEMON_CANNOT_EVOLVE
} PokemonResult;

/*----------------------------------------------------------------------------*/

/**
* Creates a new pokemon.
*
* @param name, evolution name, evolution level, cp, list of types.
* @return
* NULL if a NULL was sent or a memory allocation failed.
* new allocated pokemon otherwise
*/
Pokemon pokemonCreate(char* name, char* evolution_name, int evolution_level,
                      int cp, List types, PokemonResult* error);
/*----------------------------------------------------------------------------*/
/**
* Frees all memory allocated to pokemon
*
* @param pokemon
*/
void pokemonDestroy(Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Creates a copy of pokemon.
*
* @param pokemon to copy
* @return
* NULL if a NULL was sent or a memory allocation failed.
* new allocated pokemon otherwise
*/
Pokemon pokemonCopy(Pokemon pokemon, PokemonResult* error);

/*----------------------------------------------------------------------------*/
/**
* Sets pokemon hp to a given value.
*
* @param pokemon, hp
*/
void pokemonSetHp(Pokemon pokemon, double hp);

/*----------------------------------------------------------------------------*/
/**
* Adds to pokemon cp by a given value.
*
* @param pokemon, cp
*/
void pokemonAddCp(Pokemon pokemon, int points);

/*----------------------------------------------------------------------------*/
/**
* Adds to pokemon bonus cp by a given value.
*
* @param pokemon, bouns cp
*/
void pokemonAddBonusCp(Pokemon pokemon, int points);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemon hp.
*
* @param pokemon
*/
double pokemonGetHp(Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemon cp.
*
* @param pokemon
*/
int pokemonGetCp(Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemon level.
*
* @param pokemon
*/
int pokemonGetLevel(Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemon evolution level.
*
* @param pokemon
*/
int pokemonGetEvolutionLevel(Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemon evolution name.
*
* @param pokemon
*/
char *pokemonGetEvolutionName(Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemon bonus cp.
*
* @param pokemon
*/
int pokemonGetBonusCp(Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemon name.
*
* @param pokemon
*/
char *pokemonGetName(Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Sets a pokemon name to a given string
*
* @param pokemon, name
*/
void pokemonSetName(Pokemon pokemon, char* name);

/*----------------------------------------------------------------------------*/
/**
* Returns pokemon list of types.
*
* @param pokemon
*/
List pokemonGetTypes(Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Sets a pokemon types list to a types list
*
* @param pokemon, list of types
*/
void pokemonSetTypes(Pokemon pokemon, List types);

/*----------------------------------------------------------------------------*/
/**
* Sets a pokemon evolution name to a given string
*
* @param pokemon, evolution name
*/
void pokemonSetEvolutionName(Pokemon pokemon, char *evolution_name);

/*----------------------------------------------------------------------------*/
/**
* Sets a pokemon evolution level to a given value
*
* @param pokemon, evolution level
*/
void pokemonSetEvolutionLevel(Pokemon pokemon, int evolution_level);

/*----------------------------------------------------------------------------*/
/**
* Sets a pokemon level to a given value
*
* @param pokemon, level
*/
void pokemonSetLevel(Pokemon pokemon, int level);

/*----------------------------------------------------------------------------*/
/**
* Sets a pokemon cp to a given value
*
* @param pokemon, cp
*/
void pokemonSetCp(Pokemon pokemon, int cp);

/*----------------------------------------------------------------------------*/
/**
* Copies a type list element
* to pass to List
* @param type
* @return an element for list
*/
ListElement copyType(ListElement type);

/*----------------------------------------------------------------------------*/
/**
* Frees memory of type in list
* to pass to List
* @param type
*/
void freeType(ListElement type);

/*----------------------------------------------------------------------------*/
#endif /* POKEMON_H */