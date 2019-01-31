// -------------------------------------------------------------------------- //
//                             INCLUDES & DEFINES                             //
// -------------------------------------------------------------------------- //
#include "starbucks.h"

using namespace mtm::pokemongo;
using namespace std;

// -------------------------------------------------------------------------- //
//                                FUNCTIONS                                   //
// -------------------------------------------------------------------------- //
void Starbucks::Arrive(Trainer& trainer) {
    Pokemon& first_pokemon = pokemons.front();
    if (trainer.TryToCatch(first_pokemon)) {
        this->pokemons.remove(first_pokemon);
    }
    Location::Arrive(trainer);
}
//------------------------------------------------------------------------------
void Starbucks::Leave(Trainer& trainer) {
    Location::Leave(trainer);
}
//------------------------------------------------------------------------------