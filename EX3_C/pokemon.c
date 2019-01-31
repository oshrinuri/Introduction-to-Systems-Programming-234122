#include "pokemon.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//---------------------------Defines & Declarations-----------------------------

#define MAX_NAME_SIZE 10
#define START_LEVEL 1
#define DEFAULT_BONUS_CP 0

struct pokemon_t {
	char name[MAX_NAME_SIZE+1];
	char evolution_name[MAX_NAME_SIZE+1];
	int level; 
	int evolution_level;
	double hp;
	int cp; 
	int bonus_cp;
	List types;
};

ListElement copyType(ListElement type) {
    if (!type) {
        return NULL;
    }
    PokemonType* copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(PokemonType*)type;
    return copy;
}

void freeType(ListElement type) {
    free(type);
}

//---------------------------Main Functions------------------------------------- 

Pokemon pokemonCreate(char* name, char* evolution_name, int evolution_level,
                      int cp, List types, PokemonResult* error) {
    *error = POKEMON_SUCCESS;
    if (!name || !evolution_name) {
        *error = POKEMON_NULL_ARG;
        return NULL;
    }
    if (strlen(name) == 0) {
        *error = POKEMON_INVALID_ARG;
    }
    Pokemon new_pokemon = malloc(sizeof(*new_pokemon));
    if (!new_pokemon) {
        *error = POKEMON_OUT_OF_MEM;
        return NULL;
    }
    strcpy(new_pokemon->name, name);
    strcpy(new_pokemon->evolution_name, evolution_name);
    new_pokemon->evolution_level = evolution_level;
    new_pokemon->level = START_LEVEL;
    new_pokemon->hp = MAX_HP;
    new_pokemon->cp = cp;
    new_pokemon->bonus_cp = DEFAULT_BONUS_CP;
    new_pokemon->types = listCopy(types);
    if (!new_pokemon->types) {
        *error = POKEMON_OUT_OF_MEM;
        free(new_pokemon);
        return NULL;
    }
    return new_pokemon;
}
/*----------------------------------------------------------------------------*/
void pokemonDestroy(Pokemon pokemon) {
    if (!pokemon) return;
    listDestroy(pokemon->types);
    free(pokemon);
}
/*----------------------------------------------------------------------------*/
Pokemon pokemonCopy(Pokemon pokemon, PokemonResult* error) {
    if (!pokemon) {
        *error = POKEMON_NULL_ARG;
        return NULL;
    }
    Pokemon copy = pokemonCreate(pokemon->name, pokemon->evolution_name,
                                 pokemon->evolution_level, pokemon->cp,
                                 pokemon->types, error);
    if (!copy) {
        *error = POKEMON_OUT_OF_MEM;
        return NULL;
    }
    *error = POKEMON_SUCCESS;
    return copy;
}
/*----------------------------------------------------------------------------*/
void pokemonSetHp(Pokemon pokemon, double hp) {
    assert(pokemon!=NULL);
    if ( hp < MIN_HP) {
        pokemon->hp = MIN_HP;
    } else if (hp > MAX_HP) {
        pokemon->hp = MAX_HP;
    } else {
        pokemon->hp = hp;
    }
}
/*----------------------------------------------------------------------------*/
void pokemonAddCp(Pokemon pokemon, int points) {
    assert(pokemon!=NULL);
    if (points < 0) {
        return;
    }
    pokemon->cp += points;
}
/*----------------------------------------------------------------------------*/
void pokemonAddBonusCp(Pokemon pokemon, int points) {
    assert(pokemon!=NULL);
    if (points < 0) {
        return;
    }
    pokemon->bonus_cp += points;
}
/*----------------------------------------------------------------------------*/
double pokemonGetHp(Pokemon pokemon) {
    assert(pokemon!=NULL);
    return pokemon->hp;
}
/*----------------------------------------------------------------------------*/
int pokemonGetCp(Pokemon pokemon) {
    assert(pokemon!=NULL);
    return pokemon->cp;
}
/*----------------------------------------------------------------------------*/
int pokemonGetLevel(Pokemon pokemon) {
    assert(pokemon!=NULL);
    return pokemon->level;
}
/*----------------------------------------------------------------------------*/
int pokemonGetEvolutionLevel(Pokemon pokemon) {
    assert(pokemon!=NULL);
    return pokemon->evolution_level;
}
/*----------------------------------------------------------------------------*/
char* pokemonGetEvolutionName(Pokemon pokemon) {
    assert(pokemon!=NULL);
    return pokemon->evolution_name;
}
/*----------------------------------------------------------------------------*/
int pokemonGetBonusCp(Pokemon pokemon) {
    assert(pokemon!=NULL);
    return pokemon->bonus_cp;
}
/*----------------------------------------------------------------------------*/
char* pokemonGetName(Pokemon pokemon) {
    return pokemon->name;
}
/*----------------------------------------------------------------------------*/
List pokemonGetTypes(Pokemon pokemon){
    assert(pokemon!=NULL);
    return pokemon->types;
}
/*----------------------------------------------------------------------------*/
void pokemonSetTypes(Pokemon pokemon, List types) {
    assert(pokemon!=NULL);
    pokemon->types = types;
}
/*----------------------------------------------------------------------------*/
void pokemonSetName(Pokemon pokemon, char* name) {
    assert(pokemon!=NULL && name!=NULL);
    strcpy(pokemon->name, name);
}
/*----------------------------------------------------------------------------*/
void pokemonSetEvolutionName(Pokemon pokemon, char* evolution_name) {
    assert(pokemon!=NULL && evolution_name!=NULL);
    strcpy(pokemon->evolution_name,evolution_name);
}
/*----------------------------------------------------------------------------*/
void pokemonSetEvolutionLevel(Pokemon pokemon, int evolution_level) {
    assert(pokemon!=NULL);
    if (evolution_level < START_LEVEL) {
        return;
    }
    pokemon->evolution_level = evolution_level;
}
/*----------------------------------------------------------------------------*/
void pokemonSetLevel(Pokemon pokemon, int level) {
    assert(pokemon!=NULL);
    if (level < START_LEVEL) {
        return;
    }
    pokemon->level = level;
}
/*----------------------------------------------------------------------------*/
void pokemonSetCp(Pokemon pokemon, int cp) {
    assert(pokemon!=NULL);
    if (cp <= 0) {
        return;
    }
    pokemon->cp = cp;
}
/*----------------------------------------------------------------------------*/