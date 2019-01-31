#include "test_utils.h"
#include "../pokemon.h"
#include "../exceptions.h"

using namespace mtm::pokemongo;
using namespace std;
//------------------------------------------------------------------------------
bool TestPokemonConstructorI() {
    const PokemonType types_arr[] = {FLYING, ICE, GROUND, NORMAL};
    const set <PokemonType> types(types_arr, types_arr + 4);
    ASSERT_NO_THROW(Pokemon("Articuno", types, 100.0, 1));
    ASSERT_THROW(PokemonInvalidArgsException,
                 Pokemon("Articuno", types, 15.0, -1));
    ASSERT_THROW(PokemonInvalidArgsException,
                 Pokemon("Articuno", types, -1, 1));
    ASSERT_THROW(PokemonInvalidArgsException, Pokemon("", types, 15.0, 1));
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonConstructorII() {
    ASSERT_THROW(PokemonInvalidArgsException, Pokemon("Pikachu", 15.0, -1));
    ASSERT_THROW(PokemonInvalidArgsException, Pokemon("Pikachu", 15.0, -1));
    ASSERT_THROW(PokemonInvalidArgsException, Pokemon("", 15.0, 1));
    ASSERT_NO_THROW(Pokemon("Pikachu", 15.0, 1));
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonDestructor() {
    const Pokemon *pokemon = NULL;
    ASSERT_NO_THROW(pokemon = new Pokemon("Pikachu", 15.0, 3));
    ASSERT_EQUAL(pokemon->Level(), 3);
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonCopyConstructor() {
    const Pokemon pikachu("Pikachu", 15.0, 3);
    const Pokemon pikachu_copy(pikachu);
    ASSERT_EQUAL(pikachu_copy.Level(), pikachu.Level());
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonAssignmentOperator() {
    const Pokemon pikachu("Pikachu", 15.0, 3);
    Pokemon bulbasor("Bulbasor", 12.0, 2);
    ASSERT_NO_THROW(bulbasor = pikachu);
    ASSERT_EQUAL(bulbasor.Level(), 3);
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonEqualOperator() {
    const PokemonType pikachu_types_arr[] = {NORMAL, ELECTRIC};
    const set <PokemonType> pikachu_types(pikachu_types_arr,
                                          pikachu_types_arr + 1);
    const Pokemon pikachu("Pikachu", pikachu_types, 15.0, 3);

    const PokemonType raichu_types_arr[] = {NORMAL, ELECTRIC};
    const set <PokemonType> raichu_types(raichu_types_arr,
                                         raichu_types_arr + 1);
    const Pokemon raichu("Raichu", raichu_types, 15.0, 3);

    ASSERT_EQUAL(pikachu == raichu, true);
    ASSERT_EQUAL(pikachu == pikachu, true);
    ASSERT_EQUAL(pikachu == Pokemon("Pichu", 3.0, 1), false);
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonNotEqualOperator() {
    const PokemonType pikachu_types_arr[] = {NORMAL, ELECTRIC};
    const set <PokemonType> pikachu_types(pikachu_types_arr,
                                          pikachu_types_arr + 1);
    const Pokemon pikachu("Pikachu", pikachu_types, 15.0, 3);

    const PokemonType bulbasor_types_arr[] = {NORMAL, GRASS, BUG};
    const set <PokemonType> bulbasor_types(bulbasor_types_arr,
                                           bulbasor_types_arr + 2);
    const Pokemon bulbasor("Bulbasor", bulbasor_types, 15.0, 3);

    ASSERT_EQUAL(pikachu != bulbasor, true);
    ASSERT_EQUAL(bulbasor != bulbasor, false);
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonSmallerOperator() {
    const PokemonType mewtwo_types_arr[] = {NORMAL, ELECTRIC, FIRE, PSYCHIC,
                                            WATER, ICE};
    const set <PokemonType> mewtwo_types(mewtwo_types_arr,
                                         mewtwo_types_arr + 6);
    const Pokemon mewtwo("Meotwo", mewtwo_types, 100.0, 100);

    const PokemonType bulbasor_types_arr[] = {NORMAL, GRASS, BUG};
    const set <PokemonType> bulbasor_types(bulbasor_types_arr,
                                           bulbasor_types_arr + 2);
    const Pokemon bulbasor("Bulbasor", bulbasor_types, 15.0, 3);

    ASSERT_EQUAL(bulbasor < mewtwo, true);
    ASSERT_EQUAL(bulbasor < bulbasor, false);
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonBiggerOperator() {
    const PokemonType mewtwo_types_arr[] = {NORMAL, ELECTRIC, FIRE, PSYCHIC,
                                            WATER, ICE};
    const set <PokemonType> mewtwo_types(mewtwo_types_arr,
                                         mewtwo_types_arr + 6);
    const Pokemon mewtwo("Meotwo", mewtwo_types, 100.0, 100);

    const PokemonType bulbasor_types_arr[] = {NORMAL, GRASS, BUG};
    const set <PokemonType> bulbasor_types(bulbasor_types_arr,
                                           bulbasor_types_arr + 2);
    const Pokemon bulbasor("Bulbasor", bulbasor_types, 15.0, 3);

    ASSERT_EQUAL(mewtwo > bulbasor, true);
    ASSERT_EQUAL(mewtwo > mewtwo, false);
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonSmallerEqualOperator() {
    const PokemonType bulbasor_types_arr[] = {NORMAL, GRASS, BUG};
    const set <PokemonType> bulbasor_types(bulbasor_types_arr,
                                           bulbasor_types_arr + 2);
    const Pokemon bulbasor("Bulbasor", bulbasor_types, 15.0, 3);

    ASSERT_EQUAL(bulbasor >= bulbasor, true);
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonBiggerEqualOperator() {
    const PokemonType bulbasor_types_arr[] = {NORMAL, GRASS, BUG};
    const set <PokemonType> bulbasor_types(bulbasor_types_arr,
                                           bulbasor_types_arr + 2);
    const Pokemon bulbasor("Bulbasor", bulbasor_types, 15.0, 3);

    ASSERT_EQUAL(bulbasor <= bulbasor, true);
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonLevel() {
    const Pokemon pikachu("Pikachu", 15.0, 3);
    ASSERT_EQUAL(pikachu.Level(), 3);
    ASSERT_THROW(PokemonInvalidArgsException, Pokemon("Pichu", 12.0, 0));
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonHit() {
    Pokemon pikachu("Pikachu", 15.0, 3);
    ASSERT_EQUAL(pikachu.Hit(pikachu), false);
    ASSERT_EQUAL(pikachu.Hit(pikachu), false);
    ASSERT_EQUAL(pikachu.Hit(pikachu), true);
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonHeal() {
    Pokemon pikachu("Pikachu", 15.0, 3);
    ASSERT_EQUAL(pikachu.Hit(pikachu), false);
    ASSERT_EQUAL(pikachu.Hit(pikachu), false);
    ASSERT_EQUAL(pikachu.Hit(pikachu), true);
    pikachu.Heal();
    ASSERT_EQUAL(pikachu.Hit(pikachu), false);
    ASSERT_EQUAL(pikachu.Hit(pikachu), false);
    ASSERT_EQUAL(pikachu.Hit(pikachu), true);
    return true;
}
//------------------------------------------------------------------------------
bool TestPokemonTrain() {
    Pokemon pikachu("Pikachu", 15.0, 3);
    ASSERT_THROW(PokemonInvalidArgsException, pikachu.Train(-0.5));
    ASSERT_THROW(PokemonInvalidArgsException, pikachu.Train(0));
    ASSERT_THROW(PokemonInvalidArgsException, pikachu.Train(0.5));
    ASSERT_THROW(PokemonInvalidArgsException, pikachu.Train(1));
    return true;
}
//------------------------------------------------------------------------------