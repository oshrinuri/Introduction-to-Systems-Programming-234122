#include "location.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//---------------------------Defines & Declarations-----------------------------

struct location_t {
    char *name;
    List nearby;
    List pokemons;
};

//---------------------------Auxiliary Functions--------------------------------
/**
* String copy function to be used by the List structure
* @param location_name - A Location's name
* @return A newlly allocated copy of location_name
*/
static ListElement stringListCopy(ListElement location_name) {
    char *copy = malloc(strlen((char *) location_name) + 1);
    if (!copy) {
        return NULL;
    }
    strcpy(copy, (char *) location_name);
    return copy;
}
/*----------------------------------------------------------------------------*/
/**
* Memory deallocation function for a string used for the List structure
* to pass to List
* @param location_name - A location's name
*/
static void stringListDestroy(ListElement location_name) {
    free(location_name);
}
/*----------------------------------------------------------------------------*/
/**
* Pokemon copy function to be used by the List structure
* @param pokemon - A Pokemon to be copied
* @return A newly allocated copy of pokemon
*/
static ListElement pokemonListCopy(ListElement pokemon) {
    PokemonResult error = POKEMON_SUCCESS;
    Pokemon copy = pokemonCopy((Pokemon) pokemon, &error);
    if (!copy) {
        return NULL;
    }
    return copy;
}
/*----------------------------------------------------------------------------*/
/**
* Memory deallocation function for a Pokemon used for the List structure
* @param pokemon - A pokemon to be deallocated
*/
static void pokemonListDestroy(ListElement pokemon) {
    pokemonDestroy((Pokemon) pokemon);
}

//-------------------------- Main Functions ------------------------------------

Location locationCreate(char *name, LocationResult* error) {
    *error = LOCATION_OUT_OF_MEM;
    if (!name) {
        *error = LOCATION_NULL_ARG;
        return NULL;
    }
    Location location = malloc(sizeof(*location));
    if (!location) {
        return NULL;
    }
    List nearby = listCreate(stringListCopy, stringListDestroy);
    if (!nearby) {
        locationDestroy(location);
        return NULL;
    }
    List pokemons = listCreate(pokemonListCopy, pokemonListDestroy);
    if (!pokemons) {
        locationDestroy(location);
        return NULL;
    }
    location->nearby = nearby;
    location->pokemons = pokemons;
    location->name = malloc(strlen(name) + 1);
    if (!location->name) return NULL;
    strcpy(location->name, name);
    *error = LOCATION_SUCCESS;
    return location;
}
/*----------------------------------------------------------------------------*/
Location locationCopy(Location location, LocationResult* error) {
    *error = LOCATION_OUT_OF_MEM;
    if (!location) {
        *error = LOCATION_NULL_ARG;
        return NULL;
    }
    Location copy = locationCreate(location->name, error);
    if (!copy) {
        return NULL;
    }
    listDestroy(copy->nearby);
    listDestroy(copy->pokemons);
    copy->nearby = listCopy(location->nearby);
    if (!copy->nearby) {
        return NULL;
    }
    copy->pokemons = listCopy(location->pokemons);
    if (!copy->pokemons) {
        return NULL;
    }
    *error = LOCATION_SUCCESS;
    return copy;
}
/*----------------------------------------------------------------------------*/
void locationDestroy(Location location) {
    if (!location) return;
    free(location->name);
    listDestroy(location->nearby);
    listDestroy(location->pokemons);
    free(location);
}
/*----------------------------------------------------------------------------*/
char *locationGetName(Location location) {
    return location->name;
}
/*----------------------------------------------------------------------------*/
Pokemon locationGetFirstPokemon(Location location) {
    if (!location) {
        return NULL;
    }
    return listGetFirst(location->pokemons);
}
/*----------------------------------------------------------------------------*/
List locationGetPokemonsList(Location location) {
    assert(location!=NULL);
    return location->pokemons;
}
/*----------------------------------------------------------------------------*/
LocationResult locationAddPokemon(Location location, Pokemon pokemon) {
    if (!location || !pokemon) {
        return LOCATION_NULL_ARG;
    }
    listInsertLast(location->pokemons, pokemon);
    return LOCATION_SUCCESS;
}
/*----------------------------------------------------------------------------*/
LocationResult locationAddNearby(Location current, char *nearby) {
    if (strcmp(locationGetName(current), nearby) == 0) {
        return LOCATION_INVALID_ARG;
    }
    listInsertLast(current->nearby, nearby);
    return LOCATION_SUCCESS;
}
/*----------------------------------------------------------------------------*/
List locationGetNearby(Location location) {
    assert(location!=NULL);
    return location->nearby;
}
/*----------------------------------------------------------------------------*/
LocationResult locationRemoveFirstPokemon(Location location) {
    if (!location) {
        return LOCATION_NULL_ARG;
    }
    Pokemon first_pokemon = listGetFirst(location->pokemons);
    if (!first_pokemon) {
        return LOCATION_NO_POKEMON;
    }
    listRemoveCurrent(location->pokemons);
    return LOCATION_SUCCESS;
}
/*----------------------------------------------------------------------------*/