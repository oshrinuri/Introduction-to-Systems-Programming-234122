#include "test_utils.h"
#include "trainer.h"
#include "exceptions.h"

using namespace mtm::pokemongo;
using namespace std;

//------------------------------------------------------------------------------
bool TestTrainerConstructor() {
    ASSERT_NO_THROW(Trainer("Ash", RED));
    ASSERT_THROW(TrainerInvalidArgsException, Trainer("", RED));
    return true;
}
//------------------------------------------------------------------------------
bool TestTrainerCopyConstructor() {
    const Trainer ash("Ash", RED);
    const Trainer ash_copy(ash);
    return true;
}//------------------------------------------------------------------------------
bool TestTrainerGetStrongestPokemon() {
    Trainer ash("Ash", RED);
    ASSERT_THROW(TrainerNoPokemonsFoundException, ash.GetStrongestPokemon());
    return true;
}
//------------------------------------------------------------------------------
bool TestTrainerKillStrongestPokemon() {
    Trainer ash("Ash", RED);
    ASSERT_THROW(TrainerNoPokemonsFoundException, ash.KillStrongestPokemon());
    return true;
}
//------------------------------------------------------------------------------
bool TestTrainerIsAlly() {
    const Trainer ash("Ash", RED);
    const Trainer gary("Gary", BLUE);
    const Trainer misty("Misty", RED);
    ASSERT_EQUAL(false, ash.IsAlly(gary));
    ASSERT_EQUAL(true, ash.IsAlly(misty));
    return true;
}
//------------------------------------------------------------------------------
bool TestTrainerGetTeam() {
    const Trainer ash("Ash", RED);
    const Trainer gary("Gary", BLUE);
    const Trainer misty("Misty", YELLOW);
    ASSERT_EQUAL(RED, ash.GetTeam());
    ASSERT_EQUAL(BLUE, gary.GetTeam());
    ASSERT_EQUAL(YELLOW, misty.GetTeam());
    return true;
}
//------------------------------------------------------------------------------