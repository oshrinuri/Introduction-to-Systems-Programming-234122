#include <stdbool.h>
#include "aux_macros.h"
#include "pokemon_trainer.h"
// or and assaf

static bool testCombo() {
	bool result = true;
	Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pikachu, 6, 20);
	TEST_DIFFERENT(result, trainer, NULL);
	pokemonDestroy(pikachu);
	PokemonTrainer trainer_copy = pokemonTrainerCopy(trainer);
	TEST_DIFFERENT(result, trainer_copy, NULL);
	pokemonTrainerDestroy(trainer_copy);
	Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 10, 4);
	TEST_EQUALS(result, pokemonTrainerAddPokemon(trainer, squirtle),
		POKEMON_TRAINER_SUCCESS);
	pokemonDestroy(squirtle);
	TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 2);
	TEST_DIFFERENT(result, pokemonTrainerGetPokemon(trainer, 2), NULL);
	TEST_EQUALS(result, pokemonTrainerDepositPokemon(trainer, 2),
		POKEMON_TRAINER_SUCCESS);
	TEST_EQUALS(result, pokemonTrainerWithdrawPokemon(trainer, 1),
		POKEMON_TRAINER_SUCCESS);
	TEST_EQUALS(result, pokemonTrainerRemovePokemon(trainer, 2),
		POKEMON_TRAINER_SUCCESS);
	TEST_EQUALS(result, pokemonTrainerGetMostRankedPokemon(trainer),
		pokemonTrainerGetPokemon(trainer, 1));
	TEST_EQUALS(result, pokemonTrainerMakeMostRankedParty(trainer),
		POKEMON_TRAINER_SUCCESS);
	pokemonTrainerDestroy(trainer);
	return result;
}

static bool pokemonTrainerCreateTest (){
    bool result = true;
    Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
    TEST_EQUALS(result,pokemonTrainerCreate("", pikachu, 6, 20),NULL);
    TEST_EQUALS(result,pokemonTrainerCreate(NULL, pikachu, 6, 20),NULL);
    TEST_EQUALS(result,pokemonTrainerCreate("Ash", NULL, 6, 20),NULL);
    TEST_EQUALS(result,pokemonTrainerCreate("Ash", pikachu, -6, 20),NULL);
    TEST_EQUALS(result,pokemonTrainerCreate("Ash", pikachu, 0, 20),NULL);
    TEST_EQUALS(result,pokemonTrainerCreate("Ash", pikachu, 6, -20),NULL);
    TEST_EQUALS(result,pokemonTrainerCreate("Ash", pikachu, 6, 0),NULL);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pikachu, 6, 20);
	TEST_DIFFERENT(result,trainer,NULL);
    pokemonTrainerDestroy(trainer);
    pokemonDestroy(pikachu);
    return result;
}

static bool pokemonTrainerDestroyTest () {
	bool result = true;
	Pokemon p1 = pokemonCreate("p1", TYPE_ELECTRIC, 200, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", p1, 6, 20);
    Pokemon p2 = pokemonCreate("p2",TYPE_GRASS,30,5);
	pokemonTrainerAddPokemon(trainer,p2);
	pokemonTrainerDestroy(trainer);
    TEST_EQUALS(result,pokemonGetLevel(p1),2);
    pokemonDestroy(p1);
    pokemonDestroy(p2);
    return result;
}

static bool pokemonTrainerCopyTest() {
	bool result = true;
	Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer a = pokemonTrainerCreate(NULL, pikachu, 6, 20);
	PokemonTrainer a_Copy = pokemonTrainerCopy(a);
	TEST_EQUALS(result, a_Copy, NULL);
	pokemonTrainerDestroy(a_Copy);
	pokemonTrainerDestroy(a);
	PokemonTrainer b = pokemonTrainerCreate("B", pikachu, 6, 20);
	PokemonTrainer b_Copy = pokemonTrainerCopy(b);
	TEST_DIFFERENT(result, b_Copy, NULL);
	pokemonTrainerDestroy(b_Copy);
	pokemonTrainerDestroy(b);
	pokemonDestroy(pikachu);
	return result;
}

static bool pokemonTrainerAddPokemonTest(){
    bool result = true;
    Pokemon p1 = pokemonCreate("p1",TYPE_GRASS,30,5);
    PokemonTrainer trainer = pokemonTrainerCreate("Ash", p1, 6, 20);
    Pokemon p2 = pokemonCreate("p2",TYPE_ELECTRIC,31,6);
    TEST_EQUALS(result,pokemonTrainerAddPokemon(trainer,NULL),POKEMON_TRAINER_NULL_ARG);
    TEST_EQUALS(result,pokemonTrainerAddPokemon(NULL,p2),POKEMON_TRAINER_NULL_ARG);
    Pokemon p3 = pokemonCreate("p3",TYPE_FIRE,32,7);
    Pokemon p4 = pokemonCreate("p4",TYPE_NORMAL,33,8);
    Pokemon p5 = pokemonCreate("p5",TYPE_WATER,34,9);
    TEST_EQUALS(result,pokemonTrainerAddPokemon(trainer,p2),POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result,pokemonTrainerAddPokemon(trainer,p3),POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result,pokemonTrainerAddPokemon(trainer,p4),POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result,pokemonTrainerAddPokemon(trainer,p5),POKEMON_TRAINER_SUCCESS);
    pokemonTeachMove(p1,"move1",TYPE_FIRE,10,10);
    pokemonTeachMove(p1,"move2",TYPE_NORMAL,10,10);
    Pokemon pTrainer = pokemonTrainerGetPokemon(trainer,1);
    TEST_EQUALS(result,pokemonUnteachMove(pTrainer,"move1"),POKEMON_MOVE_DOES_NOT_EXIST);
    TEST_EQUALS(result,pokemonUnteachMove(pTrainer,"move2"),POKEMON_MOVE_DOES_NOT_EXIST);
    pokemonDestroy(p1);
    pokemonDestroy(p2);
    pokemonDestroy(p3);
    pokemonDestroy(p4);
    pokemonDestroy(p5);
    Pokemon p6 = pokemonCreate("p6",TYPE_WATER,34,9);
    TEST_EQUALS(result,pokemonTrainerAddPokemon(trainer,p6),POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result,pokemonTrainerGetNumberOfPokemons(trainer),6);
    Pokemon p7 = pokemonCreate("p7",TYPE_WATER,34,9);
    TEST_EQUALS(result,pokemonTrainerAddPokemon(trainer,p7),POKEMON_TRAINER_PARTY_FULL);
    pokemonTrainerDestroy(trainer);
    pokemonDestroy(p7);
    pokemonDestroy(p6);
    return result;

}
static bool pokemonTrainerGetPokemonTest(){
    bool result = true;
    Pokemon p1 = pokemonCreate("p1",TYPE_GRASS,30,5);
    pokemonTeachMove(p1,"mymove1",TYPE_ELECTRIC,2,22);
    pokemonTeachMove(p1,"mymove2",TYPE_NORMAL,3,33);
    pokemonTeachMove(p1,"mymove3",TYPE_ELECTRIC,4,44);
    pokemonTeachMove(p1,"mymove4",TYPE_NORMAL,5,55);
    PokemonTrainer trainer = pokemonTrainerCreate("Ash", p1, 6, 20);
    TEST_EQUALS(result,pokemonTrainerGetPokemon(NULL,1),NULL);
    TEST_EQUALS(result,pokemonTrainerGetPokemon(trainer,0),NULL);
    TEST_EQUALS(result,pokemonTrainerGetPokemon(trainer,-1),NULL);
     Pokemon p2 = pokemonCreate("p2",TYPE_ELECTRIC,31,6);
    Pokemon p3 = pokemonCreate("p3",TYPE_FIRE,32,7);
    Pokemon p4 = pokemonCreate("p4",TYPE_NORMAL,200,8);
    Pokemon p5 = pokemonCreate("p5",TYPE_WATER,34,9);
    pokemonTrainerAddPokemon(trainer,p2);
    pokemonTrainerAddPokemon(trainer,p3);
    pokemonTrainerAddPokemon(trainer,p4);
    pokemonTrainerAddPokemon(trainer,p5);
    TEST_EQUALS(result,pokemonTrainerGetPokemon(trainer,6),NULL);
    Pokemon tp = pokemonTrainerGetPokemon(trainer,1);
    TEST_DIFFERENT(result,tp,NULL);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,1)),pokemonGetLevel(tp));
    TEST_EQUALS(result,pokemonGetRank(pokemonTrainerGetPokemon(trainer,1)),pokemonGetRank(tp));
    pokemonTeachMove(tp,"mymove6",TYPE_ELECTRIC,6,66);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,1)),pokemonGetLevel(tp));
    TEST_EQUALS(result,pokemonGetRank(pokemonTrainerGetPokemon(trainer,1)),pokemonGetRank(tp));
    pokemonTrainerDestroy(trainer);
    pokemonDestroy(p1);
    pokemonDestroy(p2);
    pokemonDestroy(p3);
    pokemonDestroy(p4);
    pokemonDestroy(p5);
    return result;
}

static bool pokemonTrainerRemovePokemonTest(){
    bool result = true;
    Pokemon p1 = pokemonCreate("p1",TYPE_GRASS,800,5);
    pokemonTeachMove(p1,"mymove1",TYPE_NORMAL,2,22);
    pokemonTeachMove(p1,"mymove2",TYPE_NORMAL,3,33);
    pokemonTeachMove(p1,"mymove3",TYPE_NORMAL,4,44);
    pokemonTeachMove(p1,"mymove4",TYPE_NORMAL,5,55);
    PokemonTrainer trainer = pokemonTrainerCreate("Ash", p1, 6, 20);
    //TEST_EQUALS(result,pokemonTrainerRemovePokemon(trainer,1),POKEMON_TRAINER_REMOVE_LAST);
     Pokemon p2 = pokemonCreate("p2",TYPE_ELECTRIC,700,6);
    Pokemon p3 =pokemonCreate("p3",TYPE_FIRE,32,7);
    Pokemon p4 =pokemonCreate("p4",TYPE_NORMAL,200,8);
    Pokemon p5 = pokemonCreate("p5",TYPE_WATER,600,9);
    pokemonTrainerAddPokemon(trainer,p2);
    pokemonTrainerAddPokemon(trainer,p3);
    pokemonTrainerAddPokemon(trainer,p4);
    pokemonTrainerAddPokemon(trainer,p5);    
	TEST_EQUALS(result,pokemonTrainerRemovePokemon(NULL,3),POKEMON_TRAINER_NULL_ARG);
    TEST_EQUALS(result,pokemonTrainerRemovePokemon(trainer,0),POKEMON_TRAINER_INVALID_INDEX);
    TEST_EQUALS(result,pokemonTrainerRemovePokemon(trainer,-1),POKEMON_TRAINER_INVALID_INDEX);
    TEST_EQUALS(result,pokemonTrainerRemovePokemon(trainer,6),POKEMON_TRAINER_INVALID_INDEX);
    TEST_EQUALS(result,pokemonTrainerRemovePokemon(trainer,3),POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result,pokemonTrainerGetNumberOfPokemons(trainer),4);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,4)),6);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,3)),2);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,2)),7);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,1)),8);
    TEST_EQUALS(result,pokemonTrainerRemovePokemon(trainer,1),POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result,pokemonTrainerGetNumberOfPokemons(trainer),3);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,1)),7);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,2)),2);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,3)),6);
    pokemonDestroy(p1);
    pokemonDestroy(p2);
    pokemonDestroy(p3);
    pokemonDestroy(p4);
    pokemonDestroy(p5);
    pokemonTrainerDestroy(trainer);
    return result;
}

static bool pokemonTrainerDepositPokemonTest(){
    bool result = true;
    Pokemon p1 = pokemonCreate("p1",TYPE_GRASS,800,5);
    pokemonTeachMove(p1,"mymove1",TYPE_GRASS,2,22);
    pokemonTeachMove(p1,"mymove2",TYPE_GRASS,3,33);
    pokemonTeachMove(p1,"mymove3",TYPE_GRASS,4,44);
    pokemonTeachMove(p1,"mymove4",TYPE_NORMAL,5,55);
    PokemonTrainer trainer = pokemonTrainerCreate("Ash", p1, 6, 2);
    TEST_EQUALS(result,pokemonTrainerDepositPokemon(trainer,1),POKEMON_TRAINER_DEPOSIT_LAST);

    Pokemon p2 = pokemonCreate("p2",TYPE_ELECTRIC,700,6);
    Pokemon p3 = pokemonCreate("p3",TYPE_FIRE,300,7);
    Pokemon p4 = pokemonCreate("p4",TYPE_NORMAL,200,8);
    Pokemon p5 = pokemonCreate("p5",TYPE_WATER,600,9);
    pokemonTrainerAddPokemon(trainer,p2);
    pokemonTrainerAddPokemon(trainer,p3);
    pokemonTrainerAddPokemon(trainer,p4);
    pokemonTrainerAddPokemon(trainer,p5);

    TEST_EQUALS(result,pokemonTrainerDepositPokemon(NULL,3),POKEMON_TRAINER_NULL_ARG);
    TEST_EQUALS(result,pokemonTrainerDepositPokemon(trainer,0),POKEMON_TRAINER_INVALID_INDEX);
    TEST_EQUALS(result,pokemonTrainerDepositPokemon(trainer,-1),POKEMON_TRAINER_INVALID_INDEX);
    TEST_EQUALS(result,pokemonTrainerDepositPokemon(trainer,6),POKEMON_TRAINER_INVALID_INDEX);
    TEST_EQUALS(result,pokemonTrainerDepositPokemon(trainer,3),POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result,pokemonTrainerGetNumberOfPokemons(trainer),5);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,4)),6);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,3)),2);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,2)),7);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,1)),8);
    TEST_EQUALS(result,pokemonTrainerDepositPokemon(trainer,1),POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result,pokemonTrainerGetNumberOfPokemons(trainer),5);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,1)),7);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,2)),2);
    TEST_EQUALS(result,pokemonGetLevel(pokemonTrainerGetPokemon(trainer,3)),6);
    pokemonDestroy(p1);
    pokemonDestroy(p2);
    pokemonDestroy(p3);
    pokemonDestroy(p4);
    pokemonDestroy(p5);
    pokemonTrainerDestroy(trainer);
    return result;
}

static bool pokemonTrainerWithdrawPokemonTest() {
    bool result = true;
    Pokemon p1 = pokemonCreate("p1",TYPE_GRASS,800,5);
    pokemonTeachMove(p1,"mymove1",TYPE_GRASS,2,22);
    pokemonTeachMove(p1,"mymove2",TYPE_GRASS,3,33);
    pokemonTeachMove(p1,"mymove3",TYPE_GRASS,4,44);
    pokemonTeachMove(p1,"mymove4",TYPE_NORMAL,5,55);
    PokemonTrainer trainer = pokemonTrainerCreate("Ash", p1, 5, 2);
    TEST_EQUALS(result,pokemonTrainerDepositPokemon(trainer,1),POKEMON_TRAINER_DEPOSIT_LAST);

    Pokemon p2 = pokemonCreate("p2",TYPE_ELECTRIC,700,6);
    Pokemon p3 = pokemonCreate("p3",TYPE_FIRE,300,7);
    Pokemon p4 = pokemonCreate("p4",TYPE_NORMAL,200,8);
    Pokemon p5 = pokemonCreate("p5",TYPE_WATER,600,9);
    pokemonTrainerAddPokemon(trainer,p2);
    pokemonTrainerAddPokemon(trainer,p3);
    pokemonTrainerAddPokemon(trainer,p4);
    pokemonTrainerAddPokemon(trainer,p5);

    TEST_EQUALS(result,pokemonTrainerWithdrawPokemon(NULL,3),POKEMON_TRAINER_NULL_ARG);
    TEST_EQUALS(result,pokemonTrainerWithdrawPokemon(trainer,0),POKEMON_TRAINER_INVALID_INDEX);
    TEST_EQUALS(result,pokemonTrainerWithdrawPokemon(trainer,-1),POKEMON_TRAINER_INVALID_INDEX);
    TEST_EQUALS(result,pokemonTrainerWithdrawPokemon(trainer,6),POKEMON_TRAINER_INVALID_INDEX);
    TEST_EQUALS(result,pokemonTrainerWithdrawPokemon(trainer,1),POKEMON_TRAINER_INVALID_INDEX);
    pokemonTrainerDepositPokemon(trainer,1);
    pokemonTrainerDepositPokemon(trainer,1);
    TEST_EQUALS(result,pokemonTrainerWithdrawPokemon(trainer,3),POKEMON_TRAINER_INVALID_INDEX);

    Pokemon p6 = pokemonCreate("p6",TYPE_FIRE,500,7);
    pokemonTrainerAddPokemon(trainer,p6);
    Pokemon p7 = pokemonCreate("p7",TYPE_FIRE,400,7);
    pokemonTrainerAddPokemon(trainer,p7);
    TEST_EQUALS(result,pokemonTrainerGetNumberOfPokemons(trainer),7);
    TEST_EQUALS(result,pokemonTrainerWithdrawPokemon(trainer,1),POKEMON_TRAINER_PARTY_FULL);
    pokemonTrainerRemovePokemon(trainer,1);
    TEST_EQUALS(result,pokemonTrainerWithdrawPokemon(trainer,1),POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result,pokemonTrainerWithdrawPokemon(trainer,1),POKEMON_TRAINER_PARTY_FULL);
    pokemonTrainerRemovePokemon(trainer,1);
    TEST_EQUALS(result,pokemonTrainerWithdrawPokemon(trainer,1),POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result,pokemonTrainerGetNumberOfPokemons(trainer),5);
    pokemonDestroy(p1);
    pokemonDestroy(p2);
    pokemonDestroy(p3);
    pokemonDestroy(p4);
    pokemonDestroy(p5);
    pokemonDestroy(p6);
    pokemonDestroy(p7);
    pokemonTrainerDestroy(trainer);
    return result;
}

static bool pokemonTrainerGetNumberOfPokemonsTest(){
    bool result = true;
    Pokemon p1 = pokemonCreate("p1",TYPE_GRASS,800,5);
    PokemonTrainer trainer = pokemonTrainerCreate("Ash", p1, 5, 2);

    Pokemon p2 = pokemonCreate("p2",TYPE_ELECTRIC,700,6);
 	Pokemon p3 = pokemonCreate("p3",TYPE_FIRE,300,7);
    Pokemon p4 = pokemonCreate("p4",TYPE_NORMAL,200,8);
    Pokemon p5 = pokemonCreate("p5",TYPE_WATER,600,9);
    pokemonTrainerAddPokemon(trainer,p2);
    pokemonTrainerAddPokemon(trainer,p3);
    pokemonTrainerAddPokemon(trainer,p4);
    pokemonTrainerAddPokemon(trainer,p5);

    TEST_EQUALS(result,pokemonTrainerGetNumberOfPokemons(trainer),5);
    pokemonTrainerRemovePokemon(trainer,2);
    pokemonTrainerRemovePokemon(trainer,2);
    TEST_EQUALS(result,pokemonTrainerGetNumberOfPokemons(trainer),3);
    pokemonDestroy(p1);
    pokemonDestroy(p2);
    pokemonDestroy(p3);
    pokemonDestroy(p4);
    pokemonDestroy(p5);
    pokemonTrainerDestroy(trainer);
    return result;
}

static bool pokemonTrainerPrintEnumerationTest() {
	bool result = true;
	Pokemon p1 = pokemonCreate("A", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer tr = pokemonTrainerCreate(NULL, p1, 6, 20);
	TEST_EQUALS(result, pokemonTrainerPrintEnumeration(tr, NULL),
			POKEMON_TRAINER_NULL_ARG);
	pokemonTrainerDestroy(tr);
	PokemonTrainer tr2 = pokemonTrainerCreate("C", p1, 20, 7);
	TEST_EQUALS(result, pokemonTrainerPrintEnumeration(tr2, NULL),
			POKEMON_TRAINER_NULL_ARG);
	pokemonTrainerDestroy(tr2);
	pokemonDestroy(p1);
	return result;
}

int main(){
    RUN_TEST(testCombo);
    RUN_TEST(pokemonTrainerCreateTest);
    RUN_TEST(pokemonTrainerDestroyTest);
    RUN_TEST(pokemonTrainerCopyTest);
    RUN_TEST(pokemonTrainerAddPokemonTest);
    RUN_TEST(pokemonTrainerGetPokemonTest);
    RUN_TEST(pokemonTrainerRemovePokemonTest);
    RUN_TEST(pokemonTrainerDepositPokemonTest);
    RUN_TEST(pokemonTrainerWithdrawPokemonTest);
    RUN_TEST(pokemonTrainerGetNumberOfPokemonsTest);
    RUN_TEST(pokemonTrainerPrintEnumerationTest);
    //RUN_TEST(testPokemonTrainerGetMostRankedPokemon);
    //RUN_TEST(testPokemonTrainerMakeMostRankedParty);
	return 0;
}
