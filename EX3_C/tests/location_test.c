#include "../location.h"
#include "test_utilities.h"
#include <string.h>

static bool testLocationCreate(){
    LocationResult error;
    Location paris = locationCreate("paris",&error);
    ASSERT_TEST( paris != NULL);
    ASSERT_TEST(error == LOCATION_SUCCESS);
    locationDestroy(paris);
    return true;
}

static bool testLocationCopy(){
    LocationResult error;
    Location paris = locationCreate("paris", &error);
    Location copy = locationCopy(paris, &error);
    ASSERT_TEST( copy != NULL);
    ASSERT_TEST( strcmp("paris", locationGetName(copy)) == 0);
    locationDestroy(paris);
    locationDestroy(copy);
    return true;
}

static bool testLocationDestroy(){
    LocationResult error;
    Location paris = locationCreate("paris", &error);
    locationDestroy(paris);
    return true;
}

static bool testLocationGetName(){
    LocationResult error;
    Location paris = locationCreate("paris",&error);
    ASSERT_TEST( strcmp("paris", locationGetName(paris)) == 0);
    locationDestroy(paris);
    return true;
}

static bool testLocationGetFirstPokemon(){
    PokemonResult pokemon_error;
    LocationResult error;
    List types = listCreate(copyType, freeType);
    Pokemon rika = pokemonCreate("Rikachu", "Paichu",3,1,types,&pokemon_error);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu",3,1,types,&pokemon_error);
    listDestroy(types);
    Location paris = locationCreate("paris",&error);
    locationAddPokemon(paris, pika);
    locationAddPokemon(paris, rika);
    ASSERT_TEST( strcmp(pokemonGetName(locationGetFirstPokemon(paris)), pokemonGetName(pika)) == 0);
    pokemonDestroy(pika);
    pokemonDestroy(rika);
    locationDestroy(paris);
    return true;
}

static bool testLocationAddNearby(){
    LocationResult error;
    Location paris = locationCreate("paris",&error);
    Location rome = locationCreate("rome",&error);
    Location berlin = locationCreate("berlin",&error);
    ASSERT_TEST( locationAddNearby(paris, "rome") == LOCATION_SUCCESS);
    ASSERT_TEST( locationAddNearby(paris, "paris") == LOCATION_INVALID_ARG);
    locationDestroy(paris);
    locationDestroy(rome);
    locationDestroy(berlin);
    return true;
}

static bool testLocationAddPokemon(){
    PokemonResult pokemon_error;
    LocationResult error;
    List types = listCreate(copyType, freeType);
    Pokemon rika = pokemonCreate("Rikachu", "Paichu",3,1,types,&pokemon_error);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu",3,1,types,&pokemon_error);
    listDestroy(types);
    Location paris = locationCreate("paris",&error);
    locationAddPokemon(paris, pika);
    locationAddPokemon(paris, rika);
    ASSERT_TEST( strcmp(pokemonGetName(locationGetFirstPokemon(paris)), pokemonGetName(pika)) == 0);
    pokemonDestroy(pika);
    pokemonDestroy(rika);
    locationDestroy(paris);
    return true;
}

static bool testLocationRemoveFirstPokemon(){
    PokemonResult pokemon_error;
    LocationResult error;
    List types = listCreate(copyType, freeType);
    Pokemon pika = pokemonCreate("Pikachu", "Raichu",3,1,types,&pokemon_error);
    listDestroy(types);
    Location paris = locationCreate("paris",&error);
    locationAddPokemon(paris, pika);
    locationRemoveFirstPokemon(paris);
    ASSERT_TEST( locationGetFirstPokemon(paris) == NULL );
    pokemonDestroy(pika);
    locationDestroy(paris);
    return true;
}

static bool testLocationGetNearby(){
    LocationResult error;
    Location paris = locationCreate("paris",&error);
    ASSERT_TEST(locationAddNearby(paris, "rome") ==  LOCATION_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst( locationGetNearby(paris)),"rome") == 0);
    locationDestroy(paris);
    return true;
}


int main(){
    RUN_TEST(testLocationCreate);
    RUN_TEST(testLocationDestroy);
    RUN_TEST(testLocationCopy);
    RUN_TEST(testLocationAddNearby);
    RUN_TEST(testLocationAddPokemon);
    RUN_TEST(testLocationGetFirstPokemon);
    RUN_TEST(testLocationGetName);
    RUN_TEST(testLocationGetNearby);
    RUN_TEST(testLocationRemoveFirstPokemon);
    return 0;
}