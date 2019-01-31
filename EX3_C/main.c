#include "parser.h"

/*----------------------------------------------------------------------------*/
/**
 * Initialazing the input channel
 * @param argc - Number of argument of main program
 * @param argv - Main program's word arguments
 * @return
 */
static FILE *initInput(int argc, char **argv) {
    if ((argc == EXECUTE_COMMAND_OUTPUT_LENGTH ||
         argc == EXECUTE_COMMAND_INPUT_LENGTH)
        && strcmp(argv[EXECUTE_COMMAND_INPUT_POS], "-i") == 0) {
        return fopen(argv[EXECUTE_COMMAND_INPUT_POS + 1], READ_ONLY);
    }
    return stdin;
}
/*----------------------------------------------------------------------------*/
/**
 * Initialazing the output channel
 * @param argc - Number of argument of main program
 * @param argv - Main program's word arguments
 * @return
 */
static FILE *initOutput(int argc, char **argv) {
    if (argc == EXECUTE_COMMAND_OUTPUT_LENGTH &&
        strcmp(argv[EXECUTE_COMMAND_OUTPUT_POS], "-o") == 0) {
        return fopen(argv[EXECUTE_COMMAND_OUTPUT_POS + 1], WRITE);
    }
    return stdout;
}

//------------------------- Input Validity Check -------------------------------
/**
 * Check whether command's input/output flag arguments are valid
 * @param argc - Number of argument of main program
 * @param argv - Main program's word arguments
 * @return
 * MTM_INVALID_COMMAND_LINE_PARAMETERS - Invalid command line parameters
 * MTM_SUCCESS - Otherwise
 */
static MtmErrorCode checkInputOutputFlags(int argc, char **argv) {
    if (argc == EXECUTE_COMMAND_INPUT_LENGTH) {
        if ((strcmp(argv[EXECUTE_COMMAND_INPUT_POS], "-i")) != 0 &&
            strcmp(argv[EXECUTE_COMMAND_INPUT_POS], "-o") != 0) {
            return MTM_INVALID_COMMAND_LINE_PARAMETERS;
        }
    } else if (argc == EXECUTE_COMMAND_OUTPUT_LENGTH) {
        if ((strcmp(argv[EXECUTE_COMMAND_OUTPUT_POS], "-o")) != 0) {
            return MTM_INVALID_COMMAND_LINE_PARAMETERS;
        }
    }
    return MTM_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/**
 * Check whether command's file arguments are valid
 * @param argc - Number of argument of main program
 * @param argv - Main program's word arguments
 * @return
 * MTM_INVALID_COMMAND_LINE_PARAMETERS - Invalid command line parameters
 * MTM_SUCCESS - Otherwise
 */
static MtmErrorCode checkExecutionCommand(int argc, char **argv) {
    if (argc < MIN_EXECUTE_COMMAND_LENGTH ||
        argc > MAX_EXECUTE_COMMAND_LENGTH ||
        argc == EXECUTE_COMMAND_INPUT_LENGTH - 1 ||
        argc == EXECUTE_COMMAND_OUTPUT_LENGTH - 1) {
        return MTM_INVALID_COMMAND_LINE_PARAMETERS;
    }
    if (strcmp(argv[POKEDEX_FLAG_POS], "-p") != 0)
        return MTM_INVALID_COMMAND_LINE_PARAMETERS;
    if (strcmp(argv[EVOLUTIONS_FLAG_POS], "-e") != 0)
        return MTM_INVALID_COMMAND_LINE_PARAMETERS;
    if (strcmp(argv[LOCATIONS_FLAG_POS], "-l") != 0)
        return MTM_INVALID_COMMAND_LINE_PARAMETERS;
    return checkInputOutputFlags(argc, argv);
}

/*----------------------------------------------------------------------------*/
/**
 * Returns a PokemonType {enum} which is suitable to a given type string.
 * @param type - A string type
 * @return PokemonType suitable to char* type
 */
static PokemonType stringToType(char *type) {
    if (strcmp(type, "NORMAL") == 0) return TYPE_NORMAL;
    if (strcmp(type, "FIRE") == 0) return TYPE_FIRE;
    if (strcmp(type, "WATER") == 0) return TYPE_WATER;
    if (strcmp(type, "BUG") == 0) return TYPE_BUG;
    if (strcmp(type, "GRASS") == 0) return TYPE_GRASS;
    if (strcmp(type, "FLYING") == 0) return TYPE_FLYING;
    if (strcmp(type, "FAIRY") == 0) return TYPE_FAIRY;
    if (strcmp(type, "GHOST") == 0) return TYPE_GHOST;
    if (strcmp(type, "GROUND") == 0) return TYPE_GROUND;
    if (strcmp(type, "ICE") == 0) return TYPE_ICE;
    if (strcmp(type, "POISON") == 0) return TYPE_POISON;
    if (strcmp(type, "PSYCHIC") == 0) return TYPE_PSYCHIC;
    if (strcmp(type, "ROCK") == 0) return TYPE_ROCK;
    return TYPE_NORMAL;
}

/*----------------------------------------------------------------------------*/
/**
 * Fills the Pokedex map ADT of PokemonGo from a given text file.
 * @param input - Source file
 * @param pokemonGo - PokemonGo ADT
 * @return
 */
static MtmErrorCode fillPokedex(FILE *input, PokemonGo pokemonGo) {
    char *pokemon_name = NULL;
    int pokemon_cp;
    char *type = NULL;
    char current_line[MAX_INPUT_LEN];
    List types = listCreate(copyType, freeType);
    if (!types) return MTM_OUT_OF_MEMORY;
    while (fgets(current_line, MAX_INPUT_LEN, input) != NULL) {
        pokemon_name = strtok(current_line, DELIMITER);
        pokemon_cp = atoi(strtok(NULL, DELIMITER));
        type = strtok(NULL, DELIMITER);
        while (type != NULL) {
            PokemonType strType = stringToType(type);
            if (listInsertLast(types, &strType) == LIST_OUT_OF_MEMORY) {
                return MTM_OUT_OF_MEMORY;
            }
            type = strtok(NULL, DELIMITER);
        }
        PokemonResult res;
        Pokemon new_pokemon = pokemonCreate(pokemon_name, "", NO_EVOLUTION,
                                            pokemon_cp, types, &res);
        if (res == POKEMON_OUT_OF_MEM) return MTM_OUT_OF_MEMORY;
        mapPut(pokemonGoGetPokedex(pokemonGo), pokemon_name, new_pokemon);
        listClear(types);
        pokemonDestroy(new_pokemon);
    }
    listDestroy(types);
    return MTM_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/**
 * Updates the Pokemon's evolution field
 * in the Pokedex PokemonGo from a given text file.
 * @param input - Source file
 * @param pokemonGo - PokemonGo ADT
 * @return
 */
static MtmErrorCode fillEvolutions(FILE *input, PokemonGo pokemonGo) {
    char *pokemon_name = NULL;
    char *evolution_name = NULL;
    int evolution_level;
    char current_line[MAX_INPUT_LEN];
    while (fgets(current_line, MAX_INPUT_LEN, input) != NULL) {
        pokemon_name = strtok(current_line, DELIMITER);
        evolution_name = strtok(NULL, DELIMITER);
        evolution_level = atoi(strtok(NULL, DELIMITER));
        Pokemon pokemon = (Pokemon) mapGet(pokemonGoGetPokedex(pokemonGo),
                                           pokemon_name);
        if (!pokemon) {
            continue;
        }
        pokemonSetEvolutionName(pokemon, evolution_name);
        pokemonSetEvolutionLevel(pokemon, evolution_level);
    }
    return MTM_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/**
 * Checks whether a given text argument represents a pokemon from the Pokedex.
 * @param pokemonGo - PokemonGo ADT
 * @param arg - A pokemon name to be checked
 * @return
 * TRUE if char* arg is a Pokemon (appears on the Pokedex) ; FALSE otherwise.
 */
static bool isPokemon(PokemonGo pokemonGo, char *arg) {
    if ((Pokemon) mapGet(pokemonGoGetPokedex(pokemonGo), arg) != NULL) {
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------------*/
/**
 * Removes all semicolons from a given string.
 * @param str
 * @param semicolon
 */
static void removeChar(char *str, char semicolon) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != semicolon) dst++;
    }
    *dst = '\0';
}

/*----------------------------------------------------------------------------*/
/**
 * Returns a given string without any semicolons in it.
 * @param str
 * @return
 */
static char *ignoreSemiColon(char *str) {
    int len = strlen(str);
    if (str[len - 1] == ';') {
        removeChar(str, ';');
    }
    return str;
}

/*----------------------------------------------------------------------------*/
/**
 * Fills the Locations map ADT of PokemonGo from a given text file.
 * @param input - Source file
 * @param pokemonGo - PokemonGo ADT
 * @return
 */
static MtmErrorCode fillLocations(FILE *input, PokemonGo pokemonGo) {
    char *location_name = NULL;
    char *other_arg = NULL;
    char current_line[MAX_INPUT_LEN];
    while (fgets(current_line, MAX_INPUT_LEN, input) != NULL) {
        location_name = ignoreSemiColon(strtok(current_line, DELIMITER));
        if (pokemonGoAddLocation(pokemonGo, location_name) ==
            POKEMON_GO_OUT_OF_MEM) {
            return MTM_OUT_OF_MEMORY;
        }
        other_arg = strtok(NULL, DELIMITER);
        while (other_arg != NULL) {
            if (isPokemon(pokemonGo, ignoreSemiColon(other_arg))) {
                PokemonResult error = POKEMON_SUCCESS;
                Pokemon pokemon = pokemonCopy(
                        mapGet(pokemonGoGetPokedex(pokemonGo), other_arg),
                        &error);
                if (error == POKEMON_OUT_OF_MEM) return MTM_OUT_OF_MEMORY;
                locationAddPokemon(
                        mapGet(pokemonGoGetLocations(pokemonGo), location_name),
                        pokemon);
                pokemonDestroy(pokemon);
            } else {
                locationAddNearby(
                        mapGet(pokemonGoGetLocations(pokemonGo), location_name),
                        other_arg);
            }
            other_arg = strtok(NULL, DELIMITER);
        }
    }
    return MTM_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/**
 * Fills PokemonGo ADTS: Pokedex, evolutions and locations fro a given source
 * file.
 * @param pokemonGo - PokemonGo ADT
 * @param pokedex_file_name - file name of pokedex
 * @param evolutions_file_name - file name of evolutions
 * @param locations_file_name - file name of locations
 * @return
 */
static MtmErrorCode fillADTS(PokemonGo pokemonGo, char *pokedex_file_name,
                             char *evolutions_file_name,
                             char *locations_file_name) {
    FILE *pokedex_file = fopen(pokedex_file_name, READ_ONLY);
    if (!pokedex_file) {
        return MTM_CANNOT_OPEN_FILE;
    }
    if (fillPokedex(pokedex_file, pokemonGo) == MTM_OUT_OF_MEMORY)
        return MTM_OUT_OF_MEMORY;
    fclose(pokedex_file);
    FILE *evolutions_file = fopen(evolutions_file_name, READ_ONLY);
    if (!evolutions_file) {
        return MTM_CANNOT_OPEN_FILE;
    }
    if (fillEvolutions(evolutions_file, pokemonGo) == MTM_OUT_OF_MEMORY)
        return MTM_OUT_OF_MEMORY;
    fclose(evolutions_file);
    FILE *locations_file = fopen(locations_file_name, READ_ONLY);
    if (!locations_file) {
        return MTM_CANNOT_OPEN_FILE;
    }
    if (fillLocations(locations_file, pokemonGo) == MTM_OUT_OF_MEMORY)
        return MTM_OUT_OF_MEMORY;
    fclose(locations_file);
    return MTM_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/**
 * Prints a specific error message by its error code and free any allocated
 * memory of PokemonGo program.
 * @param pokemonGo
 * @param errorCode
 * @return
 */
static MtmErrorCode
printErrorAndExit(PokemonGo pokemonGo, MtmErrorCode errorCode) {
    mtmPrintErrorMessage(stderr, errorCode);
    pokemonGoDestroy(pokemonGo);
    return errorCode;
}

/*----------------------------------------------------------------------------*/
int main(int argc, char **argv) {
    PokemonGoResult pokemongo_result = POKEMON_GO_SUCCESS;
    PokemonGo pokemonGo = pokemonGoCreate(&pokemongo_result);

    /* checks for memory allocation failure*/
    if (!pokemonGo) {
        mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
        return MTM_OUT_OF_MEMORY;
    }

    /* checks for invalid command line parameters*/
    MtmErrorCode command_line_res = checkExecutionCommand(argc, argv);
    if (command_line_res != MTM_SUCCESS) {
        return printErrorAndExit(pokemonGo,
                                 MTM_INVALID_COMMAND_LINE_PARAMETERS);
    }

    /* fill ADTs according to txt file */
    MtmErrorCode fill_adts_res = fillADTS(pokemonGo, POKEDEX_FILE_NAME,
                                          EVOLUTIONS_FILE_NAME,
                                          LOCATIONS_FILE_NAME);
    if (fill_adts_res != MTM_SUCCESS) {
        return printErrorAndExit(pokemonGo, fill_adts_res);
    }

    /* open IO stream (standard IO / files) */
    FILE *input = initInput(argc, argv);
    if (!input) {
        return printErrorAndExit(pokemonGo, MTM_CANNOT_OPEN_FILE);
    }
    FILE *output = initOutput(argc, argv);
    if (!output) {
        if (input != stdin) {
            fclose(input);
        }
        return printErrorAndExit(pokemonGo, MTM_CANNOT_OPEN_FILE);
    }

    /*getting commands from stdin / file and executing the respective function*/
    char current_command[MAX_INPUT_LEN];
    MtmErrorCode error_code = MTM_SUCCESS;
    while (fgets(current_command, MAX_INPUT_LEN, input) != NULL) {
        handleCurrentCommand(output, current_command, pokemonGo, &error_code);
        if (error_code != MTM_SUCCESS) {
            if (output != stdout) {
                fclose(output);
            }
            return printErrorAndExit(pokemonGo, error_code);
        }
    }

    /*closing the input/output if they are files*/
    if (input != stdin) {
        fclose(input);
    }
    if (output != stdout) {
        fclose(output);
    }
    pokemonGoDestroy(pokemonGo);
    return 0;
}
/*----------------------------------------------------------------------------*/