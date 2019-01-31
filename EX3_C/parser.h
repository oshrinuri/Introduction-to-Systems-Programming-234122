#include <string.h>
#include <stdlib.h>
#include "print_utils.h"
#include "PokemonGo.h"

#define DELIMITER " \r\t\n"
#define READ_ONLY "r"
#define WRITE "w"
#define POKEDEX_FLAG_POS 1
#define EVOLUTIONS_FLAG_POS 3
#define LOCATIONS_FLAG_POS 5
#define POKEDEX_FILE_NAME argv[2]
#define EVOLUTIONS_FILE_NAME argv[4]
#define LOCATIONS_FILE_NAME argv[6]
#define EXECUTE_COMMAND_INPUT_LENGTH 9
#define EXECUTE_COMMAND_OUTPUT_LENGTH 11
#define EXECUTE_COMMAND_INPUT_POS 7
#define EXECUTE_COMMAND_OUTPUT_POS 9
#define MIN_EXECUTE_COMMAND_LENGTH 7
#define MAX_EXECUTE_COMMAND_LENGTH 11
#define MAX_INPUT_LEN 250

/**
 * Returns a relevant Mtm error code according to PokemonGo program errors
 * @param result - MtmError code that suits the argument's error code
*/
void PokemonGoResultToMtmErrorCode(PokemonGoResult result);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "trainer add" command
*/
void executeTrainerAdd(FILE *output_channel, char *string, PokemonGo pokemonGo,
                       PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "trainer go" command
*/
void executeTrainerGoToLocation(FILE *output_channel, char *string,
                                PokemonGo pokemonGo,
                                PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "trainer purchase" command
*/
void executeTrainerPurchase(char *string, PokemonGo pokemonGo,
                            PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "store add" command
*/
void executeStoreAdd(char *string, PokemonGo pokemonGo,
                     PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "battle fight" command
*/
void executePokemonGoTrainerBattle(FILE *output_channel, char *string,
                                   PokemonGo pokemonGo,
                                   PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "pokemon heal" command
*/
void executePokemonGoTrainerHealPokemon(char *string, PokemonGo pokemonGo,
                                        PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "pokemon train" command
*/
void executePokemonGoTrainerTrainPokemon(char *string, PokemonGo pokemonGo,
                                         PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "report $trainer" command
*/
void executePokemonGoReportTrainer(FILE *output_channel, char *string,
                                   PokemonGo pokemonGo,
                                   PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "report $location" command
*/
void executePokemonGoReportLocations(FILE *output_channel, char *string,
                                     PokemonGo pokemonGo,
                                     PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "report stock" command
*/
void executePokemonGoReportStock(FILE *output_channel, char *string,
                                 PokemonGo pokemonGo,
                                 PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "Trainer add" command
*/
void handleTrainerCommand(FILE *output, char *second_word, char *current_line,
                          PokemonGo pokemonGo, PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Calling function for "store" command
*/
void
handleStoreCommand(char *second_word, char *current_line, PokemonGo pokemonGo,
                   PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * "battle fight" parsing function
*/
void handleBattleCommand(FILE *output, char *second_word, char *current_line,
                         PokemonGo pokemonGo, PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * "pokemon" parsing function
*/
void
handlePokemonCommand(char *second_word, char *current_line, PokemonGo pokemonGo,
                     PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * "report" parsing function
*/
void handleReportCommand(FILE *output, char *second_word, char *current_line,
                         PokemonGo pokemonGo, PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Analyze a given command by comparing it to each of the known commands in the
 * PokemonGo system and call to the suitable function for that command
*/
void analyzeCurrentCommand(FILE *output, char *first_word, char *second_word,
                           char *current_line, PokemonGo pokemonGo,
                           PokemonGoResult *error_code);

/*----------------------------------------------------------------------------*/
/**
 * Split a given command given from a file / standard intput into tokens and
 * call the suitable function for that command
*/
void
handleCurrentCommand(FILE *output, char *current_command, PokemonGo pokemonGo,
                     MtmErrorCode *main_error_code);