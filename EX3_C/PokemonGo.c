#include "PokemonGo.h"
#include <string.h>
#include <stdlib.h>

//-------------------------- Defines & Declarations ----------------------------

#define MAX_NAME_SIZE 10
#define POTION_PRICE 1
#define CANDY_PRICE 2
#define PRIZE_10 10
#define PRIZE_20 20
#define PRIZE_30 30

struct pokemon_go_t {
    Map trainers_map;
    Map pokedex_map;
    Map locations_map;
    Store store;
};

//-------------------------- Auxiliary Functions -------------------------------

/****************************** Declarations **********************************/
static inline int ceilPos(double x) {
    return (x) - (int) (x) > 0 ? (int) (x + 1) : (int) (x);
}
/*----------------------------------------------------------------------------*/
static Trainer trainerGetByName(PokemonGo pokemonGo, char *name);
/*----------------------------------------------------------------------------*/
static Location locationGetByName(PokemonGo pokemonGo, char *name);
/*----------------------------------------------------------------------------*/
static bool isNearby(PokemonGo pokemonGo, char *curr_location, char *nearby);
/*----------------------------------------------------------------------------*/
static int pokemonPrize(Pokemon pokemon);
/*----------------------------------------------------------------------------*/
static PokemonGoResult pokemonGoTrainerHunt(PokemonGo pokemonGo,
                                            FILE *output_channel, char *name);
/*----------------------------------------------------------------------------*/
static int itemPrice(char *item_type);
/*----------------------------------------------------------------------------*/
static MapKeyElement stringCopy(MapKeyElement src);
/*----------------------------------------------------------------------------*/
static int stringCompare(MapKeyElement dest, MapKeyElement src);
/*----------------------------------------------------------------------------*/
static void stringDestroy(MapKeyElement str);
/*----------------------------------------------------------------------------*/
static MapDataElement trainerMapCopy(MapDataElement trainer);
/*----------------------------------------------------------------------------*/
static MapDataElement pokemonMapCopy(MapDataElement pokemon);
/*----------------------------------------------------------------------------*/
static void trainerMapDestroy(MapDataElement trainer);
/*----------------------------------------------------------------------------*/
static void pokemonMapDestroy(MapDataElement pokemon);
/*----------------------------------------------------------------------------*/
static void locationMapDestroy(MapDataElement location);
/*----------------------------------------------------------------------------*/
static int itemSortByAscendingValue(ListElement item1, ListElement item2);
/*----------------------------------------------------------------------------*/
static double calcDelta(double trainer_exp, int pokemon_cp, double pokemon_hp);
/*----------------------------------------------------------------------------*/
static void pokemonGoCheckEvolve(PokemonGo pokemonGo, Pokemon pokemon);
/*----------------------------------------------------------------------------*/
static PokemonGoResult sortAndPrintItemList(List items_list,
                                            FILE *output_channel,
                                            char *item_type);
/*----------------------------------------------------------------------------*/
static void printPokemonsOfTrainer(Map pokemons, FILE *output_channel);
/*----------------------------------------------------------------------------*/
static void handleBattleArgs(PokemonGo pokemonGo, FILE *output_channel,
                             Pokemon pokemon1, Pokemon pokemon2,
                             Trainer trainer1, Trainer trainer2,
                             double old_hp_1, double old_hp_2, int old_level_1,
                             int old_level_2, int cp_1, int cp_2,
                             int new_level_1,
                             int new_level_2, double new_hp_1, double new_hp_2,
                             double old_xp_1, double old_xp_2, double new_xp_1,
                             double new_xp_2,
                             int id1, int id2);

/*----------------------------------------------------------------------------*/
static void executeBattle(PokemonGo pokemonGo, FILE *output_channel,
                          Trainer trainer1, int id1, Trainer trainer2, int id2);
/**************************** End of declarations *****************************/

/**
 * Returns a Trainer by a given name
 * @param pokemonGo - PokemonGo ADT
 * @param name - Trainer's name
 * @return - A trainer with the relevant name
 */
static Trainer trainerGetByName(PokemonGo pokemonGo, char *name) {
    Trainer trainer = mapGet(pokemonGo->trainers_map, name);
    if (!trainer) {
        return NULL;
    }
    return trainer;
}
/*----------------------------------------------------------------------------*/
/**
 * Returns a Location by a given name
 * @param pokemonGo - PokemonGo ADT
 * @param name - Location's name
 * @return - A Location with the relevant name
 */
static Location locationGetByName(PokemonGo pokemonGo, char *name) {
    Location location = mapGet(pokemonGo->locations_map, name);
    if (!location) {
        return NULL;
    }
    return location;
}
/*----------------------------------------------------------------------------*/
/**
 * Returns TRUE if destination location is nearby to a given location
 * @param pokemonGo - PokemonGo ADT
 * @param curr_location - The location to check if dest is near to
 * @param dest - Destination's location
 * @return - TRUE if the two locations are nearby, FALSE otherwise
 */
static bool isNearby(PokemonGo pokemonGo, char *curr_location, char *dest) {
    Location location = locationGetByName(pokemonGo, curr_location);
    List nearby_locations = locationGetNearby(location);
    LIST_FOREACH(char*, nearby_iterator, nearby_locations) {
        if (strcmp(nearby_iterator, dest) == 0) {
            return true;
        }
    }
    return false;
}
/*----------------------------------------------------------------------------*/
/**
 * Calculates Pokemon's catching bonus according to its type
 * @param pokemon - The pokemon to analyze its bonus
 * @return A given Pokemon's additional pokemon coins to be added after
 * catching it.
 */
static int pokemonPrize(Pokemon pokemon) {
    int max_prize = PRIZE_10;
    LIST_FOREACH(PokemonType*, type_iterator, pokemonGetTypes(pokemon)) {
        if (*type_iterator == TYPE_FIRE || *type_iterator == TYPE_POISON ||
            *type_iterator == TYPE_FLYING) {
            max_prize = PRIZE_30;
        } else if (*type_iterator == TYPE_ELECTRIC ||
                   *type_iterator == TYPE_FAIRY ||
                   *type_iterator == TYPE_ROCK || *type_iterator == TYPE_ICE ||
                   *type_iterator == TYPE_WATER) {
            max_prize = PRIZE_20;
        }
    }
    return max_prize;
}
/*----------------------------------------------------------------------------*/
/**
 * Prints trainer's catch result in his current location and add the pokemon
 * to his pokemons list whether catching has succeeded.
 * @param pokemonGo - PokemonGo ADT
 * @param output_channel - Output channel
 * @param name - Trainer's name
 * @return
 */
static PokemonGoResult pokemonGoTrainerHunt(PokemonGo pokemonGo,
                                            FILE *output_channel, char *name) {
    if (!pokemonGo || !name) {
        return POKEMON_GO_NULL_ARG;
    }
    Trainer trainer = trainerGetByName(pokemonGo, name);
    Location location = locationGetByName(pokemonGo,
                                          trainerGetLocation(trainer));
    Pokemon pokemon = locationGetFirstPokemon(location);
    mtmPrintCatchResult(output_channel, name, pokemonGetName(pokemon),
                        locationGetName(location));
    if (!pokemon) {
        return POKEMON_GO_SUCCESS;
    }
    trainerSetPokecoins(trainer,
                        trainerGetPokecoins(trainer) + pokemonPrize(pokemon));
    if (trainerAddPokemon(trainer, pokemon) == TRAINER_OUT_OF_MEM) {
        return POKEMON_GO_OUT_OF_MEM;
    }
    locationRemoveFirstPokemon(location);
    return POKEMON_GO_SUCCESS;
}
/*----------------------------------------------------------------------------*/
/**
 * Return an item's price in store according to its type.
 * @param item_type - A candy / A potion.
 * @return Item's price according to its type.
 */
static int itemPrice(char *item_type) {
    return strcmp(item_type, "candy") == 0 ? CANDY_PRICE : POTION_PRICE;
}
/*----------------------------------------------------------------------------*/
/**
 * String copy function to be used for PokemonGo Sub-ADTS.
 * @param src - Source string to be copied
 * @return - A copy of the string.
 *           NULL in case of memory allocation failure or a NULL parameter.
 */
static MapKeyElement stringCopy(MapKeyElement src) {
    if (!src) {
        return NULL;
    }
    char *dest = malloc(strlen((char *) src) + 1);
    if (!dest) {
        return NULL;
    }
    strcpy(dest, (char *) src);
    return dest;
}
/*----------------------------------------------------------------------------*/
/**
 * Comparsion function for two strings (dest,src)
 * @param dest - Argument 1
 * @param src - Argument 2
 * @return -
 * stringCompare(char* dest, char* src) ==  0     ======>     dest = src
 * stringCompare(char* dest, char* src) ==  > 0   ======>     dest > src
 * stringCompare(char* dest, char* src) ==  < 0   ======>     dest < src
 */
static int stringCompare(MapKeyElement dest, MapKeyElement src) {
    int cmp_res = strcmp((char *) dest, (char *) src);
    if (cmp_res > 0) {
        return 1;
    } else if (cmp_res < 0) {
        return -1;
    } else {
        return 0;
    }
}
/*----------------------------------------------------------------------------*/
/**
 * Memory deallocation function for a given string.
 * @param str
 */
static void stringDestroy(MapKeyElement str) {
    free(str);
}
/*----------------------------------------------------------------------------*/
/**
 * Copying function used for the Map structure
 * @param trainer - A Trainer element to be copied
 * @return A newlly allocated exact copy of trainer.
 *         NULL in case of NULL was sent as an argument.
 */
static MapDataElement trainerMapCopy(MapDataElement trainer) {
    if (!trainer) {
        return NULL;
    }
    TrainerResult error;
    return trainerCopy((Trainer) trainer, &error);
}
/*----------------------------------------------------------------------------*/
/**
 * Copying function used for the Map structure
 * @param trainer - A Pokemon element to be copied
 * @return A newlly allocated exact copy of
 *         NULL in case of NULL was sent as an argument.
 */
static MapDataElement pokemonMapCopy(MapDataElement pokemon) {
    if (!pokemon) {
        return NULL;
    }
    PokemonResult error;
    return pokemonCopy((Pokemon) pokemon, &error);
}
/*----------------------------------------------------------------------------*/
/**
 * Copying function used for the Map structure
 * @param trainer - A Location element to be copied
 * @return A newlly allocated exact copy of location
 *         NULL in case of NULL was sent as an argument.
 */
static MapDataElement locationMapCopy(MapDataElement location) {
    if (!location) {
        return NULL;
    }
    LocationResult error;
    return locationCopy((Location) location, &error);
}
/*----------------------------------------------------------------------------*/
/**
 * Memory deallocation function used for the Map structure
 * @param trainer - A Trainer element to be deallocated
 */
static void trainerMapDestroy(MapDataElement trainer) {
    if (!trainer) return;
    trainerDestroy((Trainer) trainer);
}
/*----------------------------------------------------------------------------*/
/**
 * Memory deallocation function used for the Map structure
 * @param pokemon - A Pokemon element to be deallocated
 */
static void pokemonMapDestroy(MapDataElement pokemon) {
    if (!pokemon) return;
    pokemonDestroy((Pokemon) pokemon);
}
/*----------------------------------------------------------------------------*/
/**
 * Memory deallocation function used for the Map structure
 * @param location - A Location element to be deallocated
 */
static void locationMapDestroy(MapDataElement location) {
    if (!location) return;
    locationDestroy((Location) location);
}
/*----------------------------------------------------------------------------*/
/**
 * A function used to compare two Items by their value.
 * @param item1 - First item to be compared
 * @param item2  - Second item to be compared
 * @return
 * itemSortByAscendingValue(item1,item2) ==  0  ====>  val(item1)=val(item2)
 * itemSortByAscendingValue(item1,item2) ==  1  ====>  val(item1)>val(item2)
 * itemSortByAscendingValue(item1,item2) ==  -1  ====>  val(item1)<val(item2)
 */
static int itemSortByAscendingValue(ListElement item1, ListElement item2) {
    int value1 = itemGetValue((Item) item1);
    int value2 = itemGetValue((Item) item2);
    if (value2 > value1) {
        return -1;
    } else if (value2 < value1) {
        return 1;
    } else {
        return 0;
    }
}
/*----------------------------------------------------------------------------*/
/**
 * A function used for pokemons' battle calculations.
 * @param trainer_exp - A given trainer exp
 * @param pokemon_cp - A given pokemon combat power
 * @param pokemon_hp - A given pokemon health points
 * @return A factor used by pokemon's battle procedures.
 */
static double calcDelta(double trainer_exp, int pokemon_cp, double pokemon_hp) {
    return (trainer_exp * pokemon_cp * (1.0 + (pokemon_hp / 200.0)));
}
/*----------------------------------------------------------------------------*/
/**
 * A function that checks if a given Pokemon has to be evolved according to its
 * current level.
 * @param pokemonGo - PokemonGo ADT
 * @param pokemon - A given Pokemon to check its evolution
 */
static void pokemonGoCheckEvolve(PokemonGo pokemonGo, Pokemon pokemon) {
    if (!pokemon) {
        return;
    }
    if (strlen(pokemonGetEvolutionName(pokemon)) == 0) {
        return;
    }
    while ((pokemonGetLevel(pokemon) >= pokemonGetEvolutionLevel(pokemon)) &&
           pokemonGetEvolutionLevel(pokemon) != NO_EVOLUTION) {
        pokemonGoEvolve(pokemonGo, pokemon);
    }
}
/*----------------------------------------------------------------------------*/
/**
 * A function that sorted a list of items by a given criteria.
 * @param items_list - The list of items to print.
 * @param output_channel - Output channel
 * @param item_type - A "candy" or a "potion".
 * @return
 */
static PokemonGoResult sortAndPrintItemList(List items_list,
                                            FILE *output_channel,
                                            char *item_type) {
    List sorted_items = listCopy(items_list);
    if (!sorted_items) return POKEMON_GO_OUT_OF_MEM;
    ListResult sort_res = listSort(sorted_items, itemSortByAscendingValue);
    if (sort_res == LIST_NULL_ARGUMENT) return POKEMON_GO_NULL_ARG;
    if (sort_res == LIST_OUT_OF_MEMORY) return POKEMON_GO_OUT_OF_MEM;
    LIST_FOREACH (Item, item_iterator, sorted_items) {
        mtmPrintItem(output_channel, item_type, itemGetValue(item_iterator),
                     itemGetQuantity(item_iterator));
    }
    listDestroy(sorted_items);
    return POKEMON_GO_SUCCESS;
}
/*----------------------------------------------------------------------------*/
/**
 * This function iterates through a given Trainer's Pokemons list and prints
 * each one of his Pokemons
 * @param pokemons - A given Trainer's list of Pokemons
 * @param output_channel - Output channel
 */
static void printPokemonsOfTrainer(Map pokemons, FILE *output_channel) {
    mtmPrintPokemonsHeaderForTrainer(output_channel);
    int cp, level;
    double hp;
    char *species;
    MAP_FOREACH(int*, pokemon_iterator, pokemons) {
        hp = pokemonGetHp(mapGet(pokemons, pokemon_iterator));
        level = pokemonGetLevel(mapGet(pokemons, pokemon_iterator));
        cp = pokemonGetCp(mapGet(pokemons, pokemon_iterator));
        species = pokemonGetName(mapGet(pokemons, pokemon_iterator));
        mtmPrintPokemon(output_channel, *pokemon_iterator, species, hp, cp,
                        level);
    }
}
/*----------------------------------------------------------------------------*/
/**
 * A function used for updating the two fighting Pokemons stats and evaluate
 * their status and evolution. Afterwards, the battle's result is printed to
 * the output channel.
 * @param pokemonGo - PokemonGo ADT
 * @param output_channel - Output chanel
 * @param pokemon1 - First pokemon in the battle
 * @param pokemon2 - Second pokemon in the battle
 * @param trainer1 - First trainer in the battle
 * @param trainer2 - Second trainer in the battle
 * @param old_hp_1 - First pokemon's previous HP (before battle)
 * @param old_hp_2 - Second pokemon's previous HP (before battle)
 * @param old_level_1 - First pokemon's previous level (before battle)
 * @param old_level_2 - Second pokemon's previous level (before battle)
 * @param cp_1 - First pokemon's CP
 * @param cp_2 - Second pokemon's CP
 * @param new_level_1 - First pokemon's new level (after battle)
 * @param new_level_2 - Second pokemon's new level (after battle)
 * @param new_hp_1 - First pokemon's new HP (after battle)
 * @param new_hp_2 - Second pokemon's new HP (after battle)
 * @param old_xp_1 - First trainer's previous EXP (before battle)
 * @param old_xp_2 - Second trainer's previous EXP (before battle)
 * @param new_xp_1 - First trainer's new EXP (after battle)
 * @param new_xp_2 - Second trainer's new EXP (after battle)
 * @param id1 - First pokemon's ID
 * @param id2 - Second pokemon's ID
 */
static void handleBattleArgs(PokemonGo pokemonGo, FILE *output_channel,
                             Pokemon pokemon1, Pokemon pokemon2,
                             Trainer trainer1, Trainer trainer2,
                             double old_hp_1, double old_hp_2, int old_level_1,
                             int old_level_2, int cp_1, int cp_2,
                             int new_level_1,
                             int new_level_2, double new_hp_1, double new_hp_2,
                             double old_xp_1, double old_xp_2, double new_xp_1,
                             double new_xp_2,
                             int id1, int id2) {
    char pokemon_1_name[MAX_NAME_SIZE + 1];
    char pokemon_2_name[MAX_NAME_SIZE + 1];
    strcpy(pokemon_1_name, pokemonGetName(pokemon1));
    strcpy(pokemon_2_name, pokemonGetName(pokemon2));
    pokemonSetLevel(pokemon1, new_level_1);
    pokemonSetLevel(pokemon2, new_level_2);
    pokemonSetHp(pokemon1, new_hp_1);
    pokemonSetHp(pokemon2, new_hp_2);
    trainerSetExp(trainer1, new_xp_1);
    trainerSetExp(trainer2, new_xp_2);
    bool is_dead_1 = false;
    bool is_dead_2 = false;
    if (pokemonGetHp(pokemon1) <= MIN_HP) {
        is_dead_1 = true;
        trainerRemovePokemon(trainer1, id1);
    }
    if (pokemonGetHp(pokemon2) <= MIN_HP) {
        is_dead_2 = true;
        trainerRemovePokemon(trainer2, id2);
    }
    if (!is_dead_1) {
        pokemonGoCheckEvolve(pokemonGo, pokemon1);
    }
    if (!is_dead_2) {
        pokemonGoCheckEvolve(pokemonGo, pokemon2);
    }
    mtmPrintBattle(output_channel, trainerGetName(trainer1),
                   trainerGetName(trainer2), pokemon_1_name, pokemon_2_name,
                   cp_1, cp_2, old_hp_1, old_hp_2, new_hp_1, new_hp_2,
                   old_level_1, old_level_2, new_level_1, new_level_2,
                   old_xp_1, old_xp_2, new_xp_1, new_xp_2, is_dead_1,
                   is_dead_2);
}
/*----------------------------------------------------------------------------*/
/**
 * A function used for initiating a fight between two Pokemons of two different
 * Trainers.
 * @param pokemonGo - PokemonGo ADT
 * @param output_channel - Output channel
 * @param trainer1 - First trainer in the battle
 * @param id1 - First trainer's Pokemon ID number
 * @param trainer2 - Second trainer in the battle
 * @param id2 - Second trainer's Pokemon ID number
 */
static void executeBattle(PokemonGo pokemonGo, FILE *output_channel,
                          Trainer trainer1, int id1, Trainer trainer2,
                          int id2) {
    Pokemon pokemon1 = trainerGetPokemonByID(trainer1, id1);
    Pokemon pokemon2 = trainerGetPokemonByID(trainer2, id2);
    double old_hp_1 = pokemonGetHp(pokemon1);
    double old_hp_2 = pokemonGetHp(pokemon2);
    int old_level_1 = pokemonGetLevel(pokemon1);
    int old_level_2 = pokemonGetLevel(pokemon2);
    int cp_1 = pokemonGetCp(pokemon1);
    int cp_2 = pokemonGetCp(pokemon2);
    int new_level_1 = old_level_1 + ceilPos((old_level_1 + old_level_2) / 4.0);
    int new_level_2 = old_level_2 + ceilPos((old_level_2 + old_level_1) / 4.0);
    double delta_1 = calcDelta(trainerGetExp(trainer2), pokemonGetCp(pokemon2),
                               pokemonGetHp(pokemon2));
    double delta_2 = calcDelta(trainerGetExp(trainer1), pokemonGetCp(pokemon1),
                               pokemonGetHp(pokemon1));
    double new_hp_1 = pokemonGetHp(pokemon1) - delta_1;
    double new_hp_2 = pokemonGetHp(pokemon2) - delta_2;
    new_hp_1 = new_hp_1 < MIN_HP ? MIN_HP : new_hp_1;
    new_hp_2 = new_hp_2 < MIN_HP ? MIN_HP : new_hp_2;
    double old_xp_1 = trainerGetExp(trainer1);
    double old_xp_2 = trainerGetExp(trainer2);
    double new_xp_1 = old_xp_1 + (delta_2 / 10.0);
    double new_xp_2 = old_xp_2 + (delta_1 / 10.0);
    handleBattleArgs(pokemonGo, output_channel, pokemon1, pokemon2, trainer1,
                     trainer2, old_hp_1, old_hp_2, old_level_1, old_level_2,
                     cp_1, cp_2, new_level_1, new_level_2, new_hp_1, new_hp_2,
                     old_xp_1, old_xp_2, new_xp_1, new_xp_2, id1, id2);
}

//-------------------------- Main Functions ----------------------------------//

PokemonGo pokemonGoCreate(PokemonGoResult *error) {
    *error = POKEMON_GO_OUT_OF_MEM;
    PokemonGo pokemonGo = malloc(sizeof(*pokemonGo));
    if (!pokemonGo) return NULL;
    pokemonGo->trainers_map = mapCreate(stringCopy, trainerMapCopy,
                                        stringDestroy, trainerMapDestroy,
                                        stringCompare);
    if (!pokemonGo->trainers_map) {
        pokemonGoDestroy(pokemonGo);
        return NULL;
    }
    pokemonGo->pokedex_map = mapCreate(stringCopy, pokemonMapCopy,
                                       stringDestroy, pokemonMapDestroy,
                                       stringCompare);
    if (!pokemonGo->pokedex_map) {
        pokemonGoDestroy(pokemonGo);
        return NULL;
    }
    pokemonGo->locations_map = mapCreate(stringCopy, locationMapCopy,
                                         stringDestroy, locationMapDestroy,
                                         stringCompare);
    if (!pokemonGo->locations_map) {
        pokemonGoDestroy(pokemonGo);
        return NULL;
    }
    StoreResult store_error;
    pokemonGo->store = storeCreate(&store_error);
    if (!pokemonGo->store) {
        pokemonGoDestroy(pokemonGo);
        return NULL;
    }
    *error = POKEMON_GO_SUCCESS;
    return pokemonGo;
}
/*----------------------------------------------------------------------------*/
PokemonGo pokemonGoCopy(PokemonGo pokemonGo, PokemonGoResult* error) {
    *error = POKEMON_OUT_OF_MEM;
    if (!pokemonGo) {
        *error = POKEMON_GO_NULL_ARG;
        return NULL;
    }
    PokemonGo copy = pokemonGoCreate(error);
    if (*error == POKEMON_GO_OUT_OF_MEM) {
        return NULL;
    }
    copy->pokedex_map = mapCopy(pokemonGo->pokedex_map);
    if (!copy->pokedex_map) return NULL;
    copy->locations_map = mapCopy(pokemonGo->locations_map);
    if (!copy->locations_map) return NULL;
    copy->trainers_map = mapCopy(pokemonGo->trainers_map);
    if (!copy->trainers_map) return NULL;
    StoreResult store_error;
    copy->store = storeCopy(pokemonGo->store, &store_error);
    if (!copy->store) return NULL;
    *error = POKEMON_GO_SUCCESS;
    return copy;
}
/*----------------------------------------------------------------------------*/
void pokemonGoDestroy(PokemonGo pokemonGo) {
    mapDestroy(pokemonGo->trainers_map);
    mapDestroy(pokemonGo->locations_map);
    mapDestroy(pokemonGo->pokedex_map);
    storeDestroy(pokemonGo->store);
    free(pokemonGo);
}
/*----------------------------------------------------------------------------*/
Map pokemonGoGetPokedex(PokemonGo pokemonGo) {
    return pokemonGo->pokedex_map;
}
/*----------------------------------------------------------------------------*/
Map pokemonGoGetLocations(PokemonGo pokemonGo) {
    return pokemonGo->locations_map;
}
/*----------------------------------------------------------------------------*/
PokemonGoResult pokemonGoAddLocation(PokemonGo pokemonGo, char *name) {
    if (!pokemonGo || !name) {
        return POKEMON_GO_NULL_ARG;
    }
    if (strlen(name) == 0) {
        return POKEMON_GO_INVALID_ARG;
    }
    LocationResult error;
    Location location = locationCreate(name, &error);
    if (error == LOCATION_OUT_OF_MEM) return POKEMON_GO_OUT_OF_MEM;
    if (mapPut(pokemonGo->locations_map, name, location) == MAP_OUT_OF_MEMORY) {
        return POKEMON_GO_OUT_OF_MEM;
    }
    locationDestroy(location);
    return POKEMON_GO_SUCCESS;
}
//-------------------------- Store Functions -----------------------------------

PokemonGoResult pokemonGoAddItemsToStore(PokemonGo pokemonGo,
                                         char *item, int value, int quantity) {
    if (!pokemonGo || !item) {
        return POKEMON_GO_NULL_ARG;
    }
    if ((strcmp(item, "candy") != 0 && strcmp(item, "potion") != 0) || value <= 0 ||
        quantity <= 0) {
        return POKEMON_GO_INVALID_ARG;
    }
    if (storeAddItems(pokemonGo->store, item, value, quantity) ==
        STORE_OUT_OF_MEM) {
        return POKEMON_GO_OUT_OF_MEM;
    }
    return POKEMON_GO_SUCCESS;
}

//-------------------------- Pokemon Functions ---------------------------------

PokemonGoResult pokemonGoEvolve(PokemonGo pokemonGo, Pokemon pokemon) {
    Pokemon evolution = mapGet(pokemonGo->pokedex_map,
                               pokemonGetEvolutionName(pokemon));
    int bonus_cp = pokemonGetBonusCp(pokemon);
    int level = pokemonGetLevel(pokemon);
    pokemonSetName(pokemon, pokemonGetName(evolution));
    pokemonSetEvolutionLevel(pokemon, pokemonGetEvolutionLevel(evolution));
    pokemonSetEvolutionName(pokemon, pokemonGetEvolutionName(evolution));
    pokemonSetTypes(pokemon, listCopy(pokemonGetTypes(pokemon)));
    pokemonSetCp(pokemon, pokemonGetCp(evolution));
    pokemonAddCp(pokemon, bonus_cp);
    pokemonSetLevel(pokemon, level);
    return POKEMON_GO_SUCCESS;
}

//-------------------------- Trainer Functions ---------------------------------

PokemonGoResult pokemonGoAddTrainer(PokemonGo pokemonGo, FILE *output_channel,
                                    char *name, int budget, char *start_point) {
    if (!pokemonGo || !name || !start_point) {
        return POKEMON_GO_NULL_ARG;
    }
    if (budget < 0 || strlen(name) == 0 || strlen(start_point) == 0) {
        return POKEMON_GO_INVALID_ARG;
    }
    if (mapContains(pokemonGo->trainers_map, name)) {
        return POKEMON_GO_TRAINER_NAME_ALREADY_EXISTS;
    }
    Location starting_location = locationGetByName(pokemonGo, start_point);
    if (!starting_location) {
        return POKEMON_GO_LOCATION_DOES_NOT_EXIST;
    }
    TrainerResult error = TRAINER_SUCCESS;
    Trainer trainer = trainerCreate(name, budget, start_point, &error);
    if (!trainer) {
        return POKEMON_GO_OUT_OF_MEM;
    }
    if (mapPut(pokemonGo->trainers_map, name, trainer) == MAP_OUT_OF_MEMORY) {
        return POKEMON_GO_OUT_OF_MEM;
    }
    trainerDestroy(trainer);
    pokemonGoTrainerHunt(pokemonGo, output_channel, name);
    return POKEMON_GO_SUCCESS;
}
/*----------------------------------------------------------------------------*/
PokemonGoResult pokemonGoTrainerMoveHunt(PokemonGo pokemonGo,
                                         FILE *output_channel,
                                         char *name, char *go_to) {
    if (!pokemonGo || !name || !go_to) {
        return POKEMON_GO_NULL_ARG;
    }
    Trainer trainer = trainerGetByName(pokemonGo, name);
    if (!trainer) {
        return POKEMON_GO_TRAINER_DOES_NOT_EXIST;
    }
    Location location = locationGetByName(pokemonGo, go_to);
    if (!location) {
        return POKEMON_GO_LOCATION_DOES_NOT_EXIST;
    }
    if (strcmp((trainerGetLocation(trainer)), go_to) == 0) {
        return POKEMON_GO_TRAINER_ALREADY_IN_LOCATION;
    }
    if (!isNearby(pokemonGo, trainerGetLocation(trainer), go_to)) {
        return POKEMON_GO_LOCATION_IS_NOT_REACHABLE;
    }
    trainerSetLocation(trainer, go_to);
    pokemonGoTrainerHunt(pokemonGo, output_channel, name);
    return POKEMON_GO_SUCCESS;
}
/*----------------------------------------------------------------------------*/
PokemonGoResult pokemonGoTrainerPurchase(PokemonGo pokemonGo,
                                         char *name, char *item, int value) {
    if (!pokemonGo || !name || !item) {
        return POKEMON_GO_NULL_ARG;
    }
    if ((strcmp(item, "candy") != 0 && strcmp(item, "potion") != 0) || value <= 0) {
        return POKEMON_GO_INVALID_ARG;
    }
    Trainer trainer = mapGet(pokemonGo->trainers_map, name);
    if (!trainer) {
        return POKEMON_GO_TRAINER_DOES_NOT_EXIST;
    }
    int price = itemPrice(item) * value;
    if (trainerGetPokecoins(trainer) < price) {
        return POKEMON_GO_BUDGET_IS_NOT_SUFFICIENT;
    }
    if (storeRemoveItem(pokemonGo->store, item, value) == STORE_OUT_OF_STOCK) {
        return POKEMON_GO_ITEM_OUT_OF_STOCK;
    }
    storeAddItems(trainerGetItems(trainer), item, value, 1);
    trainerSetPokecoins(trainer, trainerGetPokecoins(trainer) - price);
    return POKEMON_GO_SUCCESS;
}
/*----------------------------------------------------------------------------*/
PokemonGoResult
pokemonGoTrainerBattle(PokemonGo pokemonGo, FILE *output_channel,
                       char *trainer1_name, int id1,
                       char *trainer2_name, int id2) {
    if (!trainer1_name || !trainer2_name) return POKEMON_GO_NULL_ARG;
    if (id1 <= 0 || id2 <= 0 || strcmp(trainer1_name, trainer2_name) == 0) {
        return POKEMON_GO_INVALID_ARG;
    }
    if (!mapContains(pokemonGo->trainers_map, trainer1_name) &&
        !mapContains(pokemonGo->trainers_map, trainer2_name)) {
        return POKEMON_GO_TRAINER_DOES_NOT_EXIST;
    }
    Trainer trainer1 = mapGet(pokemonGo->trainers_map, trainer1_name);
    Trainer trainer2 = mapGet(pokemonGo->trainers_map, trainer2_name);
    if (!trainerPokemonExist(trainer1, id1) ||
        !trainerPokemonExist(trainer2, id2)) {
        return POKEMON_GO_POKEMON_DOES_NOT_EXIST;
    }
    executeBattle(pokemonGo, output_channel, trainer1, id1, trainer2, id2);
    return POKEMON_GO_SUCCESS;
}
/*----------------------------------------------------------------------------*/
PokemonGoResult pokemonGoTrainerHealPokemon(PokemonGo pokemonGo, char *name,
                                            int id) {
    if (!name) {
        return POKEMON_GO_NULL_ARG;
    }
    Trainer trainer = mapGet(pokemonGo->trainers_map, name);
    if (!trainer) {
        return POKEMON_GO_TRAINER_DOES_NOT_EXIST;
    }
    TrainerResult heal_result = trainerPokemonUsePotion(trainer, id);
    if (heal_result == TRAINER_POKEMON_DOES_NOT_EXIST) {
        return POKEMON_GO_POKEMON_DOES_NOT_EXIST;
    }
    if (heal_result == TRAINER_NO_AVAILABLE_ITEM_FOUND) {
        return POKEMON_GO_NO_AVAILABLE_ITEM_FOUND;
    }
    if (heal_result == TRAINER_POKEMON_HP_IS_AT_MAX) {
        return POKEMON_GO_HP_IS_AT_MAX;
    }
    return POKEMON_GO_SUCCESS;
}
/*----------------------------------------------------------------------------*/
PokemonGoResult
pokemonGoTrainerTrainPokemon(PokemonGo pokemonGo, char *name, int id) {
    if (!name) {
        return POKEMON_GO_NULL_ARG;
    }
    if (id < 1) {
        return POKEMON_GO_INVALID_ARG;
    }
    Trainer trainer = mapGet(pokemonGo->trainers_map, name);
    if (!trainer) {
        return POKEMON_GO_TRAINER_DOES_NOT_EXIST;
    }
    if (id <= 0 || !trainerPokemonExist(trainer, id)) {
        return POKEMON_GO_POKEMON_DOES_NOT_EXIST;
    }
    TrainerResult train_result = trainerPokemonUseCandy(trainer, id);
    if (train_result == TRAINER_NO_AVAILABLE_ITEM_FOUND)
        return POKEMON_GO_NO_AVAILABLE_ITEM_FOUND;
    if (train_result == TRAINER_POKEMON_DOES_NOT_EXIST)
        return POKEMON_GO_POKEMON_DOES_NOT_EXIST;
    return POKEMON_GO_SUCCESS;
}

//-------------------------- Report Functions ----------------------------------

PokemonGoResult
pokemonGoReportTrainer(PokemonGo pokemonGo, char *name, FILE *output_channel) {
    if (!pokemonGo || !name || !output_channel) return POKEMON_GO_NULL_ARG;
    if (strlen(name) == 0) return POKEMON_GO_INVALID_ARG;
    Trainer trainer = mapGet(pokemonGo->trainers_map, name);
    if (!trainer) return POKEMON_GO_TRAINER_DOES_NOT_EXIST;
    char *location = trainerGetLocation(trainer);
    int budget = trainerGetPokecoins(trainer);
    double exp = trainerGetExp(trainer);
    mtmPrintTrainerHeader(output_channel, name, location, budget, exp);
    mtmPrintItemsHeaderForTrainer(output_channel);
    List candies_ascending = storeGetCandies(trainerGetItems(trainer));
    PokemonGoResult sort_res1 = sortAndPrintItemList(candies_ascending,
                                                     output_channel, "candy");
    if (sort_res1 != POKEMON_GO_SUCCESS) return sort_res1;
    List potions_ascending = storeGetPotions(trainerGetItems(trainer));
    PokemonGoResult sort_res2 = sortAndPrintItemList(potions_ascending,
                                                     output_channel, "potion");
    if (sort_res2 != POKEMON_GO_SUCCESS) return sort_res2;
    printPokemonsOfTrainer(trainerGetPokemons(trainer), output_channel);
    return POKEMON_GO_SUCCESS;
}
/*----------------------------------------------------------------------------*/
PokemonGoResult pokemonGoReportLocations(PokemonGo pokemonGo,
                                         FILE *output_channel) {
    if (!pokemonGo || !output_channel) {
        return POKEMON_GO_NULL_ARG;
    }
    mtmPrintLocationsHeader(output_channel);
    char *first_pokemon = NULL;
    MAP_FOREACH (char*, location_iterator, pokemonGo->locations_map) {
        first_pokemon = pokemonGetName(locationGetFirstPokemon(
                locationGetByName(pokemonGo, location_iterator)));
        mtmPrintLocation(output_channel, location_iterator, first_pokemon);
    }
    return POKEMON_GO_SUCCESS;
}
/*----------------------------------------------------------------------------*/
PokemonGoResult pokemonGoReportStock(PokemonGo pokemonGo,
                                     FILE *output_channel) {
    if (!pokemonGo || !output_channel) {
        return POKEMON_GO_NULL_ARG;
    }
    mtmPrintStockHeader(output_channel);
    sortAndPrintItemList(storeGetCandies(pokemonGo->store), output_channel,
                         "candy");
    sortAndPrintItemList(storeGetPotions(pokemonGo->store), output_channel,
                         "potion");
    return POKEMON_GO_SUCCESS;
}