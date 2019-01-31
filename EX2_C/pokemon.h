#ifndef POKEMON_H_
#define POKEMON_H_

#include <stdio.h>
#include <stdbool.h>

typedef enum {
	TYPE_NORMAL,
	TYPE_FIRE,
	TYPE_WATER,
	TYPE_GRASS,
	TYPE_ELECTRIC
} PokemonType;

typedef enum {
	POKEMON_SUCCESS,
	POKEMON_NULL_ARG,
	POKEMON_OUT_OF_MEM,
	POKEMON_INVALID_MOVE_NAME,
	POKEMON_INVALID_TYPE,
	POKEMON_INVALID_POWER_POINTS,
	POKEMON_INVALID_STRENGTH,
	POKEMON_INVALID_NAME,
	POKEMON_INCOMPATIBLE_MOVE_TYPE,
	POKEMON_MOVE_ALREADY_EXISTS,
	POKEMON_MOVE_DOES_NOT_EXIST,
	POKEMON_NO_POWER_POINTS,
	POKEMON_NO_HEALTH_POINTS,
	POKEMON_CANNOT_EVOLVE
} PokemonResult;

typedef struct pokemon_move_t {
	char* name;
	PokemonType type;
	int power_points, max_power_points;
	int strength;
} *PokemonMove;

/*
* The pokemon structure.
*/
typedef struct pokemon_t {
	char* name;
	PokemonType type;
	int experience;
	int health_points;
	PokemonMove* moves;
	int number_of_moves, max_number_of_moves;
} *Pokemon;

/**
* Creates a new pokemon.
* The name, type, and experience parameters set the corresponding properties
* of the pokemon.
* The max_number_of_moves parameter sets the maximum amount of moves that the
* pokemon is allowed to know at any given time.
* The pokemon is created without moves, and with full health points
* (see the pokemonHeal function).
*
* @return
* 	A new allocated pokemon.
* 	If name is NULL or empty, type is invalid, experience or max_number_of_moves
*   is not positive, or in case of a memory allocation failure - return NULL.
**/
Pokemon pokemonCreate(char* name, PokemonType type, int experience,
	int max_number_of_moves);

/**
* Frees all memory allocated for the given pokemon.
* This function can receive NULL. In this case, no action will be taken.
*/
void pokemonDestroy(Pokemon pokemon);

/**
* Creates a newly allocated copy of a given pokemon.
*
* @return
*   A newly allocated copy of the original pokemon.
*   NULL if the pokemon is NULL or in any case of memory allocation failure.
*/
Pokemon pokemonCopy(Pokemon pokemon);

/**
* Teaches the given pokemon a new move.
* A pokemon can only learn a move which is compatible with its type: a move of
* a type which is either normal, or equals to the pokemon's type.
* After creation, the move power points equal to the maximum power points value.
* If the pokemon already knows max_number_of_moves moves, the move should
* replace the first existing move in lexicographical order (use strcmp).
*
* @return
* 	POKEMON_NULL_ARG if pokemon is NULL or move_name is NULL.
* 	POKEMON_INVALID_MOVE_NAME if move_name is empty.
* 	POKEMON_INVALID_TYPE if type is illegal.
*	POKEMON_INVALID_POWER_POINTS if max_power_points is not positive.
*	POKEMON_INVALID_STRENGTH if strength is not positive.
*   POKEMON_INCOMPATIBLE_MOVE_TYPE if the type of the move is not compatible
*   with the type of the pokemon (see the description above).
*	POKEMON_MOVE_ALREADY_EXISTS if the pokemon already knows a move with the 
*   given name.
* 	POKEMON_SUCCESS otherwise.
*/
PokemonResult pokemonTeachMove(Pokemon pokemon, char* move_name,
	PokemonType type, int max_power_points, int strength);

/**
* Unteaches the given pokemon a move.
*
* @return
* 	POKEMON_NULL_ARG if pokemon is NULL or move_name is NULL.
* 	POKEMON_INVALID_MOVE_NAME if move_name is empty.
*	POKEMON_MOVE_DOES_NOT_EXIST if the pokemon doesn't know this move.
* 	POKEMON_SUCCESS otherwise.
*/
PokemonResult pokemonUnteachMove(Pokemon pokemon, char* move_name);

/**
* Calculates the level of the given pokemon.
* The level is calculated using the following formula, rounded up:
* level = experience / 100
*
* This function asserts (pokemon != NULL).
*/
int pokemonGetLevel(Pokemon pokemon);

/**
* Calculates the strength rank of the given pokemon.
* The rank is calculated using the following formula:
* rank = level + average_move_strength
* Where average_move_strength is the average value of the move strengths.
* If the average value is not an integer, it should be rounded down.
* If the pokemon has no moves, the rank is zero.
*
* This function asserts (pokemon != NULL).
*/
int pokemonGetRank(Pokemon pokemon);

/**
* Uses a given move on an opponent pokemon in a battle.
* If the function succeeds, the following happens:
* 1. The health points of opponent_pokemon are reduced by the following formula:
*    health_points = health_points - (attacker_level * 2 + move_strength) * factor
*    The "factor" variable equals 2 when the attacker move type has advantage
*    over the opponent pokemon type. This happens when:
*    Water attacks fire, fire attacks grass, grass attacks water,
*    electric attacks water
*    In any other scenario, the "factor" variable equals 1.
*    The health points cannot be reduced below zero.
* 2. The power points of the used move decrease by 1.
* 3. The experience of the attacking pokemon is increased by the health points
*    reduced for the opponent pokemon. The experience cannot be increased above
*    10000.
*
* @return
* 	POKEMON_NULL_ARG if pokemon, opponent_pokemon or move_name is NULL.
* 	POKEMON_INVALID_MOVE_NAME if move_name is empty.
*	POKEMON_MOVE_DOES_NOT_EXIST if the pokemon doesn't know this move.
*	POKEMON_NO_POWER_POINTS if there are no power points left for the move.
*	POKEMON_NO_HEALTH_POINTS if the health points value of one of the involved
*   pokemons is zero.
* 	POKEMON_SUCCESS otherwise.
*/
PokemonResult pokemonUseMove(Pokemon pokemon, Pokemon opponent_pokemon,
	char* move_name);

/**
* Heals the given pokemon.
* If the function succeeds, the following happens:
* 1. The health points of the pokemon change to the pokemon level plus 100.
* 2. The power points of all of the moves reset to the maximum power points.
*
* @return
* 	POKEMON_NULL_ARG if pokemon is NULL.
* 	POKEMON_SUCCESS otherwise.
*/
PokemonResult pokemonHeal(Pokemon pokemon);
	

/**
* Evolves the given pokemon.
* If the function succeeds, the pokemon gets a new name, and its experience
* value rounds up to the next multiple of 100.
*
* @return
* 	POKEMON_NULL_ARG if pokemon or new_name is NULL.
* 	POKEMON_INVALID_NAME if move_name is empty.
*   POKEMON_CANNOT_EVOLVE if the experience of the pokemon is 10000.
* 	POKEMON_SUCCESS otherwise.
*/
PokemonResult pokemonEvolve(Pokemon pokemon, char* new_name);

/**
* Prints the name of the pokemon.
*
* @return
* 	POKEMON_NULL_ARG if pokemon or file is NULL.
* 	POKEMON_SUCCESS otherwise.
*/
PokemonResult pokemonPrintName(Pokemon pokemon, FILE* file);

/**
* Prints the voice that the pokemon makes.
* If the function succeeds, the voice that is printed is based on the pokemon's
* name.
* First, a string is created by removing all of the non-letter characters from
* the name. For example: "Ho-Oh" -> "HoOh".
* Second, if the resulting string is smaller than 4 characters, the whole string
* is printed twice, joined with a dash. Otherwise, half of it is printed
* (rounded up) twice, joined with a dash.
* Examples: "HoOh" -> "Ho-Ho", "Pikachu" -> "Pika-Pika", "Mew" -> "Mew-Mew".
*
* @return
* 	POKEMON_NULL_ARG if pokemon or file is NULL.
* 	POKEMON_SUCCESS otherwise.
*/
PokemonResult pokemonPrintVoice(Pokemon pokemon, FILE* file);

#endif // POKEMON_H_
