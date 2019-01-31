// -------------------------------------------------------------------------- //
//                             INCLUDES & DEFINES                             //
// -------------------------------------------------------------------------- //
#include "pokestop.h"

using namespace mtm::pokemongo;
using namespace std;

typedef list<Item*>::iterator Items_Iterator;

// -------------------------------------------------------------------------- //
//                                FUNCTIONS                                   //
// -------------------------------------------------------------------------- //
void Pokestop::Arrive(Trainer& trainer) {
    Items_Iterator first_item_ptr = items.begin();
    const Item& first_item = **first_item_ptr;
    if (trainer.Level() > first_item.Level()) {
        trainer.GetItems().push_front(*first_item_ptr);
        items.remove(*first_item_ptr);
    }
    Location::Arrive(trainer);
}
//------------------------------------------------------------------------------
void Pokestop::Leave(Trainer& trainer) {
    Location::Leave(trainer);
}
//------------------------------------------------------------------------------