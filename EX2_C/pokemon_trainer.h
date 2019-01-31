#ifndef POKEMON_TRAINER_H_
#define POKEMON_TRAINER_H_

#include "pokemon.h"

typedef enum {
	POKEMON_TRAINER_SUCCESS,
	POKEMON_TRAINER_NULL_ARG,
	POKEMON_TRAINER_OUT_OF_MEM,
	POKEMON_TRAINER_INVALID_INDEX,
	POKEMON_TRAINER_PARTY_FULL,
	POKEMON_TRAINER_REMOVE_LAST,
	POKEMON_TRAINER_DEPOSIT_LAST,
	POKEMON_TRAINER_DEPOSIT_FULL
} PokemonTrainerResult;

typedef struct pokemon_trainer_t {
	char* name;
	Pokemon* local_pokemons;
	Pokemon* remote_pokemons;
	int num_of_local_pokemons, max_num_of_local_pokemons;
	int num_of_remote_pokemons, max_num_of_remote_pokemons;
} *PokemonTrainer;

/**
* Creates a new trainer.
* initial_pokemon sets the trainer's only pokemon, created via copy.
* max_num_local sets the trainer's max number of available pokemon.
* max_num_remote will sets the trainer's max number of deposited pokemon.
* 
* return
* 	A new allocated trainer.
* 	If name is NULL or empty, intial_pokemon is null, max_num_local
*   or max_num_remote is not larger than 0, return NULL.
**/
PokemonTrainer pokemonTrainerCreate(char* name, Pokemon initial_pokemon,
	int max_num_local, int max_num_remote);

/**
* Frees all memory allocated for the given pokemon trainer.
* This function can receive NULL. In this case, no action will be taken.
*/
void pokemonTrainerDestroy(PokemonTrainer trainer);


/**
* Creates a newly allocated copy of a given pokemon trainer.
*
* @return
*   A newly allocated copy of the original pokemon trainer.
*   NULL if the pokemon is NULL or in any case of memory allocation failure.
*/
PokemonTrainer pokemonTrainerCopy(PokemonTrainer trainer);

/**
* Creates a copy of a given pokemon and adds it to the trainer's party.
* Increases the number of pokemons a trainer holds.
*
* @return
*	POKEMON_TRAINER_NULL_ARG if pokemon or trainer args are NULL.
*   POKEMON_TRAINER_PARTY_FULL if trainer's party is full.
*   POKEMON_TRAINER_OUT_OF_MEM if the copied pokemon is NULL.
*	POKEMON_TRAINER_SUCCESS otherwise.
*/
PokemonTrainerResult pokemonTrainerAddPokemon(PokemonTrainer trainer,
	Pokemon pokemon);

/**
* Gets the pokemon from a trainer's party by given trainer and index.
*
* @return
*	NULL if trainer is NULL, pokemon_index is smaller than 1 or larger than
*   num_of_local_pokemons.
*/
Pokemon pokemonTrainerGetPokemon(PokemonTrainer trainer, int pokemon_index);

/**
* Removes pokemon by index from a trainer's party.
* Frees memory allocated for that pokemon.
* Decreases number of trainer's local pokemons.
*
* @return
*	POKEMON_TRAINER_NULL_ARG if trainer arg is NULL.
*   POKEMON_TRAINER_INVALID INDEX if pokemon_index is smaller than 1 or larger 
*		than num_of_local_pokemons.
*   POKEMON_TRAINER_REMOVE_LAST if trainer has only 1 pokemon 
*	POKEMON_TRAINER_OUT_OF_MEM if failed to shift pokemons left after removal.
*	POKEMON_TRAINER_SUCCESS otherwise.
*/
PokemonTrainerResult pokemonTrainerRemovePokemon(
	PokemonTrainer trainer, int pokemon_index);
	
/**
* Moves pokemon by index from a trainer's party to remote party via copy.
* Decreases number of trainer's local pokemons.
* Increases the number of pokemons in remote party.
* 
* @return
*	POKEMON_TRAINER_NULL_ARG if trainer arg is NULL.
*   POKEMON_TRAINER_INVALID INDEX if pokemon_index is smaller than 1 or larger 
*		than num_of_local_pokemons.
*   POKEMON_TRAINER_DEPOSIT_LAST if trainer has only 1 pokemon 
*	POKEMON_TRAINER_DEPOSIT_FULL if remote party is full.
*	POKEMON_TRAINER_OUT_OF_MEM if copied pokemon is NULL or failed to remove
*		last pokemon from local party.
*	POKEMON_TRAINER_SUCCESS otherwise.
*/	
PokemonTrainerResult pokemonTrainerDepositPokemon(
	PokemonTrainer trainer, int pokemon_index);

/**
* Moves pokemon by index from a remote party to trainer's local party via copy.
* Increases number of trainer's local pokemons.
* Decreases the number of pokemons in remote party.
* 
* @return
*	POKEMON_TRAINER_NULL_ARG if trainer arg is NULL.
*   POKEMON_TRAINER_INVALID INDEX if pokemon_index is smaller than 1 or larger 
*		than num_of_remote_pokemons.
*   POKEMON_TRAINER_PARTY_FULL if trainer holds max_num_of_local_pokemons. 
*	POKEMON_TRAINER_DEPOSIT_FULL if remote party is full.
*	POKEMON_TRAINER_OUT_OF_MEM if failed to add given pokemon to the party.
*	POKEMON_TRAINER_SUCCESS otherwise.
*/	
PokemonTrainerResult pokemonTrainerWithdrawPokemon(
	PokemonTrainer trainer, int pokemon_index);

/**
* Retrieves total number of pokemons a trainer holds including remote.
* 
* @return
*	total number of pokemons.
*/	
int pokemonTrainerGetNumberOfPokemons(PokemonTrainer trainer);

/**
* Retrieves the highest ranked pokemon of trainer.
* A local pokemon has priority if local and remote are of equal rank.
* 
* @return
*	Trainer's highest ranked pokemon.
*/	
Pokemon pokemonTrainerGetMostRankedPokemon(PokemonTrainer trainer);

/**
* Arranges trainer's local and remote pokemons by rank in decreasing order with
* priority for local pokemons.
* First each array, local and remote, is sorted by rank and then copied to
*	a combined array of all pokemons wich is sorted again by rank, then
*	copied back to local until it's full and then to remote.
* 
* @return
*	POKEMON_TRAINER_OUT_OF_MEM if failed to allocate memory for a combined 
*		array of all pokemons.
*	POKEMON_TRAINER_SUCCESS otherwise.
*/	
PokemonTrainerResult pokemonTrainerMakeMostRankedParty(PokemonTrainer trainer);

/**
* Prints to file for each pokemon in trainer's party according to the 
*	following format:
*	
*		<trainer>: <pokemon>
*		<pokemon>: <pokemon_voice>
* 
* @return
*	POKEMON_TRAINER_NULL_ARG if trainer or file is NULL.
*	POKEMON_TRAINER_OUT_OF_MEM if
*	POKEMON_TRAINER_SUCCESS otherwise.
*/
PokemonTrainerResult pokemonTrainerPrintEnumeration(
	PokemonTrainer trainer, FILE* file);

#endif // POKEMON_TRAINER_H_
