#include <stdbool.h>
#include <stdio.h>
#include "aux_macros.h"
#include "pokemon_trainer.h"

static bool testCombo() {
    bool result = true;
    Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 500, 4);
    pokemonTeachMove(pikachu, "Lightning", TYPE_ELECTRIC, 10, 110);
    PokemonTrainer trainer = pokemonTrainerCreate("Ash", pikachu, 5, 20);
    pokemonDestroy(pikachu);
    TEST_DIFFERENT(result, trainer, NULL);
    PokemonTrainer trainer_copy = pokemonTrainerCopy(trainer);
    TEST_DIFFERENT(result, trainer_copy, NULL);
    pokemonTrainerDestroy(trainer_copy);
    Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 500, 4);
    pokemonTeachMove(squirtle, "Bubble", TYPE_WATER, 10, 110);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(trainer, squirtle),
                POKEMON_TRAINER_SUCCESS);
    pokemonDestroy(squirtle);
    Pokemon gengar = pokemonCreate("Gengar", TYPE_WATER, 500, 4);
    pokemonTeachMove(gengar, "Gengar", TYPE_WATER, 10, 110);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(trainer, gengar),
                POKEMON_TRAINER_SUCCESS);
    pokemonDestroy(gengar);
    Pokemon charmander = pokemonCreate("Charmander", TYPE_FIRE, 400, 4);
    pokemonTeachMove(charmander, "First Blast", TYPE_FIRE, 68, 80);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(trainer, charmander),
                POKEMON_TRAINER_SUCCESS);
    pokemonDestroy(charmander);
    Pokemon bulbasor = pokemonCreate("Bulbasor", TYPE_GRASS, 200, 4);
    pokemonTeachMove(bulbasor, "Leaf", TYPE_GRASS, 138, 190);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(trainer, bulbasor),
                POKEMON_TRAINER_SUCCESS);
    pokemonDestroy(bulbasor);
    TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 5);
    TEST_DIFFERENT(result, pokemonTrainerGetPokemon(trainer, 2), NULL);
    TEST_EQUALS(result, pokemonTrainerDepositPokemon(trainer, 2),
                POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result, pokemonTrainerDepositPokemon(trainer, 2),
                POKEMON_TRAINER_SUCCESS);
    Pokemon A = pokemonCreate("A", TYPE_GRASS, 300, 4);
    Pokemon B = pokemonCreate("B", TYPE_GRASS, 300, 4);
    pokemonTeachMove(A, "Bubble", TYPE_GRASS, 10, 110);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(trainer, A),
                POKEMON_TRAINER_SUCCESS);
    pokemonTeachMove(B, "Bubble", TYPE_GRASS, 10, 110);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(trainer, B),
                POKEMON_TRAINER_SUCCESS);
    pokemonDestroy(A);
    pokemonDestroy(B);
    TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 7);
    TEST_EQUALS(result, pokemonTrainerWithdrawPokemon(trainer, 1),
                POKEMON_TRAINER_PARTY_FULL);
    TEST_EQUALS(result, pokemonTrainerRemovePokemon(trainer, -42),
                POKEMON_TRAINER_INVALID_INDEX);
    TEST_EQUALS(result, pokemonTrainerGetMostRankedPokemon(trainer),
                pokemonTrainerGetPokemon(trainer, 3));
    TEST_EQUALS(result, pokemonTrainerMakeMostRankedParty(trainer),
                POKEMON_TRAINER_SUCCESS);
    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerCreate() {
    bool result = true;
    Pokemon mew = pokemonCreate("Mew", TYPE_NORMAL, 300, 4);
    PokemonTrainer ash = pokemonTrainerCreate("Ash", mew, 6, 6);
    TEST_DIFFERENT(result, ash, NULL);
    PokemonTrainer gary = pokemonTrainerCreate("Gary", mew, 0, 6);
    TEST_EQUALS(result, gary, NULL);
    PokemonTrainer john = pokemonTrainerCreate("John", mew, 6, 0);
    TEST_EQUALS(result, john, NULL);
    PokemonTrainer john2 = pokemonTrainerCreate("John", NULL, 6, 0);
    TEST_EQUALS(result, john2, NULL);
    TEST_EQUALS(result, pokemonTrainerCreate("", mew, 6, 0),NULL);
    pokemonDestroy(mew);
    pokemonTrainerDestroy(ash);
    pokemonTrainerDestroy(gary);
    pokemonTrainerDestroy(john);
    return result;
}

static bool testPokemonTrainerDestroy() {
    bool result = true;
    Pokemon mew = pokemonCreate("Mew", TYPE_NORMAL, 300, 4);
    PokemonTrainer ash = pokemonTrainerCreate("Ash", mew, 6, 6);
    pokemonTrainerAddPokemon(ash, mew);
    TEST_DIFFERENT(result, ash, NULL);
    pokemonDestroy(mew);
    pokemonTrainerDestroy(ash);
    return result;
}

static bool testPokemonTrainerCopy() {
    bool result = true;
    Pokemon magikarp = pokemonCreate("Magikarp", TYPE_WATER, 100, 4);
    PokemonTrainer gary = pokemonTrainerCreate("Gary", magikarp, 4, 6);
    Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 200, 4);
    pokemonTrainerAddPokemon(gary, pikachu);
    pokemonTrainerAddPokemon(gary, magikarp);
    PokemonTrainer null_copy = pokemonTrainerCopy(NULL);
    TEST_EQUALS(result, null_copy, NULL);
    PokemonTrainer copy = pokemonTrainerCopy(gary);
    TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(gary),
                pokemonTrainerGetNumberOfPokemons(copy));
    pokemonDestroy(magikarp);
    pokemonDestroy(pikachu);
    pokemonTrainerDestroy(gary);
    pokemonTrainerDestroy(copy);
    return result;
}

static bool testPokemonTrainerAddPokemon() {
    bool result = true;
    Pokemon magikarp = pokemonCreate("Magikarp", TYPE_WATER, 100, 4);
    Pokemon charmander = pokemonCreate("Charmander", TYPE_FIRE, 200, 4);
    Pokemon bulbasor = pokemonCreate("Bulbasor", TYPE_GRASS, 300, 4);
    Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 400, 4);
    Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 500, 4);
    PokemonTrainer gary = pokemonTrainerCreate("Gary", magikarp, 4, 6);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(NULL, bulbasor),
                POKEMON_TRAINER_NULL_ARG);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(gary, NULL),
                POKEMON_TRAINER_NULL_ARG);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(gary, charmander),
                POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(gary, bulbasor),
                POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(gary, squirtle),
                POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result, pokemonTrainerAddPokemon(gary, pikachu),
                POKEMON_TRAINER_PARTY_FULL);
    pokemonDestroy(magikarp);
    pokemonDestroy(pikachu);
    pokemonDestroy(bulbasor);
    pokemonDestroy(charmander);
    pokemonDestroy(squirtle);
    pokemonTrainerDestroy(gary);
    return result;
}

static bool testPokemonTrainerGetPokemon() {
    bool result = true;
    Pokemon magikarp = pokemonCreate("Magikarp", TYPE_WATER, 100, 4);
    PokemonTrainer gary = pokemonTrainerCreate("Gary", magikarp, 4, 6);
    TEST_EQUALS(result, NULL, pokemonTrainerGetPokemon(gary, 0));
    TEST_EQUALS(result, NULL, pokemonTrainerGetPokemon(gary, 7));
    TEST_EQUALS(result, pokemonTrainerGetMostRankedPokemon(gary),
                pokemonTrainerGetPokemon(gary, 1)); // both func return pokemon
    pokemonDestroy(magikarp);
    pokemonTrainerDestroy(gary);
    return result;
}

static bool testPokemonTrainerRemovePokemon() {
    bool result = true;
    Pokemon magikarp = pokemonCreate("Magikarp", TYPE_WATER, 100, 4);
    PokemonTrainer gary = pokemonTrainerCreate("Gary", magikarp, 4, 6);
    TEST_EQUALS(result, POKEMON_TRAINER_INVALID_INDEX,
                pokemonTrainerRemovePokemon(gary, 0));
    TEST_EQUALS(result, POKEMON_TRAINER_INVALID_INDEX,
                pokemonTrainerRemovePokemon(gary, 7));
    TEST_EQUALS(result, POKEMON_TRAINER_REMOVE_LAST,
                pokemonTrainerRemovePokemon(gary, 1));
    pokemonDestroy(magikarp);
    pokemonTrainerDestroy(gary);
    return result;
}

static bool testPokemonTrainerDepositPokemon() {
    bool result = true;
    Pokemon magikarp = pokemonCreate("Magikarp", TYPE_WATER, 100, 4);
    Pokemon charmander = pokemonCreate("Charmander", TYPE_FIRE, 200, 4);
    Pokemon mew = pokemonCreate("Mew", TYPE_NORMAL, 300, 4);
    PokemonTrainer gary = pokemonTrainerCreate("Gary", magikarp, 4, 1);
    TEST_EQUALS(result, POKEMON_TRAINER_INVALID_INDEX,
                pokemonTrainerDepositPokemon(gary, 0));
    TEST_EQUALS(result, POKEMON_TRAINER_INVALID_INDEX,
                pokemonTrainerDepositPokemon(gary, 7));
    pokemonTrainerAddPokemon(gary, charmander);
    TEST_EQUALS(result, POKEMON_TRAINER_SUCCESS,
                pokemonTrainerDepositPokemon(gary, 2));
    pokemonTrainerAddPokemon(gary, mew);
    TEST_EQUALS(result, POKEMON_TRAINER_DEPOSIT_FULL,
                pokemonTrainerDepositPokemon(gary, 2));
    pokemonTrainerRemovePokemon(gary, 2);
    TEST_EQUALS(result, POKEMON_TRAINER_DEPOSIT_LAST,
                pokemonTrainerDepositPokemon(gary, 1));
    pokemonDestroy(mew);
    pokemonDestroy(magikarp);
    pokemonDestroy(charmander);
    pokemonTrainerDestroy(gary);
    return result;
}

static bool testPokemonTrainerWithdrawPokemon() {
    bool result = true;
    Pokemon magikarp = pokemonCreate("Magikarp", TYPE_WATER, 100, 4);
    Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 500, 4);
    Pokemon charmander = pokemonCreate("Charmander", TYPE_FIRE, 200, 4);
    PokemonTrainer gary = pokemonTrainerCreate("Gary", magikarp, 2, 2);
    pokemonTrainerAddPokemon(gary, charmander);
    pokemonTrainerDepositPokemon(gary, 1);
    pokemonTrainerAddPokemon(gary, pikachu);
    TEST_EQUALS(result, POKEMON_TRAINER_INVALID_INDEX,
                pokemonTrainerWithdrawPokemon(gary, 0));
    TEST_EQUALS(result, POKEMON_TRAINER_INVALID_INDEX,
                pokemonTrainerWithdrawPokemon(gary, 3));
    TEST_EQUALS(result, POKEMON_TRAINER_PARTY_FULL,
                pokemonTrainerWithdrawPokemon(gary, 1));
    pokemonTrainerRemovePokemon(gary, 1);
    TEST_EQUALS(result, POKEMON_TRAINER_SUCCESS,
                pokemonTrainerWithdrawPokemon(gary, 1));
    pokemonDestroy(magikarp);
    pokemonDestroy(charmander);
    pokemonDestroy(pikachu);
    pokemonTrainerDestroy(gary);
    return result;
}

static bool testPokemonTrainerGetNumberOfPokemons() {
    bool result = true;
    Pokemon magikarp = pokemonCreate("Magikarp", TYPE_WATER, 100, 4);
    Pokemon charmander = pokemonCreate("Charmander", TYPE_FIRE, 200, 4);
    PokemonTrainer gary = pokemonTrainerCreate("Gary", magikarp, 2, 2);
    pokemonTrainerAddPokemon(gary, charmander);
    pokemonTrainerDepositPokemon(gary, 2);
    TEST_EQUALS(result, 2, pokemonTrainerGetNumberOfPokemons(gary));
    pokemonDestroy(magikarp);
    pokemonDestroy(charmander);
    pokemonTrainerDestroy(gary);
    return result;
}

static bool testPokemonTrainerGetMostRankedPokemon() {
    bool result = true;
    Pokemon magikarp = pokemonCreate("Magikarp", TYPE_WATER, 100, 4);
    pokemonTeachMove(magikarp, "Splash", TYPE_WATER, 3, 612);
    Pokemon charmander = pokemonCreate("Charmander", TYPE_FIRE, 200, 4);
    pokemonTeachMove(charmander, "First Blast", TYPE_FIRE, 68, 80);
    PokemonTrainer gary = pokemonTrainerCreate("Gary", magikarp, 2, 2);
    pokemonTrainerAddPokemon(gary, charmander);
    pokemonTrainerDepositPokemon(gary, 2);
    TEST_EQUALS(result, NULL, pokemonTrainerGetMostRankedPokemon(NULL));
    TEST_EQUALS(result, pokemonTrainerGetPokemon(gary, 1),
                pokemonTrainerGetMostRankedPokemon(gary));
    pokemonDestroy(magikarp);
    pokemonDestroy(charmander);
    pokemonTrainerDestroy(gary);
    return result;
}

static bool testPokemonTrainerMakeMostRankedParty() {
    bool result = true;
    Pokemon magikarp = pokemonCreate("Magikarp", TYPE_WATER, 100, 4);
    Pokemon charmander = pokemonCreate("Charmander", TYPE_FIRE, 200, 4);
    Pokemon bulbasor = pokemonCreate("Bulbasor", TYPE_GRASS, 300, 4);
    Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 400, 4);
    Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 500, 4);
    Pokemon ponita = pokemonCreate("Ponita", TYPE_FIRE, 600, 4);
    Pokemon lapras = pokemonCreate("Lapras", TYPE_WATER, 400, 4);
    pokemonTeachMove(magikarp, "Splash", TYPE_WATER, 1, 1);
    pokemonTeachMove(charmander, "First Blast", TYPE_FIRE, 1, 1);
    pokemonTeachMove(bulbasor, "Leaf", TYPE_GRASS, 1, 1);
    pokemonTeachMove(squirtle, "Bubble", TYPE_WATER, 1, 1);
    pokemonTeachMove(pikachu, "Thunder", TYPE_ELECTRIC, 1, 1);
    pokemonTeachMove(ponita, "Leer", TYPE_FIRE, 1, 1);
    pokemonTeachMove(lapras, "Hail", TYPE_WATER, 1, 1);
    PokemonTrainer gary = pokemonTrainerCreate("Gary", magikarp, 3, 4);
    pokemonTrainerAddPokemon(gary, lapras);
    pokemonTrainerAddPokemon(gary, ponita);
    pokemonTrainerDepositPokemon(gary, 2);
    pokemonTrainerDepositPokemon(gary, 2);
    pokemonTrainerAddPokemon(gary, pikachu);
    pokemonTrainerDepositPokemon(gary, 2);
    pokemonTrainerAddPokemon(gary, bulbasor);
    pokemonTrainerDepositPokemon(gary, 2);
    pokemonTrainerAddPokemon(gary, squirtle);
    pokemonTrainerAddPokemon(gary, charmander);
    TEST_EQUALS(result, pokemonTrainerMakeMostRankedParty(NULL),
                POKEMON_TRAINER_NULL_ARG);
    TEST_EQUALS(result, pokemonTrainerMakeMostRankedParty(gary),
                POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result, pokemonGetRank(pokemonTrainerGetPokemon(gary, 1)),
                pokemonGetRank(ponita));
    TEST_EQUALS(result, pokemonGetRank(pokemonTrainerGetPokemon(gary, 2)),
                pokemonGetRank(pikachu));
    TEST_EQUALS(result, pokemonGetRank(pokemonTrainerGetPokemon(gary, 3)),
                pokemonGetRank(squirtle));
    pokemonTrainerRemovePokemon(gary, 2);
    pokemonTrainerRemovePokemon(gary, 2);
    pokemonTrainerWithdrawPokemon(gary, 1);
    pokemonTrainerWithdrawPokemon(gary, 1);
    TEST_EQUALS(result, pokemonGetRank(pokemonTrainerGetPokemon(gary, 2)),
                pokemonGetRank(lapras));
    TEST_EQUALS(result, pokemonGetRank(pokemonTrainerGetPokemon(gary, 3)),
                pokemonGetRank(bulbasor));
    pokemonTrainerRemovePokemon(gary, 2);
    pokemonTrainerRemovePokemon(gary, 2);
    pokemonTrainerWithdrawPokemon(gary, 1);
    pokemonTrainerWithdrawPokemon(gary, 1);
    TEST_EQUALS(result, pokemonGetRank(pokemonTrainerGetPokemon(gary, 2)),
                pokemonGetRank(charmander));
    TEST_EQUALS(result, pokemonGetRank(pokemonTrainerGetPokemon(gary, 3)),
                pokemonGetRank(magikarp));
    pokemonDestroy(magikarp);
    pokemonDestroy(charmander);
    pokemonDestroy(bulbasor);
    pokemonDestroy(squirtle);
    pokemonDestroy(pikachu);
    pokemonDestroy(ponita);
    pokemonDestroy(lapras);
    pokemonTrainerDestroy(gary);
    return result;
}

static bool testPokemonTrainerPrintEnumeration() {
    bool result = true;
    TEST_EQUALS(result, POKEMON_TRAINER_NULL_ARG,
                pokemonTrainerPrintEnumeration(NULL, NULL));
    return result;
}

int main() {
    RUN_TEST(testCombo);
    RUN_TEST(testPokemonTrainerCreate);
    RUN_TEST(testPokemonTrainerDestroy);
    RUN_TEST(testPokemonTrainerCopy);
    RUN_TEST(testPokemonTrainerAddPokemon);
    RUN_TEST(testPokemonTrainerGetPokemon);
    RUN_TEST(testPokemonTrainerRemovePokemon);
    RUN_TEST(testPokemonTrainerDepositPokemon);
    RUN_TEST(testPokemonTrainerWithdrawPokemon);
    RUN_TEST(testPokemonTrainerGetNumberOfPokemons);
    RUN_TEST(testPokemonTrainerGetMostRankedPokemon);
    RUN_TEST(testPokemonTrainerMakeMostRankedParty);
    RUN_TEST(testPokemonTrainerPrintEnumeration);
    return 0;
}