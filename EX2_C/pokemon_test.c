#include <stdbool.h>
#include "aux_macros.h"
#include "pokemon.h"

static bool testCombo() {
	bool result = true;
	Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	TEST_DIFFERENT(result, pikachu, NULL);
	Pokemon pikachu_copy = pokemonCopy(pikachu);
	TEST_DIFFERENT(result, pikachu_copy, NULL);
	pokemonDestroy(pikachu_copy);
	TEST_EQUALS(result, pokemonTeachMove(pikachu, "Thunder", TYPE_ELECTRIC, 10, 110), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonTeachMove(pikachu, "Flash", TYPE_ELECTRIC, 10, 110), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonTeachMove(pikachu, "Heal", TYPE_ELECTRIC, 10, 110), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonTeachMove(pikachu, "Slam", TYPE_ELECTRIC, 10, 110), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, "Flash"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, "Heal"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, "Slam"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonGetLevel(pikachu), 1);
	TEST_EQUALS(result, pokemonGetRank(pikachu), 111);
	Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 10, 4);
	pokemonTeachMove(squirtle, "Bubble", TYPE_WATER, 30, 40);
	TEST_EQUALS(result, pokemonUseMove(
		squirtle, pikachu, "Bubble"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonHeal(pikachu), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonEvolve(squirtle, "Wartortle"), POKEMON_SUCCESS);
	pokemonDestroy(pikachu);
	pokemonDestroy(squirtle);
	return result;
}

static bool testPokemonCreate() {
	bool result = true;
	Pokemon charizard = pokemonCreate("Chariazrd", TYPE_FIRE, 4001, 4) ;
	TEST_DIFFERENT(result, charizard, NULL);
    Pokemon water1 = pokemonCreate("Waturtle", TYPE_WATER, 9999, 4) ;
    TEST_EQUALS(result, water1, NULL);
    Pokemon water2 = pokemonCreate("Waturtle", TYPE_WATER, -1, 4) ;
    TEST_EQUALS(result, water2, NULL);
    Pokemon water3 = pokemonCreate("\0", TYPE_WATER, 200, 0) ;
    TEST_EQUALS(result, water3, NULL);
    pokemonDestroy(charizard);
	return result;
}

static bool testPokemonDestroy() {
	bool result = true;
	Pokemon charizard = pokemonCreate("Chariazrd", TYPE_FIRE, 4001, 4) ;
	pokemonDestroy(charizard);

	//trying to destroy a NULL pointer
	Pokemon null_pokemon = NULL;
	pokemonDestroy(null_pokemon);
	
	return result;
}

static bool testPokemonCopy() {
	bool result = true;
	Pokemon charizard = pokemonCreate("Chariazrd", TYPE_FIRE, 1234, 1);
    pokemonTeachMove(charizard, "Fireblast", TYPE_FIRE, 10, 12 );
    Pokemon copy = pokemonCopy(charizard);
    TEST_EQUALS(result,pokemonGetLevel(charizard),pokemonGetLevel(charizard));
    TEST_EQUALS(result,pokemonGetRank(charizard),pokemonGetRank(charizard));
    
    //trying to copy a NULL pointer
	Pokemon null_pokemon = NULL;
	TEST_EQUALS(result,null_pokemon,NULL);
	
	pokemonDestroy(charizard);
	pokemonDestroy(copy);
	return result;
}

static bool testPokemonTeachMove() {
	bool result = true;
	Pokemon charizard = pokemonCreate("Chariazrd", TYPE_FIRE, 4001, 4);
	TEST_EQUALS(result, POKEMON_SUCCESS,	pokemonTeachMove(charizard, "Fireblast", TYPE_FIRE, 10, 12));
    TEST_EQUALS(result, POKEMON_MOVE_ALREADY_EXISTS, pokemonTeachMove(charizard, "Fireblast", TYPE_FIRE, 10, 12));
    TEST_EQUALS(result, POKEMON_NULL_ARG,pokemonTeachMove(charizard, NULL, TYPE_FIRE, 10, 12));
    TEST_EQUALS(result, POKEMON_NULL_ARG,pokemonTeachMove(NULL, "TestMove", TYPE_FIRE, 10, 12));
    TEST_EQUALS(result, POKEMON_INVALID_TYPE, pokemonTeachMove(charizard, "Fireblast", -999, 10, 12));
    TEST_EQUALS(result, POKEMON_INVALID_MOVE_NAME,pokemonTeachMove(charizard, "", TYPE_FIRE, 10, 12));
    TEST_EQUALS(result, POKEMON_INVALID_POWER_POINTS,pokemonTeachMove(charizard, "Fireblast", TYPE_FIRE, -1, 12));
    TEST_EQUALS(result, POKEMON_INVALID_STRENGTH,pokemonTeachMove(charizard, "Fireblast", TYPE_FIRE, 10, -1));
    TEST_EQUALS(result, POKEMON_INCOMPATIBLE_MOVE_TYPE,pokemonTeachMove(charizard, "Fireblast", TYPE_WATER, 10, 12));
    
    // Testing a case of trying to add a new move to a full moves list:
   	TEST_EQUALS(result, POKEMON_SUCCESS,pokemonTeachMove(charizard, "Aspire", TYPE_FIRE, 10, 12));
	TEST_EQUALS(result, POKEMON_SUCCESS,pokemonTeachMove(charizard, "Earthquake", TYPE_FIRE, 10, 12));
	TEST_EQUALS(result, POKEMON_SUCCESS,pokemonTeachMove(charizard, "Fire Rage", TYPE_FIRE, 10, 12));
	TEST_EQUALS(result, POKEMON_SUCCESS, pokemonTeachMove(charizard, "Leer", TYPE_FIRE, 10, 12));
	TEST_EQUALS(result, POKEMON_MOVE_DOES_NOT_EXIST, pokemonUnteachMove(charizard, "Aspire")); // First move lexically
                
    pokemonDestroy(charizard);
	return result;
}

static bool testPokemonUnteachMove() {
	bool result = true;
	Pokemon charizard = pokemonCreate("Chariazrd", TYPE_FIRE, 4001, 4);
	pokemonTeachMove(charizard, "Fireblast", TYPE_FIRE, 10, 12 );
	TEST_EQUALS(result, POKEMON_NULL_ARG, pokemonUnteachMove(charizard,NULL));
	TEST_EQUALS(result, POKEMON_NULL_ARG, pokemonUnteachMove(NULL,"Blast"));
    TEST_EQUALS(result, POKEMON_INVALID_MOVE_NAME,pokemonUnteachMove(charizard, ""));
    TEST_EQUALS(result, POKEMON_MOVE_DOES_NOT_EXIST, pokemonUnteachMove(charizard, "blast"));
    TEST_EQUALS(result, POKEMON_SUCCESS,pokemonUnteachMove(charizard, "Fireblast"));
	pokemonDestroy(charizard);
	return result;
}

static bool testPokemonGetLevel() {
	bool result = true;
	Pokemon charizard = pokemonCreate("Chariazrd", TYPE_FIRE, 9901, 4);
	TEST_EQUALS(result, pokemonGetLevel(charizard), 100) ;
	pokemonDestroy(charizard);
	return result;
}

static bool testPokemonGetRank() {
	bool result = true;
	Pokemon charizard = pokemonCreate("Chariazrd", TYPE_FIRE, 4001, 4);
	pokemonTeachMove(charizard,"Fireblast", TYPE_FIRE, 10, 12 );
	TEST_EQUALS(result, pokemonGetRank(charizard), 53);
	pokemonUnteachMove(charizard,"Fireblast");
	TEST_EQUALS(result, pokemonGetRank(charizard), 0);
	pokemonDestroy(charizard);
	return result;
}

static bool testPokemonUseMove() {
    bool result = true;

    Pokemon charizard = pokemonCreate("Chariazrd", TYPE_FIRE, 4001, 2);
    pokemonTeachMove(charizard,"Fireblast", TYPE_FIRE, 10, 9999 );
    pokemonTeachMove(charizard,"Ember", TYPE_FIRE, 1, 5 );
    Pokemon mew = pokemonCreate("Mew", TYPE_NORMAL, 4001, 2);
    pokemonTeachMove(mew,"Strike",TYPE_NORMAL,1,10000);
    Pokemon venosaur = pokemonCreate("Venosaur", TYPE_GRASS, 101, 4);
    TEST_EQUALS(result, pokemonUseMove(charizard, venosaur, "Fireblast"),POKEMON_SUCCESS);
    TEST_EQUALS(result, pokemonUseMove(charizard, venosaur, "Fireblast"),POKEMON_NO_HEALTH_POINTS);
    TEST_EQUALS(result, pokemonUseMove(mew, charizard, "Strike"),POKEMON_SUCCESS);
    TEST_EQUALS(result, pokemonUseMove(mew, venosaur, "Strike"),POKEMON_NO_POWER_POINTS);
    TEST_EQUALS(result, pokemonUseMove(charizard, venosaur, ""),POKEMON_INVALID_MOVE_NAME);
    TEST_EQUALS(result, pokemonUseMove(charizard, venosaur, "UNKNOWN"),POKEMON_MOVE_DOES_NOT_EXIST);
    pokemonDestroy(mew);
    pokemonDestroy(charizard);
    pokemonDestroy(venosaur);
    return result;
}

static bool testPokemonHeal() {
    bool result = true;

    Pokemon charizard = pokemonCreate("Chariazrd", TYPE_FIRE, 4001, 2);
    pokemonTeachMove(charizard,"Fireblast", TYPE_FIRE, 10, 1000 );
    pokemonTeachMove(charizard,"Firestorm", TYPE_FIRE, 20, 2000 );
    TEST_EQUALS(result,pokemonHeal(NULL),POKEMON_NULL_ARG);
    TEST_EQUALS(result,pokemonHeal(charizard),POKEMON_SUCCESS);
    return result;
}

static bool testPokemonEvolve() {
    bool result = true;

    Pokemon charmander = pokemonCreate("Charmander", TYPE_FIRE, 100, 1);
    pokemonTeachMove(charmander,"Amber", TYPE_FIRE, 20, 2000 );
    int lvl_before_evolve = pokemonGetLevel(charmander);
    Pokemon blastoise = pokemonCreate("Blastoise", TYPE_FIRE, 9901, 1);
    TEST_EQUALS(result,pokemonEvolve(charmander,NULL),POKEMON_NULL_ARG);
    TEST_EQUALS(result,pokemonEvolve(NULL,NULL),POKEMON_NULL_ARG);
    TEST_EQUALS(result,pokemonEvolve(charmander,""),POKEMON_INVALID_NAME);
    TEST_EQUALS(result,pokemonEvolve(blastoise,"BlastoiseX"),POKEMON_CANNOT_EVOLVE);
    TEST_EQUALS(result,pokemonEvolve(charmander,"Charizard"),POKEMON_SUCCESS);
    TEST_EQUALS(result,lvl_before_evolve+1,pokemonGetLevel(charmander));

    pokemonDestroy(charmander);
    pokemonDestroy(blastoise);
    return result;
}

static bool testPokemonPrintName() {
	bool result = true;
	TEST_EQUALS(result, POKEMON_NULL_ARG, pokemonPrintName(NULL,NULL));
	return result;
}

static bool testPokemonPrintVoice() {
	bool result = true;
	TEST_EQUALS(result, POKEMON_NULL_ARG, pokemonPrintVoice(NULL,NULL));
	return result;
}

int main() {
	RUN_TEST(testCombo);
	RUN_TEST(testPokemonCreate);
	RUN_TEST(testPokemonDestroy);
	RUN_TEST(testPokemonCopy);
	RUN_TEST(testPokemonTeachMove);
	RUN_TEST(testPokemonUnteachMove);
	RUN_TEST(testPokemonGetLevel);
	RUN_TEST(testPokemonGetRank);
	RUN_TEST(testPokemonUseMove);
	RUN_TEST(testPokemonHeal);
	RUN_TEST(testPokemonEvolve);
	RUN_TEST(testPokemonPrintName);
	RUN_TEST(testPokemonPrintVoice);
	return 0;
}
