#ifndef LOCATION_H_
#define LOCATION_H_

#include "list.h"
#include "pokemon.h"

typedef struct location_t* Location;

typedef enum {
	LOCATION_SUCCESS,
	LOCATION_NULL_ARG,
	LOCATION_INVALID_ARG,
	LOCATION_OUT_OF_MEM,
	LOCATION_NO_POKEMON
} LocationResult;

/*----------------------------------------------------------------------------*/
/**
* Creates a new location.
*
* @param name of location
* @return
* NULL if a NULL was sent or a memory allocation failed.
* new allocated location otherwise
*/
Location locationCreate(char* name, LocationResult* error);

/*----------------------------------------------------------------------------*/
/**
* Creates a copy of location.
*
* @param location to copy
* @return
* NULL if a NULL was sent or a memory allocation failed.
* new allocated location otherwise
*/
Location locationCopy(Location location, LocationResult* error);

/*----------------------------------------------------------------------------*/
/**
* Frees all memory allocated to location
*
* @param location
*/
void locationDestroy(Location location);

/*----------------------------------------------------------------------------*/
/**
* Returns the name of a location
*
* @param location
*/
char* locationGetName(Location location);

/*----------------------------------------------------------------------------*/
/**
* Returns the first pokemon in a location
*
* @param location
*/
Pokemon locationGetFirstPokemon(Location location);

/*----------------------------------------------------------------------------*/
/**
* Returns the list of pokemons in a location
*
* @param location
*/
List locationGetPokemonsList(Location location);

/*----------------------------------------------------------------------------*/
/**
* Adds a nearby element to the nearby list of location
*
* @param location, name of nearby
* @return result
*/
LocationResult locationAddNearby(Location current, char* nearby);

/*----------------------------------------------------------------------------*/
/**
* Adds a pokemon element to the pokeomon list of location
*
* @param location, pokemon
* @return result
*/
LocationResult locationAddPokemon(Location location, Pokemon pokemon);

/*----------------------------------------------------------------------------*/
/**
* Returns the list of nearbys of a location
*
* @param location
*/
List locationGetNearby(Location location);

/*----------------------------------------------------------------------------*/
/**
* Removes the first pokemon of the pokeomn list of location
*
* @param location
* @return result
*/
LocationResult locationRemoveFirstPokemon(Location location);

/*----------------------------------------------------------------------------*/
#endif /* LOCATION_H */