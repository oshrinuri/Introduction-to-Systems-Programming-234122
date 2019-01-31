#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <stdio.h>
#include <stdbool.h>

// Maximal input line length
#define MAX_LEN 120

/**
 * This type defines all errors for the system.
 * These codes should be used for calling mtmPrintErrorMessage to report
 * error messages.
 *
 * Notice that after calling mtmPrintErrorMessage with one of the first
 * three messages you should abort the program according to the exercise's
 * rules
 */
typedef enum {
	MTM_OUT_OF_MEMORY, // You should exit program after this error.
	MTM_INVALID_COMMAND_LINE_PARAMETERS, // You should exit the program after this
	// error.
			MTM_CANNOT_OPEN_FILE, // You should exit program after this error.
	MTM_INVALID_ARGUMENT,
	MTM_TRAINER_NAME_ALREADY_EXISTS,
	MTM_TRAINER_DOES_NOT_EXIST,
	MTM_LOCATION_DOES_NOT_EXIST,
	MTM_POKEMON_DOES_NOT_EXIST,
	MTM_ITEM_OUT_OF_STOCK,
	MTM_BUDGET_IS_NOT_SUFFICIENT,
	MTM_HP_IS_AT_MAX,
	MTM_NO_AVAILABLE_ITEM_FOUND ,
	MTM_TRAINER_ALREADY_IN_LOCATION,
	MTM_LOCATION_IS_NOT_REACHABLE,
	MTM_SUCCESS
} MtmErrorCode;

/**
 * mtmPrintErrorMessage - prints an error message into the given output channel
 * Use the standard error channel to print error messages.
 *
 * @param error_channel - File descriptor for the error channel
 * @param code - Error code to print message for.
 */
void mtmPrintErrorMessage(FILE* error_channel, MtmErrorCode code);

/**
 * mtmPrintBattle - prints battle description as follows:
 *
 *                           *** Pokemon Battle ***
 * <trainer_1>                         vs.                          <trainer_2>
 * <pokemon_1>(<cp_1>)                                        <pokemon_2>(cp_2)
 * <old_hp_1> -> <new_hp_1>            HP              <old_hp_2> -> <new_hp_2>
 * <old_level_1> -> <new_level_1>     Level      <old_level_2> -> <new_level_2>
 * <old_xp_1> -> <new_xp_1>            XP              <old_xp_2> -> <new_xp_2>
 *
 * [if <is_dead_1>] <trainer_1>'s <pokemon_1> died.
 * [if <is_dead_2>] <trainer_2>'s <pokemon_2> died.
 *
 * Where <pokemon_i> is the species of the pokemon used by <trainer_i> in the
 * battle.
 *
 * @param output_channel - File descriptor for the output channel
 */
void mtmPrintBattle(FILE* output_channel,
                    char* trainer_1, char* trainer_2,
										char* pokemon_1, char* pokemon_2,
										int cp_1, int cp_2,
										double old_hp_1, double old_hp_2,
										double new_hp_1, double new_hp_2,
										int old_level_1, int old_level_2,
										int new_level_1, int new_level_2,
										double old_xp_1, double old_xp_2,
										double new_xp_1, double new_xp_2,
										bool is_dead_1, bool is_dead_2);

/**
 * mtmPrintLocation - prints the header for the locations report.
 *
 * @param output_channel - File descriptor for the output channel.
 */
void mtmPrintLocationsHeader(FILE* output_channel);

/**
 * mtmPrintLocation - prints location description as follows:
 * 
 * [if <pokemon> != NULL] <location_name>: <pokemon>
 * [if <pokemon> == NULL] <location_name>: empty
 *
 * Where <pokemon> is the species of the pokemon held in the specified location.
 *
 * @param output_channel - File descriptor for the output channel
 */
void mtmPrintLocation(FILE* output_channel, char* location_name, char* pokemon);

/**
 * mtmPrintLocation - prints the header for the stock report.
 *
 * @param output_channel - File descriptor for the output channel.
 */
void mtmPrintStockHeader(FILE* output_channel);

/**
 * mtmPrintItem - prints item description. Should be used in both stock report
 * and trainer report (see below).
 *
 * @param output_channel - File descriptor for the output channel.
 * @param type - Item type (either "candy" or "potion").
 * @param value - Item value.
 * @param quantity - quantity in stock.
 */
void mtmPrintItem(FILE* output_channel, char* type, int value, int quantity);

/**
 * mtmPrintTrainerHeader - prints header for trainer report.
 * report.
 *
 * @param output_channel - File descriptor for the output channel.
 * @param name - trainer's name.
 * @param location - trainer's location.
 * @param budget - trainer's budget in Pokecoins.
 * @param xp - trainer's xp.
 */
void mtmPrintTrainerHeader(FILE* output_channel,
                           char* name,
                           char* location,
                           int budget,
                           double xp);
/**
 * mtmPrintItemsHeaderForTrainer - prints header for items list in trainer
 * report.
 *
 * @param output_channel - File descriptor for the output channel.
 */
void mtmPrintItemsHeaderForTrainer(FILE* output_channel);

/**
 * mtmPrintPokemonsHeaderForTrainer - prints header for pokemons list in trainer
 * report.
 *
 * @param output_channel - File descriptor for the output channel.
 */
void mtmPrintPokemonsHeaderForTrainer(FILE* output_channel);

/**
 * mtmPrintPokemon - prints pokemon description. Should be used in trainer
 * report.
 *
 * @param output_channel - File descriptor for the output channel.
 * @param id - pokemon id.
 * @param species - pokemon species.
 * @param hp - pokemon hp.
 * @param cp - pokemon cp.
 * @param level - pokemon level.
 */
void mtmPrintPokemon(FILE* output_channel,
                     int id,
                     char* species,
                     double hp,
										 int cp,
                     int level);

/**
 * mtmPrintPokemon - prints "trainer go" command result.
 *
 * @param output_channel - File descriptor for the output channel.
 * @param trainer - trainer name.
 * @param species - captured pokemon species. Pass NULL if location was found 
 *                  empty.
 * @param location - location in which trainer tried to catch a pokemon.
 */
void mtmPrintCatchResult(FILE* output_channel,
                         char* trainer,
                         char* species,
                         char* location);

#endif  // PRINT_UTILS_H
