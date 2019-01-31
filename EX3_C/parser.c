#include "parser.h"

void PokemonGoResultToMtmErrorCode(PokemonGoResult result) {
    MtmErrorCode error_code = MTM_SUCCESS;
    switch (result) {
        case POKEMON_GO_SUCCESS:
            return;
        case POKEMON_GO_OUT_OF_MEM:
            error_code = MTM_OUT_OF_MEMORY;
            break;
        case POKEMON_GO_INVALID_COMMAND_LINE_PARAMETERS:
            error_code = MTM_INVALID_COMMAND_LINE_PARAMETERS;
            break;
        case POKEMON_GO_CANNOT_OPEN_FILE:
            error_code = MTM_CANNOT_OPEN_FILE;
            break;
        case POKEMON_GO_INVALID_ARG:
            error_code = MTM_INVALID_ARGUMENT;
            break;
        case POKEMON_GO_NULL_ARG:
            error_code = MTM_INVALID_ARGUMENT;
            break;
        case POKEMON_GO_TRAINER_NAME_ALREADY_EXISTS:
            error_code = MTM_TRAINER_NAME_ALREADY_EXISTS;
            break;
        case POKEMON_GO_TRAINER_DOES_NOT_EXIST:
            error_code = MTM_TRAINER_DOES_NOT_EXIST;
            break;
        case POKEMON_GO_LOCATION_DOES_NOT_EXIST:
            error_code = MTM_LOCATION_DOES_NOT_EXIST;
            break;
        case POKEMON_GO_POKEMON_DOES_NOT_EXIST:
            error_code = MTM_POKEMON_DOES_NOT_EXIST;
            break;
        case POKEMON_GO_ITEM_OUT_OF_STOCK:
            error_code = MTM_ITEM_OUT_OF_STOCK;
            break;
        case POKEMON_GO_BUDGET_IS_NOT_SUFFICIENT:
            error_code = MTM_BUDGET_IS_NOT_SUFFICIENT;
            break;
        case POKEMON_GO_HP_IS_AT_MAX:
            error_code = MTM_HP_IS_AT_MAX;
            break;
        case POKEMON_GO_NO_AVAILABLE_ITEM_FOUND:
            error_code = MTM_NO_AVAILABLE_ITEM_FOUND;
            break;
        case POKEMON_GO_TRAINER_ALREADY_IN_LOCATION:
            error_code = MTM_TRAINER_ALREADY_IN_LOCATION;
            break;
        case POKEMON_GO_LOCATION_IS_NOT_REACHABLE:
            error_code = MTM_LOCATION_IS_NOT_REACHABLE;
            break;
    }
    mtmPrintErrorMessage(stderr, error_code);
}

// ------------------------ Trainer Functions ----------------------------------

void executeTrainerAdd(FILE *output_channel, char *string, PokemonGo pokemonGo,
                       PokemonGoResult *error_code) {
    char *name = strtok(NULL, DELIMITER);
    int budget = atoi(strtok(NULL, DELIMITER));
    char *start_point = strtok(NULL, DELIMITER);
    *error_code = pokemonGoAddTrainer(pokemonGo, output_channel, name, budget,
                                      start_point);
    PokemonGoResultToMtmErrorCode(*error_code);
}
/*----------------------------------------------------------------------------*/
void executeTrainerGoToLocation(FILE *output_channel, char *string,
                                PokemonGo pokemonGo,
                                PokemonGoResult *error_code) {
    char *name = strtok(NULL, DELIMITER);
    char *location = strtok(NULL, DELIMITER);
    *error_code = pokemonGoTrainerMoveHunt(pokemonGo, output_channel, name,
                                           location);
    PokemonGoResultToMtmErrorCode(*error_code);
}
/*----------------------------------------------------------------------------*/
void executeTrainerPurchase(char *string, PokemonGo pokemonGo,
                            PokemonGoResult *error_code) {
    char *name = strtok(NULL, DELIMITER);
    char *item = strtok(NULL, DELIMITER);
    int value = atoi(strtok(NULL, DELIMITER));
    *error_code = pokemonGoTrainerPurchase(pokemonGo, name, item, value);
    PokemonGoResultToMtmErrorCode(*error_code);
}

// ------------------------ Store Functions ------------------------------------

void executeStoreAdd(char *string, PokemonGo pokemonGo,
                     PokemonGoResult *error_code) {
    char *item = strtok(NULL, DELIMITER);
    int value = atoi(strtok(NULL, DELIMITER));
    int quantity = atoi(strtok(NULL, DELIMITER));
    *error_code = pokemonGoAddItemsToStore(pokemonGo, item, value, quantity);
    PokemonGoResultToMtmErrorCode(*error_code);
}

// ------------------------ Battle Functions -----------------------------------

void executePokemonGoTrainerBattle(FILE *output_channel, char *string,
                                   PokemonGo pokemonGo,
                                   PokemonGoResult *error_code) {
    char *trainer1 = strtok(NULL, DELIMITER);
    char *trainer2 = strtok(NULL, DELIMITER);
    int pokemon_id1 = atoi(strtok(NULL, DELIMITER));
    int pokemon_id2 = atoi(strtok(NULL, DELIMITER));
    *error_code = pokemonGoTrainerBattle(pokemonGo, output_channel, trainer1,
                                         pokemon_id1, trainer2, pokemon_id2);
    PokemonGoResultToMtmErrorCode(*error_code);
}

// ------------------------ Pokemon Functions ----------------------------------

void executePokemonGoTrainerHealPokemon(char *string, PokemonGo pokemonGo,
                                        PokemonGoResult *error_code) {
    char *trainer = strtok(NULL, DELIMITER);
    int pokemon_id = atoi(strtok(NULL, DELIMITER));
    *error_code = pokemonGoTrainerHealPokemon(pokemonGo, trainer, pokemon_id);
    PokemonGoResultToMtmErrorCode(*error_code);
}
/*----------------------------------------------------------------------------*/
void executePokemonGoTrainerTrainPokemon(char *string, PokemonGo pokemonGo,
                                         PokemonGoResult *error_code) {
    char *trainer = strtok(NULL, DELIMITER);
    int pokemon_id = atoi(strtok(NULL, DELIMITER));
    *error_code = pokemonGoTrainerTrainPokemon(pokemonGo, trainer, pokemon_id);
    PokemonGoResultToMtmErrorCode(*error_code);
}

// ------------------------ Report Functions -----------------------------------

void executePokemonGoReportTrainer(FILE *output_channel, char *string,
                                   PokemonGo pokemonGo,
                                   PokemonGoResult *error_code) {
    char *trainer = strtok(NULL, DELIMITER);
    *error_code = pokemonGoReportTrainer(pokemonGo, trainer, output_channel);
    PokemonGoResultToMtmErrorCode(*error_code);
}
/*----------------------------------------------------------------------------*/
void executePokemonGoReportLocations(FILE *output_channel, char *string,
                                     PokemonGo pokemonGo,
                                     PokemonGoResult *error_code) {
    *error_code = pokemonGoReportLocations(pokemonGo, output_channel);
    PokemonGoResultToMtmErrorCode(*error_code);
}
/*----------------------------------------------------------------------------*/
void executePokemonGoReportStock(FILE *output_channel, char *string,
                                 PokemonGo pokemonGo,
                                 PokemonGoResult *error_code) {
    *error_code = pokemonGoReportStock(pokemonGo, output_channel);
    PokemonGoResultToMtmErrorCode(*error_code);
}

// Trainer commands ------------------------------------------------------------
void handleTrainerCommand(FILE *output, char *second_word, char *current_line,
                          PokemonGo pokemonGo, PokemonGoResult *error_code) {
    if (strcmp(second_word, "add") == 0) {
        executeTrainerAdd(output, current_line, pokemonGo, error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) return;
    } else if (strcmp(second_word, "go") == 0) {
        executeTrainerGoToLocation(output, current_line, pokemonGo, error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) return;
    } else if (strcmp(second_word, "purchase") == 0) {
        executeTrainerPurchase(current_line, pokemonGo, error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) return;
    } else {
        *error_code = POKEMON_GO_INVALID_COMMAND_LINE_PARAMETERS;
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
    }
}

// Store commands --------------------------------------------------------------
void
handleStoreCommand(char *second_word, char *current_line, PokemonGo pokemonGo,
                   PokemonGoResult *error_code) {
    if (strcmp(second_word, "add") == 0) {
        executeStoreAdd(current_line, pokemonGo, error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) return;
    } else {
        *error_code = POKEMON_GO_INVALID_COMMAND_LINE_PARAMETERS;
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
    }
}

// Battle commands -------------------------------------------------------------
void handleBattleCommand(FILE *output, char *second_word, char *current_line,
                         PokemonGo pokemonGo, PokemonGoResult *error_code) {
    if (strcmp(second_word, "fight") == 0) {
        executePokemonGoTrainerBattle(output, current_line, pokemonGo,
                                      error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) return;
    } else {
        *error_code = POKEMON_GO_INVALID_COMMAND_LINE_PARAMETERS;
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
    }
}

// Pokemon commands ------------------------------------------------------------
void
handlePokemonCommand(char *second_word, char *current_line, PokemonGo pokemonGo,
                     PokemonGoResult *error_code) {
    if (strcmp(second_word, "train") == 0) {
        executePokemonGoTrainerTrainPokemon(current_line, pokemonGo,
                                            error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) return;
    } else if (strcmp(second_word, "heal") == 0) {
        executePokemonGoTrainerHealPokemon(current_line, pokemonGo, error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) return;
    } else {
        *error_code = POKEMON_GO_INVALID_COMMAND_LINE_PARAMETERS;
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
    }
}


// Report commands -------------------------------------------------------------
void handleReportCommand(FILE *output, char *second_word, char *current_line,
                         PokemonGo pokemonGo, PokemonGoResult *error_code) {
    if (strcmp(second_word, "locations") == 0) {
        executePokemonGoReportLocations(output, current_line, pokemonGo,
                                        error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) return;
    } else if (strcmp(second_word, "stock") == 0) {
        executePokemonGoReportStock(output, current_line, pokemonGo,
                                    error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) return;
    } else if (strcmp(second_word, "trainer") == 0) {
        executePokemonGoReportTrainer(output, current_line, pokemonGo,
                                      error_code);
    } else {
        *error_code = POKEMON_GO_INVALID_COMMAND_LINE_PARAMETERS;
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
    }
}

// ------------------------ Command Analysis Functions -------------------------

void analyzeCurrentCommand(FILE *output, char *first_word, char *second_word,
                           char *current_line, PokemonGo pokemonGo,
                           PokemonGoResult *error_code) {
    if (strcmp(first_word, "trainer") == 0) {
        handleTrainerCommand(output, second_word, current_line, pokemonGo,
                             error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) {
            return;
        }
    } else if (strcmp(first_word, "store") == 0) {
        handleStoreCommand(second_word, current_line, pokemonGo,
                           error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) {
            return;
        }
    } else if (strcmp(first_word, "battle") == 0) {
        handleBattleCommand(output, second_word, current_line, pokemonGo,
                            error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) {
            return;
        }
    } else if (strcmp(first_word, "pokemon") == 0) {
        handlePokemonCommand(second_word, current_line, pokemonGo,
                             error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) {
            return;
        }
    } else if (strcmp(first_word, "report") == 0) {
        handleReportCommand(output, second_word, current_line, pokemonGo,
                            error_code);
        if (*error_code == POKEMON_GO_OUT_OF_MEM) {
            return;
        }
    } else {
        *error_code = POKEMON_GO_INVALID_COMMAND_LINE_PARAMETERS;
    }
}
/*----------------------------------------------------------------------------*/
void handleCurrentCommand(FILE *output, char *current_command,
                          PokemonGo pokemonGo, MtmErrorCode *main_error_code) {
    PokemonGoResult error_code = POKEMON_GO_SUCCESS;
    char *first_word = NULL;
    char *second_word = NULL;
    first_word = strtok(current_command, DELIMITER);
    if (first_word == NULL) return;
    second_word = strtok(NULL, DELIMITER);
    analyzeCurrentCommand(output, first_word, second_word, current_command,
                          pokemonGo, &error_code);
    if (error_code == POKEMON_GO_OUT_OF_MEM) {
        *main_error_code = MTM_OUT_OF_MEMORY;
    }
    if (error_code == POKEMON_GO_INVALID_COMMAND_LINE_PARAMETERS) {
        *main_error_code = MTM_INVALID_COMMAND_LINE_PARAMETERS;
    }
}
/*----------------------------------------------------------------------------*/