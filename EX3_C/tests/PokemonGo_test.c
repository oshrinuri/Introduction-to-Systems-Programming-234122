#include "../PokemonGo.h"
#include "test_utilities.h"
#include <stdlib.h>

static ListElement copyTypes(ListElement type) {
    PokemonType *copy = malloc(sizeof(PokemonType *));
    *copy = *(PokemonType *) type;
    return copy;
}

static void destroyTypes(ListElement type) {
    free(type);
}

static List listTypeCreate() {
    List types = listCreate(copyTypes, destroyTypes);
    return types;
}

static bool testPokemonGoCreate() {
    PokemonGoResult error;
    PokemonGo pokemonGo = pokemonGoCreate(&error);
    ASSERT_TEST(pokemonGo != NULL);
    ASSERT_TEST(error == POKEMON_GO_SUCCESS);
    pokemonGoDestroy(pokemonGo);
    return true;
}

static bool testPokemonGoDestroy() {
    PokemonGoResult error;
    PokemonGo pokemonGo = pokemonGoCreate(&error);
    ASSERT_TEST(error == POKEMON_GO_SUCCESS);
    pokemonGoDestroy(pokemonGo);
    return true;
}

static bool testPokemonGoGetPokedex() {
    PokemonGoResult error;
    PokemonGo pokemonGo = pokemonGoCreate(&error);
    ASSERT_TEST(error == POKEMON_GO_SUCCESS);
    Map pokedex = pokemonGoGetPokedex(pokemonGo);
    ASSERT_TEST(pokedex != NULL);
    pokemonGoDestroy(pokemonGo);
    return true;
}

static bool testPokemonGoAddTrainer() {
    PokemonGoResult error;
    PokemonGo pokemonGo = pokemonGoCreate(&error);
    PokemonResult pokemon_error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pikachu = pokemonCreate("Pikachu", "Raichu", 3, 1, types,
                                    &pokemon_error);
    ASSERT_TEST(pokemonGoAddLocation(pokemonGo, "bern") == POKEMON_GO_SUCCESS);
    Location bern = mapGet(pokemonGoGetLocations(pokemonGo), "bern");
    ASSERT_TEST(locationAddPokemon(bern, pikachu) == LOCATION_SUCCESS);
    ASSERT_TEST(error == POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, NULL, 50, "paris") ==
                POKEMON_GO_NULL_ARG);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, NULL, 50, NULL) ==
                POKEMON_GO_NULL_ARG);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Ash", 50, "") ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "", 50, "rome") ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Linda", -999, "bern") ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Ash", 50, "paris") ==
                POKEMON_GO_LOCATION_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Linda", 123, "bern") ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Linda", 123, "bern") ==
                POKEMON_GO_TRAINER_NAME_ALREADY_EXISTS);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Rafi", 123, "lima") ==
                POKEMON_GO_LOCATION_DOES_NOT_EXIST);
    listDestroy(types);
    pokemonDestroy(pikachu);
    pokemonGoDestroy(pokemonGo);
    return true;
}

static bool testPokemonGoTrainerMoveHunt() {
    PokemonGoResult error;
    PokemonGo pokemonGo = pokemonGoCreate(&error);
    PokemonResult pokemon_error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pikachu = pokemonCreate("Pikachu", "Raichu", 3, 1, types,
                                    &pokemon_error);
    ASSERT_TEST(pokemonGoAddLocation(pokemonGo, "bern") == POKEMON_GO_SUCCESS);
    Location bern = mapGet(pokemonGoGetLocations(pokemonGo), "bern");
    ASSERT_TEST(pokemonGoAddLocation(pokemonGo, "eilat") == POKEMON_GO_SUCCESS);
    Location eilat = mapGet(pokemonGoGetLocations(pokemonGo), "eilat");
    ASSERT_TEST(locationAddPokemon(bern, pikachu) == LOCATION_SUCCESS);
    ASSERT_TEST(locationAddPokemon(eilat, pikachu) == LOCATION_SUCCESS);
    ASSERT_TEST(error == POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Linda", 123, "bern") ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerMoveHunt(pokemonGo, stderr, NULL, "paris") ==
                POKEMON_GO_NULL_ARG);
    ASSERT_TEST(pokemonGoTrainerMoveHunt(pokemonGo, stderr, "Ash", NULL) ==
                POKEMON_GO_NULL_ARG);
    ASSERT_TEST(pokemonGoTrainerMoveHunt(pokemonGo, stderr, "Linda", "paris") ==
                POKEMON_GO_LOCATION_DOES_NOT_EXIST);
    ASSERT_TEST(
            pokemonGoTrainerMoveHunt(pokemonGo, stderr, "Yishay", "paris") ==
            POKEMON_GO_TRAINER_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoTrainerMoveHunt(pokemonGo, stderr, "Linda", "bern") ==
                POKEMON_GO_TRAINER_ALREADY_IN_LOCATION);
    ASSERT_TEST(pokemonGoTrainerMoveHunt(pokemonGo, stderr, "Linda", "eilat") ==
                POKEMON_GO_LOCATION_IS_NOT_REACHABLE);
    listDestroy(types);
    pokemonDestroy(pikachu);
    pokemonGoDestroy(pokemonGo);
    return true;
}

static bool testPokemonGoTrainerPurchase() {
    PokemonGoResult error;
    PokemonGo pokemonGo = pokemonGoCreate(&error);
    ASSERT_TEST(pokemonGoAddLocation(pokemonGo, "bern") == POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Linda", 100, "bern") ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, NULL, "candy", 50) ==
                POKEMON_GO_NULL_ARG);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Ash", NULL, 50) ==
                POKEMON_GO_NULL_ARG);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Ash", "candy", -30) ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Ash", "pot", -30) ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Ash", "", -30) ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Linda", "candy", 10) ==
                POKEMON_GO_ITEM_OUT_OF_STOCK);
    ASSERT_TEST(pokemonGoAddItemsToStore(pokemonGo, "candy", 50, 1000) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Linda", "candy", 50) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Rika", "candy", 50) ==
                POKEMON_GO_TRAINER_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoAddItemsToStore(pokemonGo, "potion", 500, 1) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Linda", "potion", 500) ==
                POKEMON_GO_BUDGET_IS_NOT_SUFFICIENT);
    pokemonGoDestroy(pokemonGo);
    return true;
}

static bool testPokemonGoTrainerBattle() {
    PokemonGoResult error;
    PokemonGo pokemonGo = pokemonGoCreate(&error);
    PokemonResult pokemon_error;

    // First Pokemon
    List types1 = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types1, &type1);
    Pokemon pikachu = pokemonCreate("Pikachu", "Raichu", 3, 1, types1,
                                    &pokemon_error);
    listDestroy(types1);
    ASSERT_TEST(pokemonGoAddLocation(pokemonGo, "eilat") == POKEMON_GO_SUCCESS);
    Location eilat = mapGet(pokemonGoGetLocations(pokemonGo), "eilat");
    ASSERT_TEST(locationAddPokemon(eilat, pikachu) == LOCATION_SUCCESS);

    // Second Pokemon
    List types2 = listTypeCreate();
    PokemonType type2 = TYPE_WATER;
    listInsertLast(types2, &type2);
    Pokemon squirtle = pokemonCreate("Squirtle", "Wartortle", 3, 1, types2,
                                     &pokemon_error);
    listDestroy(types2);
    ASSERT_TEST(
            pokemonGoAddLocation(pokemonGo, "metula") == POKEMON_GO_SUCCESS);
    Location metula = mapGet(pokemonGoGetLocations(pokemonGo), "metula");
    ASSERT_TEST(locationAddPokemon(metula, pikachu) == LOCATION_SUCCESS);


    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Ash", 100, "eilat") ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Gary", 100, "metula") ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerBattle(pokemonGo, stderr, NULL, 1, "Gary", 1) ==
                POKEMON_GO_NULL_ARG);
    ASSERT_TEST(pokemonGoTrainerBattle(pokemonGo, stderr, "Ash", 1, NULL, 1) ==
                POKEMON_GO_NULL_ARG);
    ASSERT_TEST(pokemonGoTrainerBattle(pokemonGo, stderr, "", 1, "Gary", 1) ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(
            pokemonGoTrainerBattle(pokemonGo, stderr, "Ash", -1, "Gary", 1) ==
            POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(
            pokemonGoTrainerBattle(pokemonGo, stderr, "Ash", 1, "Gary", -1) ==
            POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(
            pokemonGoTrainerBattle(pokemonGo, stderr, "Ash", 0, "Gary", 1) ==
            POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(
            pokemonGoTrainerBattle(pokemonGo, stderr, "Ash", 13, "Gary", 1) ==
            POKEMON_GO_POKEMON_DOES_NOT_EXIST);
    ASSERT_TEST(
            pokemonGoTrainerBattle(pokemonGo, stderr, "Ash", 1, "Gary", 1) ==
            POKEMON_GO_SUCCESS);
    pokemonDestroy(pikachu);
    pokemonDestroy(squirtle);
    pokemonGoDestroy(pokemonGo);
    return true;
}

static bool testPokemonGoTrainerHealPokemon() {
    PokemonGoResult error;
    PokemonResult pokemon_error;
    PokemonGo pokemonGo = pokemonGoCreate(&error);

    ASSERT_TEST(pokemonGoAddLocation(pokemonGo, "bern") == POKEMON_GO_SUCCESS);
    List types1 = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types1, &type1);
    Pokemon pikachu = pokemonCreate("Pikachu", "Raichu", 3, 1, types1,
                                    &pokemon_error);
    listDestroy(types1);
    Location bern = mapGet(pokemonGoGetLocations(pokemonGo), "bern");
    ASSERT_TEST(locationAddPokemon(bern, pikachu) == LOCATION_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Linda", 100, "bern") ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoAddItemsToStore(pokemonGo, "potion", 62, 3) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Linda", "potion", 62) ==
                POKEMON_GO_SUCCESS);
    Trainer linda = mapGet(pokemonGoGetTrainers(pokemonGo), "Linda");
    Pokemon pokemon1 = trainerGetPokemonByID(linda, 1);
    pokemonSetHp(pokemon1, 10);
    ASSERT_TEST(pokemonGoTrainerHealPokemon(pokemonGo, "Linda", 1) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGetHp(pokemon1) == 72);
    ASSERT_TEST(pokemonGoTrainerHealPokemon(pokemonGo, "Linda", 1) ==
                POKEMON_GO_NO_AVAILABLE_ITEM_FOUND);
    ASSERT_TEST(pokemonGoTrainerHealPokemon(pokemonGo, "Linda", 3) ==
                POKEMON_GO_POKEMON_DOES_NOT_EXIST);
    ASSERT_TEST(pokemonGoTrainerHealPokemon(pokemonGo, NULL, 1) ==
                POKEMON_GO_NULL_ARG);
    pokemonSetHp(pokemon1, MAX_HP);
    ASSERT_TEST(pokemonGoTrainerHealPokemon(pokemonGo, "Linda", 1) ==
                POKEMON_GO_HP_IS_AT_MAX);
    pokemonDestroy(pikachu);
    pokemonGoDestroy(pokemonGo);
    return true;
}

static bool testPokemonGoTrainerTrainPokemon() {
    PokemonGoResult error;
    PokemonResult pokemon_error;
    PokemonGo pokemonGo = pokemonGoCreate(&error);

    ASSERT_TEST(pokemonGoAddLocation(pokemonGo, "bern") == POKEMON_GO_SUCCESS);
    List types1 = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types1, &type1);
    Pokemon pikachu = pokemonCreate("Pikachu", "Raichu", 3, 1, types1,
                                    &pokemon_error);
    listDestroy(types1);
    Location bern = mapGet(pokemonGoGetLocations(pokemonGo), "bern");
    ASSERT_TEST(locationAddPokemon(bern, pikachu) == LOCATION_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Linda", 1000, "bern") ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoAddItemsToStore(pokemonGo, "candy", 150, 3) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Linda", "candy", 150) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Linda", "candy", 150) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoTrainerPurchase(pokemonGo, "Linda", "candy", 150) ==
                POKEMON_GO_SUCCESS);
    Trainer linda = mapGet(pokemonGoGetTrainers(pokemonGo), "Linda");
    Pokemon pokemon1 = trainerGetPokemonByID(linda, 1);
    ASSERT_TEST(pokemonGetCp(pokemon1) == 1);
    ASSERT_TEST(pokemonGoTrainerTrainPokemon(pokemonGo, "Linda", 1) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGetCp(pokemon1) == 151);
    ASSERT_TEST(pokemonGoTrainerTrainPokemon(pokemonGo, "Linda", 1) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGetCp(pokemon1) == 301);
    ASSERT_TEST(pokemonGoTrainerTrainPokemon(pokemonGo, "Linda", 1) ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGetCp(pokemon1) == 451);
    ASSERT_TEST(pokemonGoTrainerTrainPokemon(pokemonGo, "Linda", 1) ==
                POKEMON_GO_NO_AVAILABLE_ITEM_FOUND);
    ASSERT_TEST(pokemonGoTrainerTrainPokemon(pokemonGo, "Linda", 15) ==
                POKEMON_GO_POKEMON_DOES_NOT_EXIST);
    pokemonDestroy(pikachu);
    pokemonGoDestroy(pokemonGo);
    return true;
}

static bool testPokemonGoAddItemsToStore() {
    PokemonGoResult error;
    PokemonResult pokemon_error;
    PokemonGo pokemonGo = pokemonGoCreate(&error);

    ASSERT_TEST(pokemonGoAddLocation(pokemonGo, "bern") == POKEMON_GO_SUCCESS);
    List types1 = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types1, &type1);
    Pokemon pikachu = pokemonCreate("Pikachu", "Raichu", 3, 1, types1,
                                    &pokemon_error);
    listDestroy(types1);
    Location bern = mapGet(pokemonGoGetLocations(pokemonGo), "bern");
    ASSERT_TEST(locationAddPokemon(bern, pikachu) == LOCATION_SUCCESS);
    ASSERT_TEST(pokemonGoAddTrainer(pokemonGo, stderr, "Linda", 1000, "bern") ==
                POKEMON_GO_SUCCESS);
    ASSERT_TEST(pokemonGoAddItemsToStore(pokemonGo, NULL, 250, 3) ==
                POKEMON_GO_NULL_ARG);
    ASSERT_TEST(pokemonGoAddItemsToStore(NULL, "candy", 250, 3) ==
                POKEMON_GO_NULL_ARG);
    ASSERT_TEST(pokemonGoAddItemsToStore(pokemonGo, "elixir", 250, 3) ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(pokemonGoAddItemsToStore(pokemonGo, "", 250, 3) ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(pokemonGoAddItemsToStore(pokemonGo, "candy", 250, -1) ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(pokemonGoAddItemsToStore(pokemonGo, "candy", 250, 0) ==
                POKEMON_GO_INVALID_ARG);
    ASSERT_TEST(pokemonGoAddItemsToStore(pokemonGo, "candy", 150, 3) ==
                POKEMON_GO_SUCCESS);
    pokemonDestroy(pikachu);
    pokemonGoDestroy(pokemonGo);
    return true;
}

int main() {
    freopen("/dev/null", "w", stderr);
    RUN_TEST(testPokemonGoCreate);
    RUN_TEST(testPokemonGoDestroy);
    RUN_TEST(testPokemonGoGetPokedex);
    RUN_TEST(testPokemonGoAddTrainer);
    RUN_TEST(testPokemonGoTrainerMoveHunt);
    RUN_TEST(testPokemonGoTrainerPurchase);
    RUN_TEST(testPokemonGoTrainerHealPokemon);
    RUN_TEST(testPokemonGoTrainerTrainPokemon);
    RUN_TEST(testPokemonGoTrainerBattle);
    RUN_TEST(testPokemonGoAddItemsToStore);
    return 0;
}