#include <stdbool.h>
#include "aux_macros.h"
#include "pokemon.h"
//or and assaf
static bool testCombo() {
	bool result = true;
	Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	TEST_DIFFERENT(result, pikachu, NULL);
	Pokemon pikachu_copy = pokemonCopy(pikachu);
	TEST_DIFFERENT(result, pikachu_copy, NULL);
	pokemonDestroy(pikachu_copy);
	TEST_EQUALS(result, pokemonTeachMove(
		pikachu, "Thunder", TYPE_ELECTRIC, 10, 110), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, "Thunder"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonGetLevel(pikachu), 1);
	TEST_EQUALS(result, pokemonGetRank(pikachu), 0);
	Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 10, 4);
	pokemonTeachMove(squirtle, "Bubble", TYPE_WATER, 30, 40);
	TEST_EQUALS(result, pokemonUseMove(
		squirtle, pikachu, "Bubble"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonHeal(pikachu), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonEvolve(squirtle, "Wartortle"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonGetLevel(squirtle), 2);
	pokemonDestroy(pikachu);
	pokemonDestroy(squirtle);
	return result;
}

static bool testPokemonCreate() {
	bool result = true;
	Pokemon A=pokemonCreate(NULL, TYPE_GRASS,200,5);
	TEST_EQUALS(result, A, NULL);
	pokemonDestroy(A);
	Pokemon B=pokemonCreate("B", 5 ,200,5);
	TEST_EQUALS(result, B, NULL);
	pokemonDestroy(B);
	Pokemon C=pokemonCreate("C", -1 ,200,5);
	TEST_EQUALS(result, C, NULL);
	pokemonDestroy(C);
	Pokemon D=pokemonCreate("D", TYPE_GRASS,9902,5);
	TEST_EQUALS(result, D, NULL);
	pokemonDestroy(D);
	Pokemon E=pokemonCreate("E", TYPE_GRASS,-1,5);
	TEST_EQUALS(result, E, NULL);
	pokemonDestroy(E);
	Pokemon F=pokemonCreate("F", TYPE_GRASS,200,-1);
	TEST_EQUALS(result, F, NULL);
	pokemonDestroy(F);
	Pokemon G=pokemonCreate("G", TYPE_GRASS,200,5);
	TEST_DIFFERENT(result, G, NULL);
	TEST_EQUALS(result, pokemonGetLevel(G), 2);
	TEST_EQUALS(result, pokemonGetRank(G), 0);
	pokemonDestroy(G);
	return result;
}
/*
static bool testPokemonDestroy() {
	bool result = true;
	Pokemon A=pokemonCreate("A", TYPE_GRASS,200,5);
	Pokemon B=pokemonCreate("B", TYPE_WATER,150,5);
	pokemonTeachMove(A,"A1",TYPE_NORMAL,50,40);
	pokemonDestroy(A);
	TEST_DIFFERENT(result, pokemonGetLevel(A), 2);
	TEST_EQUALS(result, pokemonUseMove(A,B,"A1"), POKEMON_NULL_ARG);
	pokemonDestroy(B);
	return result;
}
*/
static bool testPokemonCopy() {
    bool result = true;
    TEST_EQUALS(result,pokemonCopy(NULL),NULL);
    Pokemon pk = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
    Pokemon pk_Copy2 = pokemonCopy(pk);
    TEST_EQUALS(result,pokemonGetLevel(pk),pokemonGetLevel(pk_Copy2));
    TEST_EQUALS(result,pokemonGetRank(pk),pokemonGetRank(pk_Copy2));
    pokemonDestroy(pk);
    pokemonDestroy(pk_Copy2);
    return result;
}
static bool testPokemonTeachMove() {
	bool result = true;

	Pokemon A=pokemonCreate(NULL, TYPE_GRASS,200,5);
	TEST_EQUALS(result, pokemonTeachMove(A,"A1",TYPE_NORMAL,50,70), POKEMON_NULL_ARG);
	pokemonDestroy(A);
	Pokemon B=pokemonCreate("B", TYPE_GRASS,200,5);
	TEST_EQUALS(result, pokemonTeachMove(B,NULL,TYPE_NORMAL,50,70), POKEMON_NULL_ARG);
	TEST_EQUALS(result, pokemonTeachMove(B,"",TYPE_NORMAL,50,70), POKEMON_INVALID_MOVE_NAME);
	TEST_EQUALS(result, pokemonTeachMove(B,"B1",6,50,70), POKEMON_INVALID_TYPE);
	TEST_EQUALS(result, pokemonTeachMove(B,"B1",TYPE_NORMAL,0,70), POKEMON_INVALID_POWER_POINTS);
	TEST_EQUALS(result, pokemonTeachMove(B,"B1",TYPE_NORMAL,50,-1), POKEMON_INVALID_STRENGTH);
	TEST_EQUALS(result, pokemonTeachMove(B,"B1",TYPE_GRASS,50,70), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonTeachMove(B,"B2",TYPE_NORMAL,50,70), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonTeachMove(B,"B2",TYPE_NORMAL,50,70), POKEMON_MOVE_ALREADY_EXISTS);
	TEST_EQUALS(result, pokemonTeachMove(B,"B3",TYPE_FIRE,50,70), POKEMON_INCOMPATIBLE_MOVE_TYPE);
	TEST_DIFFERENT(result, pokemonTeachMove(B,"B3",TYPE_GRASS,50,70), POKEMON_OUT_OF_MEM);
	Pokemon C=pokemonCreate("C", TYPE_WATER,300,5);
	TEST_EQUALS(result, pokemonUseMove(B,C,"B2"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonGetLevel(B), 3);
	TEST_EQUALS(result, pokemonGetRank(B), 73);
	pokemonDestroy(B);
	pokemonDestroy(C);
	return result;
}
static bool testPokemonUnteachMove() {
	bool result = true;
    Pokemon pk = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
    TEST_EQUALS(result,pokemonUnteachMove(NULL,"dkfjgh"),POKEMON_NULL_ARG);
    TEST_EQUALS(result,pokemonUnteachMove(pk,NULL),POKEMON_NULL_ARG);
    TEST_EQUALS(result,pokemonUnteachMove(pk,""),POKEMON_INVALID_MOVE_NAME);
    pokemonTeachMove(pk,"mymove1",TYPE_NORMAL,1,11);
    pokemonTeachMove(pk,"mymove2",TYPE_ELECTRIC,2,22);
    pokemonTeachMove(pk,"mymove3",TYPE_NORMAL,3,33);
    pokemonTeachMove(pk,"mymove4",TYPE_ELECTRIC,4,44);
    pokemonTeachMove(pk,"mymove5",TYPE_NORMAL,5,55);
    TEST_EQUALS(result,pokemonUnteachMove(pk,"mymove6"),POKEMON_MOVE_DOES_NOT_EXIST);
    TEST_EQUALS(result,pokemonUnteachMove(pk,"mymove3"),POKEMON_SUCCESS);
    TEST_EQUALS(result,pokemonUnteachMove(pk,"mymove3"),POKEMON_MOVE_DOES_NOT_EXIST);
	pokemonDestroy(pk);
	return result;
}
static bool testPokemonGetLevel() {
	bool result = true;
	Pokemon A=pokemonCreate("A", TYPE_GRASS,9900,5);
	pokemonTeachMove(A,"A1",TYPE_GRASS,50,70);
	TEST_EQUALS(result, pokemonGetLevel(A), 99);
	Pokemon B=pokemonCreate("B", TYPE_WATER,301,5);
	TEST_EQUALS(result, pokemonGetLevel(B), 4);
	TEST_EQUALS(result, pokemonUseMove(A,B,"A1"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonGetLevel(A), 100);
	pokemonDestroy(A);
	pokemonDestroy(B);
	return result;
}

static bool testPokemonGetRank() {
	bool result = true;
	Pokemon A=pokemonCreate("A", TYPE_GRASS,9901,5);
	TEST_EQUALS(result, pokemonGetRank(A), 0);
	pokemonTeachMove(A,"A1",TYPE_GRASS,50,70);
	TEST_EQUALS(result, pokemonGetRank(A), 170);
	pokemonTeachMove(A,"A2",TYPE_NORMAL,50,30);
	TEST_EQUALS(result, pokemonGetRank(A), 150);
	pokemonDestroy(A);
	return result;
}
static bool testPokemonUseMove() {
	bool result = true;

    Pokemon pk = pokemonCreate("Pikachu", TYPE_ELECTRIC, 9901, 7);
    Pokemon pk1 = pokemonCreate("Balbazur", TYPE_ELECTRIC, 20, 7);
    TEST_EQUALS(result,pokemonUseMove(NULL,pk1,"movename1"),POKEMON_NULL_ARG);
    TEST_EQUALS(result,pokemonUseMove(pk1,NULL,"movename1"),POKEMON_NULL_ARG);
    TEST_EQUALS(result,pokemonUseMove(pk,pk1,NULL),POKEMON_NULL_ARG);
    TEST_EQUALS(result,pokemonUseMove(pk,pk1,NULL),POKEMON_NULL_ARG);
    TEST_EQUALS(result,pokemonUseMove(pk,pk1,""),POKEMON_INVALID_MOVE_NAME);
    pokemonTeachMove(pk,"mymove1",TYPE_NORMAL,1,2000);
    pokemonTeachMove(pk1,"mymove11",TYPE_ELECTRIC,2,22);
    TEST_EQUALS(result,pokemonUseMove(pk,pk1,"mymove1"),POKEMON_SUCCESS);
    pokemonTeachMove(pk,"mymove2",TYPE_ELECTRIC,2,22);
    pokemonTeachMove(pk,"mymove3",TYPE_NORMAL,3,33);
    pokemonTeachMove(pk,"mymove4",TYPE_ELECTRIC,4,44);
    pokemonTeachMove(pk,"mymove5",TYPE_NORMAL,5,55);
    TEST_EQUALS(result,pokemonUseMove(pk,pk1,"mymove6"),POKEMON_MOVE_DOES_NOT_EXIST);
    TEST_EQUALS(result,pokemonUseMove(pk,pk1,"mymove1"),POKEMON_NO_POWER_POINTS);
    TEST_EQUALS(result,pokemonUseMove(pk,pk1,"mymove2"),POKEMON_NO_HEALTH_POINTS);
    TEST_EQUALS(result,pokemonUseMove(pk1,pk,"mymove11"),POKEMON_NO_HEALTH_POINTS);
	pokemonDestroy(pk);
	pokemonDestroy(pk1);
	return result;
}
static bool testPokemonHeal() {
	bool result = true;
	Pokemon A=pokemonCreate(NULL, TYPE_GRASS,200,5);
	TEST_EQUALS(result, pokemonHeal(A), POKEMON_NULL_ARG);
	pokemonDestroy(A);
	Pokemon B=pokemonCreate("B", TYPE_GRASS,200,5);
	Pokemon C=pokemonCreate("C", TYPE_WATER,150,5);
	pokemonTeachMove(B,"B1",TYPE_GRASS,50,40);
	pokemonUseMove(B,C,"B1");
	TEST_EQUALS(result, pokemonHeal(C), POKEMON_SUCCESS);
	pokemonDestroy(B);
	pokemonDestroy(C);
	return result;
}

static bool testPokemonEvolve() {
	bool result = true;
	Pokemon A=pokemonCreate(NULL, TYPE_GRASS,200,5);
	TEST_EQUALS(result, pokemonEvolve(A,"A~"), POKEMON_NULL_ARG);
	pokemonDestroy(A);
	Pokemon B=pokemonCreate("B", TYPE_GRASS,200,5);
	TEST_EQUALS(result, pokemonEvolve(B,NULL), POKEMON_NULL_ARG);
	pokemonDestroy(B);
	Pokemon C=pokemonCreate("C", TYPE_GRASS,200,5);
	TEST_EQUALS(result, pokemonEvolve(C,""), POKEMON_INVALID_NAME);
	pokemonDestroy(C);
	Pokemon D=pokemonCreate("D", TYPE_GRASS,9901,5);
	TEST_EQUALS(result, pokemonEvolve(D,"D~"), POKEMON_CANNOT_EVOLVE);
	pokemonDestroy(D);
	Pokemon E=pokemonCreate("E", TYPE_GRASS,200,5);
	TEST_DIFFERENT(result, pokemonEvolve(E,"E~"), POKEMON_OUT_OF_MEM);
	TEST_EQUALS(result, pokemonEvolve(E,"E#"), POKEMON_SUCCESS);
	pokemonDestroy(E);
	return result;
}

static bool testPokemonPrintName() {
	bool result = true;
	FILE* file=fopen("testPrintName.txt","w");
	Pokemon A=pokemonCreate(NULL, TYPE_GRASS,200,5);
	TEST_EQUALS(result, pokemonPrintName(A,file), POKEMON_NULL_ARG);
	pokemonDestroy(A);
	Pokemon B=pokemonCreate("B", TYPE_GRASS,200,5);
	TEST_EQUALS(result, pokemonPrintName(B,NULL), POKEMON_NULL_ARG);
	TEST_EQUALS(result, pokemonPrintName(B,file), POKEMON_SUCCESS);
	pokemonDestroy(B);
	fclose(file);
	return result;
}

static bool testPokemonPrintVoice() {
	bool result = true;
	FILE* file=fopen("testPrintVoice.txt","w");
	Pokemon Pikachu=pokemonCreate(NULL, TYPE_GRASS,200,5);
	TEST_EQUALS(result, pokemonPrintVoice(Pikachu,file), POKEMON_NULL_ARG);
	pokemonDestroy(Pikachu);
	Pokemon Bulbasaur_Pikachuli=pokemonCreate("Bulbasaur_Pikachuli", TYPE_GRASS,200,5);
	TEST_EQUALS(result, pokemonPrintVoice(Bulbasaur_Pikachuli,NULL), POKEMON_NULL_ARG);
	TEST_DIFFERENT(result, pokemonPrintVoice(Bulbasaur_Pikachuli,file), POKEMON_OUT_OF_MEM);
	TEST_EQUALS(result, pokemonPrintVoice(Bulbasaur_Pikachuli,file), POKEMON_SUCCESS);
	pokemonDestroy(Bulbasaur_Pikachuli);
	fclose(file);
	return result;
}


int main() {
	RUN_TEST(testCombo);
	RUN_TEST(testPokemonCreate);
	//RUN_TEST(testPokemonDestroy);
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
