#include "../trainer.h"
#include "test_utilities.h"
#include <string.h>
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

static bool testTrainerCreate() {
    TrainerResult error;
    Trainer trainer1 = trainerCreate("Ash", -1, "paris", &error);
    ASSERT_TEST(trainer1 == NULL);
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    ASSERT_TEST(trainer != NULL);
    trainerDestroy(trainer1);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerDestroy() {
    TrainerResult error;
    Trainer trainer1 = trainerCreate("Ash", 6, "paris", &error);
    trainerDestroy(trainer1);
    return true;
}

static bool testTrainerCopy() {
    TrainerResult error;
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    Trainer trainer2 = trainerCopy(trainer, &error);
    ASSERT_TEST(trainer2 != NULL);
    ASSERT_TEST(trainerGetPokecoins(trainer) == trainerGetPokecoins(trainer));
    trainerDestroy(trainer);
    trainerDestroy(trainer2);
    return true;
}

static bool testTrainerAddPokemon() {
    TrainerResult error;
    PokemonResult pokemon_error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 3, 1, types,
                                 &pokemon_error);
    listDestroy(types);
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    ASSERT_TEST(trainerAddPokemon(trainer, pika) == TRAINER_SUCCESS);
    ASSERT_TEST(trainerGetNumOfPokemons(trainer) == 1);
    pokemonDestroy(pika);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerRemovePokemon() {
    TrainerResult error;
    PokemonResult pokemon_error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 3, 1, types,
                                 &pokemon_error);
    listDestroy(types);
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    ASSERT_TEST(trainerAddPokemon(trainer, pika) == TRAINER_SUCCESS);
    ASSERT_TEST(trainerRemovePokemon(trainer, 1) == TRAINER_SUCCESS);
    ASSERT_TEST(trainerGetNumOfPokemons(trainer) == 1);
    ASSERT_TEST(trainerGetPokemonByID(trainer, 1) == NULL);
    pokemonDestroy(pika);
    trainerDestroy(trainer);
    return true;
}


static bool testTrainerGetPokemonByID() {
    TrainerResult error;
    PokemonResult pokemon_error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 3, 1, types,
                                 &pokemon_error);
    listDestroy(types);
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    trainerAddPokemon(trainer, pika);
    ASSERT_TEST(pokemonGetCp(trainerGetPokemonByID(trainer, 1)) == 1);
    pokemonDestroy(pika);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerPokemonUseCandy() {
    TrainerResult error;
    PokemonResult pokemon_error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 3, 1, types,
                                 &pokemon_error);
    listDestroy(types);
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    trainerAddPokemon(trainer, pika);
    ASSERT_TEST(trainerPokemonUseCandy(trainer, 1) ==
                TRAINER_NO_AVAILABLE_ITEM_FOUND);
    storeAddItems(trainerGetItems(trainer), "candy", 80, 2);
    storeAddItems(trainerGetItems(trainer), "candy", 80, 2);
    storeAddItems(trainerGetItems(trainer), "candy", 50, 2);
    storeAddItems(trainerGetItems(trainer), "candy", 80, 2);
    storeAddItems(trainerGetItems(trainer), "candy", 100, 1);
    ASSERT_TEST(trainerPokemonUseCandy(trainer, 2) ==
                TRAINER_POKEMON_DOES_NOT_EXIST);
    ASSERT_TEST(trainerPokemonUseCandy(trainer, 1) == TRAINER_SUCCESS);
    ASSERT_TEST(pokemonGetCp(trainerGetPokemonByID(trainer, 1)) == 101);
    ASSERT_TEST(pokemonGetBonusCp(trainerGetPokemonByID(trainer, 1)) == 100);
    pokemonDestroy(pika);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerPokemonUsePotion() {
    TrainerResult error;
    PokemonResult pokemon_error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 3, 1, types,
                                 &pokemon_error);
    listDestroy(types);
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    trainerAddPokemon(trainer, pika);
    pokemonSetHp(trainerGetPokemonByID(trainer, 1), 50.0);
    ASSERT_TEST(trainerPokemonUsePotion(trainer, 1) ==
                TRAINER_NO_AVAILABLE_ITEM_FOUND);
    storeAddItems(trainerGetItems(trainer), "potion", 45, 1);
    storeAddItems(trainerGetItems(trainer), "potion", 42, 1);
    storeAddItems(trainerGetItems(trainer), "potion", 12, 1);
    storeAddItems(trainerGetItems(trainer), "potion", 70, 1);
    storeAddItems(trainerGetItems(trainer), "potion", 49, 1);
    storeAddItems(trainerGetItems(trainer), "potion", 50, 1);
    ASSERT_TEST(trainerPokemonUsePotion(trainer, 2) ==
                TRAINER_POKEMON_DOES_NOT_EXIST);
    ASSERT_TEST(trainerPokemonUsePotion(trainer, 1) == TRAINER_SUCCESS);
    ASSERT_TEST(trainerPokemonUsePotion(trainer, 1) ==
                TRAINER_POKEMON_HP_IS_AT_MAX);
    ASSERT_TEST(storeRemoveItem(trainerGetItems(trainer), "potion", 50) ==
                STORE_OUT_OF_STOCK);
    pokemonDestroy(pika);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerGetName() {
    TrainerResult error;
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    ASSERT_TEST(strcmp(trainerGetName(trainer), "Ash") == 0);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerGetLocation() {
    TrainerResult error;
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    ASSERT_TEST(strcmp(trainerGetLocation(trainer), "paris") == 0);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerGetExp() {
    TrainerResult error;
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    ASSERT_TEST(trainerGetExp(trainer) == 1.0);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerGetPokecoins() {
    TrainerResult error;
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    ASSERT_TEST(trainerGetPokecoins(trainer) == 0);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerGetNumPokemons() {
    TrainerResult error;
    PokemonResult pokemon_error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 3, 1, types,
                                 &pokemon_error);
    listDestroy(types);
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    ASSERT_TEST(trainerGetNumOfPokemons(trainer) == 0);
    trainerAddPokemon(trainer, pika);
    ASSERT_TEST(trainerGetNumOfPokemons(trainer) == 1);
    pokemonDestroy(pika);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerPokemonExist() {
    TrainerResult error;
    PokemonResult pokemon_error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 3, 1, types,
                                 &pokemon_error);
    listDestroy(types);
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    ASSERT_TEST(!trainerPokemonExist(trainer, 1));
    trainerAddPokemon(trainer, pika);
    ASSERT_TEST(trainerPokemonExist(trainer, 1));
    pokemonDestroy(pika);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerGetItems() {
    TrainerResult error;
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    Store trainer_items = trainerGetItems(trainer);
    ASSERT_TEST(trainer_items != NULL);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerGetPokemons() {
    TrainerResult error;
    PokemonResult pokemon_error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 3, 1, types,
                                 &pokemon_error);
    listDestroy(types);
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    trainerAddPokemon(trainer, pika);
    Map pokemons = trainerGetPokemons(trainer);
    ASSERT_TEST(pokemons != NULL);
    pokemonDestroy(pika);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerSetPokecoins() {
    TrainerResult error;
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    trainerSetPokecoins(trainer, 500);
    ASSERT_TEST(trainerGetPokecoins(trainer) == 500);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerSetExp() {
    TrainerResult error;
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    trainerSetExp(trainer, 200);
    ASSERT_TEST(trainerGetExp(trainer) == 200);
    trainerDestroy(trainer);
    return true;
}

static bool testTrainerSetLocation() {
    TrainerResult error;
    Trainer trainer = trainerCreate("Ash", 0, "paris", &error);
    trainerSetLocation(trainer, "rome");
    ASSERT_TEST(strcmp("rome", trainerGetLocation(trainer)) == 0);
    trainerDestroy(trainer);
    return true;
}

int main() {
    RUN_TEST(testTrainerCreate);
    RUN_TEST(testTrainerDestroy);
    RUN_TEST(testTrainerCopy);
    RUN_TEST(testTrainerAddPokemon);
    RUN_TEST(testTrainerRemovePokemon);
    RUN_TEST(testTrainerGetName);
    RUN_TEST(testTrainerGetExp);
    RUN_TEST(testTrainerGetItems);
    RUN_TEST(testTrainerGetLocation);
    RUN_TEST(testTrainerGetNumPokemons);
    RUN_TEST(testTrainerGetPokecoins);
    RUN_TEST(testTrainerGetPokemonByID);
    RUN_TEST(testTrainerGetPokemons);
    RUN_TEST(testTrainerPokemonExist);
    RUN_TEST(testTrainerPokemonUseCandy);
    RUN_TEST(testTrainerPokemonUsePotion);
    RUN_TEST(testTrainerSetPokecoins);
    RUN_TEST(testTrainerSetExp);
    RUN_TEST(testTrainerSetLocation);
    return 0;
}