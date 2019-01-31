#include "../pokemon.h"
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

static bool testPokemonCreate() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 3, 1, types, &error);
    ASSERT_TEST(pika != NULL);
    Pokemon pica = pokemonCreate("", "sa", 5, 1, NULL, &error);
    ASSERT_TEST(pica == NULL);
    Pokemon pita = pokemonCreate("sa", "", 5, 1, NULL, &error);
    ASSERT_TEST(pita == NULL);
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonDestroy() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonCopy() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 3, 1, types, &error);
    Pokemon pita = pokemonCopy(pika, &error);
    ASSERT_TEST(1 == pokemonGetCp(pita));
    ASSERT_TEST(strcmp("Pikachu", pokemonGetName(pika)) == 0);
    ASSERT_TEST(strcmp("Pikachu", pokemonGetName(pita)) == 0);
    listDestroy(types);
    pokemonDestroy(pika);
    pokemonDestroy(pita);
    return true;
}

static bool testPokemonSetHp() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    pokemonSetHp(pika, 50.0);
    ASSERT_TEST(50.0 == pokemonGetHp(pika));
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonSetLevel() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    pokemonSetLevel(pika, 2);
    ASSERT_TEST(pokemonGetLevel(pika) == 2);
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonAddCp() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    pokemonAddCp(pika, 4);
    ASSERT_TEST(5 == pokemonGetCp(pika));
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonGetHp() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    ASSERT_TEST(100 == pokemonGetHp(pika));
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonGetCp() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    pokemonAddCp(pika, 4);
    ASSERT_TEST(5 == pokemonGetCp(pika));
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonGetLevel() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    ASSERT_TEST(1 == pokemonGetLevel(pika));
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonGetEvolutionLevel() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    pokemonSetEvolutionLevel(pika, 5);
    ASSERT_TEST(5 == pokemonGetEvolutionLevel(pika));
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonGetEvolutionName() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    ASSERT_TEST(strcmp("Raichu", pokemonGetEvolutionName(pika)) == 0);
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonGetBonusCp() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    pokemonAddBonusCp(pika, 4);
    ASSERT_TEST(4 == pokemonGetBonusCp(pika));
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonAddBonusCp() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    pokemonAddBonusCp(pika, 4);
    ASSERT_TEST(4 == pokemonGetBonusCp(pika));
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}


static bool testPokemonGetName() {
    PokemonResult error;
    List types = listTypeCreate();
    PokemonType type1 = TYPE_ELECTRIC;
    listInsertLast(types, &type1);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    ASSERT_TEST(strcmp("Pikachu", pokemonGetName(pika)) == 0);
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonGetTypes() {
    PokemonResult error;
    List types1 = listTypeCreate();
    PokemonType type1 = TYPE_FIRE;
    PokemonType type2 = TYPE_FLYING;
    listInsertFirst(types1, &type1);
    listInsertLast(types1, &type2);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types1, &error);
    PokemonType *first_type = listGetFirst(pokemonGetTypes(pika));
    ASSERT_TEST(type1 == *first_type);
    PokemonType *pikaType2 = listGetNext(pokemonGetTypes(pika));
    ASSERT_TEST(type2 == *pikaType2);
    listDestroy(types1);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonSetEvolutionName() {
    PokemonResult error;
    List types = listTypeCreate();
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    pokemonSetEvolutionName(pika, "Woo");
    ASSERT_TEST(strcmp("Woo", pokemonGetEvolutionName(pika)) == 0);
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

static bool testPokemonSetEvolutionLevel() {
    PokemonResult error;
    List types = listTypeCreate();
    Pokemon pika = pokemonCreate("Pikachu", "Raichu", 5, 1, types, &error);
    pokemonSetEvolutionLevel(pika, 10);
    ASSERT_TEST(10 == pokemonGetEvolutionLevel(pika));
    listDestroy(types);
    pokemonDestroy(pika);
    return true;
}

int main() {
    RUN_TEST(testPokemonCreate);
    RUN_TEST(testPokemonDestroy);
    RUN_TEST(testPokemonCopy);
    RUN_TEST(testPokemonAddBonusCp);
    RUN_TEST(testPokemonAddCp);
    RUN_TEST(testPokemonSetLevel);
    RUN_TEST(testPokemonGetBonusCp);
    RUN_TEST(testPokemonGetCp);
    RUN_TEST(testPokemonGetEvolutionLevel);
    RUN_TEST(testPokemonGetEvolutionName);
    RUN_TEST(testPokemonGetHp);
    RUN_TEST(testPokemonGetLevel);
    RUN_TEST(testPokemonGetName);
    RUN_TEST(testPokemonGetTypes);
    RUN_TEST(testPokemonSetEvolutionLevel);
    RUN_TEST(testPokemonSetEvolutionName);
    RUN_TEST(testPokemonSetHp);
    return 0;
}