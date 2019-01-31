#include "pokemon_trainer.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*- CONSTANTS AND DEFINES ----------------------------------------------------*/
typedef enum {
    LOCAL, REMOTE
} PokemonLocation;

/*- AUXILIARY FUNCTIONS ------------------------------------------------------*/

/**
* Destroys a trainer's last pokemon by given location.
**/
static void destroyLastPokemon(PokemonTrainer trainer,
                               PokemonLocation location) {
    (location == LOCAL) ?
    pokemonDestroy(trainer->local_pokemons[trainer->num_of_local_pokemons-1]):
    pokemonDestroy(trainer->remote_pokemons[trainer->num_of_remote_pokemons-1]);
}

/**
* Calculates the index of the highest ranked pokemon by given location.
* starting from given index.
* @return
*      index of highest ranked pokemon.
**/
static int getHighestRank(PokemonTrainer trainer, PokemonLocation location) {
    int num_pokemons = (location == LOCAL)?trainer->num_of_local_pokemons:
                       trainer->num_of_remote_pokemons;
    Pokemon* pokemons = (location == LOCAL)?trainer->local_pokemons:
                        trainer->remote_pokemons;
    int highest_rank_idx = 0;
    for (int i=num_pokemons-1; i>0; --i) {
        if( pokemonGetRank(pokemons[i]) >=
            pokemonGetRank(pokemons[i-1]) ){
            highest_rank_idx = i;
        }
    }
    return highest_rank_idx;
}

/**
* Shifts pokemons to the left in array by a given starting position.
* pokemons will hold the pokemon array.
* size will hold the array size.
* start_pos will hold the index from which to shift left.
**/
static PokemonTrainerResult shiftPokemonsLeft(Pokemon *pokemons, int size,
                                              int start_pos) {
    for (int i = start_pos; i < size - 1; ++i) {
        pokemonDestroy(pokemons[i]);
        pokemons[i] = pokemonCopy(pokemons[i + 1]);
        if (!pokemons[i]) {
            return POKEMON_TRAINER_OUT_OF_MEM;
        }
    }
    return POKEMON_TRAINER_SUCCESS;
}

/**
* Removes a trainer's remote pokemon by a given index.
**/
static PokemonTrainerResult
pokemonTrainerRemoveRemotePokemon(PokemonTrainer trainer,
                                  int pokemon_index) {
    if (pokemon_index != trainer->num_of_remote_pokemons) {
        if (shiftPokemonsLeft(trainer->remote_pokemons,
                              trainer->num_of_remote_pokemons,
                              pokemon_index - 1) ==
            POKEMON_TRAINER_OUT_OF_MEM) {
            return POKEMON_TRAINER_OUT_OF_MEM;
        }
    }
    destroyLastPokemon(trainer, REMOTE);
    trainer->num_of_remote_pokemons--;

    return POKEMON_TRAINER_SUCCESS;
}

/**
* Allocates memory for a new trainer.
* The name parameter will be the new trainer's name.
* max_num_local will hold the maximum number of pokemons a trainer can hold.
* max_num_remote will hold the maximum
* number of pokemons a trainer can keep deposited.
*
*  @return
*    A new allocated trainer.
*    if memmory allocation fails return NULL.
**/
static PokemonTrainer trainerMemoryAllocations(char *name, int max_num_local,
                                               int max_num_remote) {
    PokemonTrainer newTrainer = malloc(sizeof(*newTrainer));
    if (!newTrainer) {
        return NULL;
    }
    newTrainer->name = malloc(strlen(name) + 1);
    strcpy(newTrainer->name, name);
    if (!newTrainer->name) {
        free(newTrainer);
        return NULL;
    }
    newTrainer->local_pokemons = malloc(sizeof(*(newTrainer->local_pokemons))
                                        * max_num_local);
    if (!newTrainer->local_pokemons) {
        free(newTrainer->name);
        free(newTrainer);
        return NULL;
    }
    newTrainer->remote_pokemons = malloc(sizeof(*(newTrainer->remote_pokemons))
                                         * max_num_remote);
    if (!newTrainer->remote_pokemons) {
        free(newTrainer->name);
        free(newTrainer->local_pokemons);
        free(newTrainer);
        return NULL;
    }
    return newTrainer;
}

/**
 * Bubble-sort (pointers only) of a given Pokemon array by Pokemons' ranks.
 * [Sorting is by descending order: Highest ranked Pokemon will be first]
 * @param pokemons: Array of Pokemons (pointers).
 * @param size: The Array's size.
 */
static void sortPtrsByRank(Pokemon* pokemons, int size) {
    if (size == 1) return;
    for (int i = 0; i < size ; ++i) {
        for (int j = 0; j < size-1; ++j) {
            if (pokemonGetRank(pokemons[j + 1]) >
                pokemonGetRank(pokemons[j])) {
                Pokemon tempPtr = pokemons[j];
                pokemons[j] = pokemons[j + 1];
                pokemons[j + 1] = tempPtr;
            }
        }
    }
}

/**
 * Given a Pokemon pointers array, copy the array's pointers to a destnation
 * array.
 * @param src: Source pointers array (To be copied from).
 * @param dest: Destination pointers array.
 * @param end_src: Last index to copy from the source array.
 * @param start_dest: Index to start copying to destination array.
 */
static void copyPtrs(Pokemon* src, Pokemon* dest, int end_src,int start_dest) {
    for (int i = 0; i < end_src; ++i) {
        dest[start_dest+i] = src[i];
    }
}

/*- MAIN FUNCTIONS -----------------------------------------------------------*/

PokemonTrainer pokemonTrainerCreate(char *name, Pokemon initial_pokemon,
                                    int max_num_local, int max_num_remote) {
    if (!name || strlen(name)==0 || !initial_pokemon || max_num_local <= 0 ||
        max_num_remote <= 0) {
        return NULL;
    }
    PokemonTrainer newTrainer = trainerMemoryAllocations(name, max_num_local,
                                                         max_num_remote);
    if (!newTrainer) return NULL;
    newTrainer->max_num_of_local_pokemons = max_num_local;
    newTrainer->max_num_of_remote_pokemons = max_num_remote;
    newTrainer->num_of_local_pokemons = 0;
    newTrainer->num_of_remote_pokemons = 0;
    pokemonTrainerAddPokemon(newTrainer, initial_pokemon);

    return newTrainer;
}

void pokemonTrainerDestroy(PokemonTrainer trainer) {
    if (!trainer) {
        return;
    }
    for (int i = 0; i < trainer->num_of_local_pokemons; ++i) {
        pokemonDestroy(trainer->local_pokemons[i]);
    }
    for (int i = 0; i < trainer->num_of_remote_pokemons; ++i) {
        pokemonDestroy(trainer->remote_pokemons[i]);
    }
    free(trainer->name);
    free(trainer->local_pokemons);
    free(trainer->remote_pokemons);
    free(trainer);
}

PokemonTrainer pokemonTrainerCopy(PokemonTrainer trainer) {
    if (!trainer) {
        return NULL;
    }
    PokemonTrainer copy = pokemonTrainerCreate(trainer->name,
                                               trainer->local_pokemons[0],
                                               trainer->max_num_of_local_pokemons,
                                               trainer->max_num_of_remote_pokemons);
    if (!copy) {
        return NULL;
    }
    for (int i = 1; i < trainer->num_of_local_pokemons; ++i) {
        pokemonTrainerAddPokemon(copy, trainer->local_pokemons[i]);
    }
    for (int i = 0; i < trainer->num_of_remote_pokemons; ++i) {
        copy->remote_pokemons[i] = pokemonCopy(trainer->remote_pokemons[i]);
    }

    return copy;
}

PokemonTrainerResult pokemonTrainerAddPokemon(PokemonTrainer trainer,
                                              Pokemon pokemon) {
    if (!trainer || !pokemon) {
        return POKEMON_TRAINER_NULL_ARG;
    }
    if (trainer->num_of_local_pokemons ==
        trainer->max_num_of_local_pokemons) {
        return POKEMON_TRAINER_PARTY_FULL;
    }
    Pokemon copy = pokemonCopy(pokemon);
    if (!copy) {
        return POKEMON_TRAINER_OUT_OF_MEM;
    }
    int pokemon_idx = trainer->num_of_local_pokemons;
    trainer->local_pokemons[pokemon_idx] = copy;
    trainer->num_of_local_pokemons++;

    return POKEMON_SUCCESS;
}

Pokemon pokemonTrainerGetPokemon(PokemonTrainer trainer, int pokemon_index) {
    if (!trainer || pokemon_index < 1 ||
        pokemon_index > trainer->num_of_local_pokemons) {
        return NULL;
    }

    return trainer->local_pokemons[pokemon_index - 1];
}

PokemonTrainerResult pokemonTrainerRemovePokemon(
        PokemonTrainer trainer, int pokemon_index) {
    if (!trainer) {
        return POKEMON_TRAINER_NULL_ARG;
    }
    if (pokemon_index < 1 || pokemon_index > trainer->num_of_local_pokemons) {
        return POKEMON_TRAINER_INVALID_INDEX;
    }
    if (trainer->num_of_local_pokemons == 1) {
        return POKEMON_TRAINER_REMOVE_LAST;
    }
    if (pokemon_index != trainer->num_of_local_pokemons) {
        if (shiftPokemonsLeft(trainer->local_pokemons,
                              trainer->num_of_local_pokemons,
                              pokemon_index - 1) ==
            POKEMON_TRAINER_OUT_OF_MEM) {
            return POKEMON_TRAINER_OUT_OF_MEM;
        }
    }
    destroyLastPokemon(trainer, LOCAL);
    trainer->num_of_local_pokemons--;

    return POKEMON_TRAINER_SUCCESS;
}

PokemonTrainerResult pokemonTrainerDepositPokemon(
        PokemonTrainer trainer, int pokemon_index) {
    if (!trainer) {
        return POKEMON_TRAINER_NULL_ARG;
    }
    if (pokemon_index < 1 || pokemon_index > trainer->num_of_local_pokemons) {
        return POKEMON_TRAINER_INVALID_INDEX;
    }
    if (trainer->num_of_local_pokemons == 1) {
        return POKEMON_TRAINER_DEPOSIT_LAST;
    }
    if (trainer->num_of_remote_pokemons ==
        trainer->max_num_of_remote_pokemons) {
        return POKEMON_TRAINER_DEPOSIT_FULL;
    }
    int remote_pos = trainer->num_of_remote_pokemons;
    trainer->remote_pokemons[remote_pos] =
            pokemonCopy(trainer->local_pokemons[pokemon_index - 1]);
    if (!trainer->remote_pokemons[remote_pos]) {
        return POKEMON_TRAINER_OUT_OF_MEM;
    }
    if (pokemonTrainerRemovePokemon(trainer, pokemon_index) ==
        POKEMON_TRAINER_OUT_OF_MEM) {
        return POKEMON_TRAINER_OUT_OF_MEM;
    }
    trainer->num_of_remote_pokemons++;

    return POKEMON_TRAINER_SUCCESS;
}

PokemonTrainerResult pokemonTrainerWithdrawPokemon(
        PokemonTrainer trainer, int pokemon_index) {
    if (!trainer) {
        return POKEMON_TRAINER_NULL_ARG;
    }
    if (pokemon_index < 1 || pokemon_index > trainer->num_of_remote_pokemons) {
        return POKEMON_TRAINER_INVALID_INDEX;
    }
    if (trainer->num_of_local_pokemons == trainer->max_num_of_local_pokemons) {
        return POKEMON_TRAINER_PARTY_FULL;
    }
    if (pokemonTrainerAddPokemon(trainer,
                                 trainer->remote_pokemons[pokemon_index - 1]) ==
        POKEMON_TRAINER_OUT_OF_MEM) {
        return POKEMON_TRAINER_OUT_OF_MEM;
    }
    if (pokemonTrainerRemoveRemotePokemon(trainer, pokemon_index) ==
        POKEMON_TRAINER_OUT_OF_MEM) {
        return POKEMON_TRAINER_OUT_OF_MEM;
    }

    return POKEMON_TRAINER_SUCCESS;
}

int pokemonTrainerGetNumberOfPokemons(PokemonTrainer trainer) {
    assert(trainer != NULL);

    return trainer->num_of_local_pokemons + trainer->num_of_remote_pokemons;
}

Pokemon pokemonTrainerGetMostRankedPokemon(PokemonTrainer trainer) {
    if (!trainer) {
        return NULL;
    }
    bool search_local_only = (trainer->num_of_remote_pokemons == 0)?true:false;
    int idx_local = getHighestRank(trainer, LOCAL);
    int idx_remote = getHighestRank(trainer, REMOTE);

    if (search_local_only) {
        return trainer->local_pokemons[idx_local];
    }
    else {
        int rank_local = pokemonGetRank(trainer->local_pokemons[idx_local]);
        int rank_remote = pokemonGetRank(trainer->remote_pokemons[idx_remote]);
        return (rank_local>=rank_remote)?trainer->local_pokemons[idx_local]:
               trainer->remote_pokemons[idx_remote];
    }
}

PokemonTrainerResult pokemonTrainerMakeMostRankedParty(PokemonTrainer trainer) {
    if (!trainer) return POKEMON_TRAINER_NULL_ARG;
    int remote_size  = trainer->num_of_remote_pokemons;
    int local_size = trainer->num_of_local_pokemons;
    int combined_size = remote_size + local_size ;

    sortPtrsByRank(trainer->local_pokemons, local_size);
    sortPtrsByRank(trainer->remote_pokemons, remote_size);

    Pokemon* combined = malloc(sizeof(*combined)*(combined_size));
    if (!combined) return POKEMON_TRAINER_OUT_OF_MEM;

    copyPtrs(trainer->local_pokemons,combined,local_size,0);
    copyPtrs(trainer->remote_pokemons,combined,remote_size,local_size);

    sortPtrsByRank(combined,combined_size);

    for (int i=0;i<trainer->num_of_local_pokemons;++i) {
        trainer->local_pokemons[i] = combined[i];
    }
    for (int i=local_size,x=0;i<remote_size+local_size;i++) {
        trainer->remote_pokemons[x++] = combined[i];
    }
    free(combined);
    for (int i=0;i<=trainer->num_of_remote_pokemons;++i) {
        if (trainer->num_of_local_pokemons < trainer->max_num_of_local_pokemons) {
            pokemonTrainerWithdrawPokemon(trainer,1);
        }
    }
    return POKEMON_TRAINER_SUCCESS;
}

PokemonTrainerResult pokemonTrainerPrintEnumeration(PokemonTrainer trainer,
                                                    FILE* file) {
    if ( !file || !trainer ) {
        return POKEMON_TRAINER_NULL_ARG;
    }

    for (int i=0; i<trainer->num_of_local_pokemons; ++i) {
        fprintf (file, "%s: ", trainer->name);
        pokemonPrintName(trainer->local_pokemons[i],file);
        fprintf(file,"\n");
        pokemonPrintName(trainer->local_pokemons[i],file);
        fprintf(file,": ");
        if (pokemonPrintVoice(trainer->local_pokemons[i], file) == POKEMON_OUT_OF_MEM) {
            return POKEMON_TRAINER_OUT_OF_MEM;
        }
        fprintf (file, "\n");
    }
    return POKEMON_TRAINER_SUCCESS;
}