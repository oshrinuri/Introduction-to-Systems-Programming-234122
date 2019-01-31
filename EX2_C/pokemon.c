#include "pokemon.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

/*- CONSTANTS AND DEFINES ----------------------------------------------------*/
#define NEUTRAL   1
#define EFFECTIVE 2
#define MAX_VOICE_SIZE 4
#define MAX_EXP 9901

/*- AUXILIARY FUNCTIONS ------------------------------------------------------*/

/** Rounds x upward,
 * returning the smallest integral value that is not less than x.
 */
static inline int ceilPos(double x) {
    return (x)-(int)(x) > 0 ? (int)(x+1) : (int)(x);
}

/**
 * Checks whether a given Pokemon Type is legal.
 * @return
 *		true: If the given type is legal.
 *		false: Otherwise.
 */
static bool isLegalType(PokemonType type) {
    switch (type) {
        case TYPE_NORMAL: case TYPE_FIRE: case TYPE_WATER: case TYPE_GRASS:
        case TYPE_ELECTRIC: return true;
    }
    return false;
}

/**
 * Checks whether a given Pokemon has a certain move in his moves list.
 * @return
 * 		true: If the given move is already known by the Pokemon.
 * 		false: Otherwise.
 */
static bool isKnownMove(Pokemon pokemon, char* move_name) {
    for (int i=0; i<pokemon->number_of_moves; ++i) {
        if ( strcmp(pokemon->moves[i]->name, move_name) == 0 ) {
            return true;
        }
    }
    return false;
}

/**
 * Checks whether a given type is effective against another.
 * @return
 *		EFFECTIVE: for effective factor.
 *		NEUTRAL  : for neutral factor.
*/
static int getEffectiveness(PokemonType type1, PokemonType type2) {
    if ( (type1 == TYPE_WATER    && type2 == TYPE_FIRE ) ||
        (type1 == TYPE_FIRE     && type2 == TYPE_GRASS) ||
        (type1 == TYPE_ELECTRIC && type2 == TYPE_WATER) ||
        (type1 == TYPE_GRASS    && type2 == TYPE_WATER) ) {
        return EFFECTIVE;
    }
    return NEUTRAL;
}

/**
 * Removes non-alphabetic chars from string
 * @return
 *		The original string containing alphabetical letters only.
*/
static char* removeNonAlphabet(char* str) {
    int i, j;
    for(i = 0; str[i] != '\0'; ++i) {
        while (!( (str[i] >= 'a' && str[i] <= 'z') ||
                 (str[i] >= 'A' && str[i] <= 'Z')  || str[i] == '\0') ) {
            for(j = i; str[j] != '\0'; ++j) {
                str[j] = str[j+1];
            }
            str[j] = '\0';
        }
    }
    return str;
}

/**
 * Memory allocation for a Pokemon's moves list.
 * @return
 * 		NULL in case of any failure in memory allocation.
 * 		Otherwise, return a newly allocated EMPTY moves list.
 */
static PokemonMove* allocateMoves(Pokemon pokemon) {
    PokemonMove* moves = malloc(sizeof(PokemonMove*)
                                *(pokemon->max_number_of_moves));
    if (!moves) {
        return NULL;
    }
    return moves;
}

/**
 * Iterate through a given Pokemon's move list and return an integer
 * representing the index of the first move by lexcial order.
 * @return
 *		Index of the first move by lexical order of a given Pokemon.
 */
static int getFirstLexicalMoveIndex(Pokemon pokemon) {
    int currentMoveIdx = 0;
    for (int i=1; i<pokemon->number_of_moves;++i) {
        if (strcmp(pokemon->moves[currentMoveIdx]->name,
                   pokemon->moves[i]->name) > 0) {
            currentMoveIdx = i;
        }
    }
    return currentMoveIdx;
}

/**
 * Frees all memory allocated for a given pokemon moves.
 */
static void movesDestroy(Pokemon pokemon) {
    for (int i=0;i<pokemon->number_of_moves;++i) {
        free(pokemon->moves[i]->name);
        free(pokemon->moves[i]);
    }
    free(pokemon->moves);
}

/**
 * Shifts a given Pokemon move list array one cell to the left.
 * @return
 *      POKEMON_OUT_OF_MEM: In case of a memory allocation failure.
 *      POKEMON_SUCCESS: Shifting succeeded.
 *
 */
static PokemonResult shiftMovesLeft(Pokemon pokemon, int startPos) {
    for (int i=startPos;i < pokemon->number_of_moves-1;++i) {
        char* new_name = malloc(strlen(pokemon->moves[i+1]->name)+1);
        if (!new_name) return POKEMON_OUT_OF_MEM;
        free(pokemon->moves[i]->name);
        pokemon->moves[i]->name = strcpy(new_name,pokemon->moves[i+1]->name);
        pokemon->moves[i]->type = pokemon->moves[i+1]->type;
        pokemon->moves[i]->max_power_points = pokemon->moves[i+1]->max_power_points;
        pokemon->moves[i]->power_points = pokemon->moves[i+1]->power_points;
        pokemon->moves[i]->strength = pokemon->moves[i+1]->strength;
    }
    return POKEMON_SUCCESS;
}

/**
 * Frees the last-indexed memory of a given Pokemon move list array.
 */
static void destroyLastMove(Pokemon pokemon) {
    free(pokemon->moves[pokemon->number_of_moves-1]->name);
    free(pokemon->moves[pokemon->number_of_moves-1]);
}

/**
 * Allocate memory for a given Pokemon's new move in a given index within his
 * move list array and insert the new move to the Pokemon moves list.
 * @param pokemon:
 *        newMoveIdx: New move's index in the Pokemon's move list array.
 *        move_name: New move's name.
 *        type: Type of new move.
 *        max_power_points: Max power points of new move.
 *        strength: Strength of new move.
 * @return
 *      POKEMON_OUT_OF_MEM: In case of a memory allocation failure.
 *      POKEMON_SUCCESS: Shifting succeeded.
 *
 */
static PokemonResult newMoveAllocation(Pokemon pokemon, int newMoveIdx,
                                       char* move_name, PokemonType type,
                                       int max_power_points, int strength) {
    pokemon->moves[newMoveIdx] = malloc(sizeof(*(pokemon->moves[newMoveIdx])));
    if (!pokemon->moves[newMoveIdx]) { return POKEMON_OUT_OF_MEM; }
    pokemon->moves[newMoveIdx]->name = malloc(strlen(move_name)+1);
    if (!pokemon->moves[newMoveIdx]->name) { return POKEMON_OUT_OF_MEM; }
    strcpy(pokemon->moves[newMoveIdx]->name,move_name);
    pokemon->moves[newMoveIdx]->type = type;
    pokemon->moves[newMoveIdx]->power_points = max_power_points;
    pokemon->moves[newMoveIdx]->max_power_points = max_power_points;
    pokemon->moves[newMoveIdx]->strength = strength;
    return POKEMON_SUCCESS;
}

/**
 * Copies entire move list of a given Pokemon into a given empty move list.
 * @param pokemon: Pokemon containing the move list to be copied.
 * @param moves: Empty array of lists.
 * @return
 *      False in case of memory allocationg failure during coping,
 *      True otherwise.
 */
static bool copyMoves(Pokemon pokemon, PokemonMove* moves) {
    for (int i=0;i<pokemon->number_of_moves;++i) {
        moves[i]->name = malloc(strlen(pokemon->moves[i]->name)+1);
        if (!(moves[i]->name)) {
            return false;
        }
        strcpy(moves[i]->name,pokemon->moves[i]->name);
        moves[i]->max_power_points = pokemon->moves[i]->max_power_points;
        moves[i]->power_points = pokemon->moves[i]->power_points;
        moves[i]->strength = pokemon->moves[i]->strength;
        moves[i]->type = pokemon->moves[i]->type;
    }
    return true;
}

/**
 * Finds the index of a move within moves array.
 * @return
 *      index if found,
 *     -1 otherwise.
 */

static int getMoveIndex(Pokemon pokemon, char* move_name) {
    for (int i=0;i<pokemon->number_of_moves;++i) {
        if (strcmp(pokemon->moves[i]->name,move_name)==0) {
            return i;
        }
    }
    return -1;
}
/*- MAIN FUNCTIONS -----------------------------------------------------------*/

Pokemon pokemonCreate(char* name, PokemonType type, int experience,
                      int max_number_of_moves) {
    if (!name || strlen(name) == 0 || !isLegalType(type) ||
        experience <= 0 || experience > MAX_EXP || max_number_of_moves <= 0) {
        return NULL;
    }
    Pokemon newPokemon = malloc(sizeof(*newPokemon));
    if (!newPokemon) {
        return NULL;
    }
    newPokemon->name = malloc(strlen(name)+1);
    if (!newPokemon->name) {
        free(newPokemon);
        return NULL;
    }
    strcpy(newPokemon->name, name);
    newPokemon->type = type;
    newPokemon->experience = experience;
    newPokemon->health_points = (100+ceilPos(experience/100.0))*10;
    newPokemon->number_of_moves = 0;
    newPokemon->max_number_of_moves = max_number_of_moves;
    newPokemon->moves = allocateMoves(newPokemon);
    if (!(newPokemon->moves)) {
        free(newPokemon->name);
        free(newPokemon);
        return NULL;
    }
    return newPokemon;
}

void pokemonDestroy(Pokemon pokemon) {
    if (!pokemon) {
        return;
    }
    movesDestroy(pokemon);
    free(pokemon->name);
    free(pokemon);
}

Pokemon pokemonCopy(Pokemon pokemon) {
    if (!pokemon) {
        return NULL;
    }
    Pokemon copy = pokemonCreate(pokemon->name, pokemon->type,
                                 pokemon->experience,
                                 pokemon->max_number_of_moves);
    if (!copy) {
        return NULL;
    }
    copy->number_of_moves = pokemon->number_of_moves;
    for (int i=0;i<pokemon->number_of_moves;++i) {
        copy->moves[i] = malloc(sizeof(*(copy->moves[i])));
        if (!(copy->moves[i])) return NULL;
    }
    if (!copyMoves(pokemon, copy->moves)) {
        free(copy);
        return NULL;
    }
    return copy;
}

PokemonResult pokemonTeachMove(Pokemon pokemon, char* move_name,
                               PokemonType type, int max_power_points, int strength) {
    if (!pokemon || !move_name)		return POKEMON_NULL_ARG;
    if (strlen(move_name) == 0) 	return POKEMON_INVALID_MOVE_NAME;
    if (!isLegalType(type)) 		return POKEMON_INVALID_TYPE;
    if (max_power_points <= 0)		return POKEMON_INVALID_POWER_POINTS;
    if (strength <= 0)				return POKEMON_INVALID_STRENGTH;
    if (type != TYPE_NORMAL && pokemon->type != type) {
        return POKEMON_INCOMPATIBLE_MOVE_TYPE;
    }
    if (isKnownMove(pokemon,move_name)) return POKEMON_MOVE_ALREADY_EXISTS;
    if (pokemon->number_of_moves == pokemon->max_number_of_moves) {
        int del_pos = getFirstLexicalMoveIndex(pokemon);
        pokemonUnteachMove(pokemon,pokemon->moves[del_pos]->name);
    }
    int newMoveIdx = pokemon->number_of_moves;
    if (newMoveAllocation(pokemon,newMoveIdx,
                          move_name,type,max_power_points,
                             strength) == POKEMON_OUT_OF_MEM) {
        return POKEMON_OUT_OF_MEM;
    }
    pokemon->number_of_moves++;
    return POKEMON_SUCCESS;
}

PokemonResult pokemonUnteachMove(Pokemon pokemon, char* move_name) {
    if (!pokemon || !move_name)	       	 return POKEMON_NULL_ARG;
    if (strlen(move_name) == 0) 	   	 return POKEMON_INVALID_MOVE_NAME;
    if (!isKnownMove(pokemon,move_name)) return POKEMON_MOVE_DOES_NOT_EXIST;
    for (int i=0;i<pokemon->number_of_moves;++i) {
        if (strcmp(pokemon->moves[i]->name, move_name) == 0) {
            if (i != pokemon->number_of_moves-1) {
                if (shiftMovesLeft(pokemon,i) == POKEMON_OUT_OF_MEM) {
                    return POKEMON_OUT_OF_MEM;
                }
            }
            destroyLastMove(pokemon);
            break;
        }
    }
    pokemon->number_of_moves--;
    return POKEMON_SUCCESS;
}

int pokemonGetLevel(Pokemon pokemon) {
    assert (pokemon!=NULL);
    double level = pokemon->experience/100.0;
    return ceilPos(level);
}

int pokemonGetRank(Pokemon pokemon) {
    assert (pokemon!=NULL);
    if ( pokemon->number_of_moves == 0 ) {
        return 0;
    }
    int sum=0;
    for (int i=0; i < pokemon->number_of_moves; ++i) {
        sum += pokemon->moves[i]->strength;
    }
    return pokemonGetLevel(pokemon) + (sum/pokemon->number_of_moves);
}

PokemonResult pokemonUseMove(Pokemon pokemon, Pokemon opponent_pokemon,
                             char* move_name) {
    if ( !pokemon || !opponent_pokemon || !move_name ) {
        return POKEMON_NULL_ARG;
    }
    if ( strlen(move_name)==0 ) return POKEMON_INVALID_MOVE_NAME;
    if (!isKnownMove(pokemon, move_name)) return POKEMON_MOVE_DOES_NOT_EXIST;
    int move_idx = getMoveIndex(pokemon,move_name);
    if ( pokemon->moves[move_idx]->power_points == 0 ) {
        return POKEMON_NO_POWER_POINTS;
    }
    if ( pokemon->health_points == 0 || opponent_pokemon->health_points == 0) {
        return POKEMON_NO_HEALTH_POINTS;
    }
    int damage =
            ( (pokemonGetLevel(pokemon))*2 + pokemon->moves[move_idx]->strength ) *
            ( getEffectiveness(pokemon->type, opponent_pokemon->type) );
    opponent_pokemon->health_points -= damage;
    if ( opponent_pokemon->health_points < 0 ) {
        opponent_pokemon->health_points = 0;
    }
    pokemon->experience += damage;
    if ( pokemon->experience > MAX_EXP ) {
        pokemon->experience = MAX_EXP;
    }
    pokemon->moves[move_idx]->power_points--;
    return POKEMON_SUCCESS;
}

PokemonResult pokemonHeal(Pokemon pokemon) {
    if (!pokemon) {
        return POKEMON_NULL_ARG;
    }
    pokemon->health_points = (100 + pokemonGetLevel(pokemon))*10 ;
    for (int i=0; i<pokemon->number_of_moves; ++i) {
        pokemon->moves[i]->power_points = pokemon->moves[i]->max_power_points;
    }
    return POKEMON_SUCCESS ;
}

PokemonResult pokemonEvolve(Pokemon pokemon, char* new_name) {
    if ( !pokemon || !new_name ) {
        return POKEMON_NULL_ARG;
    }
    if ( strlen(new_name)==0 ) {
        return POKEMON_INVALID_NAME;
    }
    if (pokemon->experience == MAX_EXP) {
        return POKEMON_CANNOT_EVOLVE;
    }
    pokemon->experience += 100 - ( pokemon->experience + 99 ) % 100 ;
    if (pokemon->experience > MAX_EXP) {
        pokemon->experience = MAX_EXP;
    }
    free(pokemon->name);
    pokemon->name = malloc(strlen(new_name)+1);
    if (!pokemon->name) return POKEMON_OUT_OF_MEM;
    strcpy(pokemon->name,new_name);
    return POKEMON_SUCCESS;
}

PokemonResult pokemonPrintName(Pokemon pokemon, FILE* file) {
    if ( !pokemon || !file ) {
        return POKEMON_NULL_ARG;
    }
    fprintf(file, "%s" ,pokemon->name);
    return POKEMON_SUCCESS;
}

PokemonResult pokemonPrintVoice(Pokemon pokemon, FILE* file) {
    if ( !pokemon || !file ) {
        return POKEMON_NULL_ARG;
    }
    char* voice = malloc(strlen(pokemon->name)+1);
    if (!voice) {
        return POKEMON_OUT_OF_MEM;
    }
    strcpy(voice,pokemon->name);
    voice = removeNonAlphabet(voice);
    double len = strlen(voice);
    if ( len >= MAX_VOICE_SIZE ) {
        int voice_len = ceilPos(( len / 2));
        voice[voice_len]='\0';
        fprintf(file, "%s-%s",voice,voice);
    } else {
        fprintf(file, "%s-%s",voice,voice);
    }
    free(voice);
    return POKEMON_SUCCESS;
}