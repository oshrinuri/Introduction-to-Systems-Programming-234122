// -------------------------------------------------------------------------- //
//                             INCLUDES & DEFINES                             //
// -------------------------------------------------------------------------- //
#include "pokemon.h"

using namespace mtm::pokemongo;
using namespace std;

static const int MAX_HP = 100;
static const int NO_HP = 0;

#define SET_FOREACH(set_type,itr,set_name) \
        for (set<set_type>::iterator itr = set_name.begin(); \
             itr!=set_name.end() ; ++itr)
// -------------------------------------------------------------------------- //
//                             AUX FUNCTIONS                                  //
// -------------------------------------------------------------------------- //
double Pokemon::calcTypesValue(const Pokemon& pokemon) {
    double sum = 0;
    SET_FOREACH(PokemonType,type_iterator,pokemon.types) {
        sum += *type_iterator;
    }
    return sum;
}
// -------------------------------------------------------------------------- //
//                             CONSTRUCTORS                                   //
// -------------------------------------------------------------------------- //
Pokemon::Pokemon(const std::string& species,
                 const std::set<PokemonType>& types,
                 const double& cp, const int& level) {
    if (species.empty() || cp < 0 || level <= 0) {
        throw PokemonInvalidArgsException();
    }
    this->species = species;
    this->types = types;
    this->cp = cp;
    this->hp = MAX_HP;
    this->level = level;
    this->hit_power = cp*level;
}
//------------------------------------------------------------------------------
Pokemon::Pokemon(const std::string& species, const double& cp,
                 const int& level) {
    if (species.empty() || cp < 0 || level <= 0) {
        throw PokemonInvalidArgsException();
    }
    this->species = species;
    this->cp = cp;
    this->hp = MAX_HP;
    this->level = level;
    this->types = GetDefaultTypes(species);
    this->hit_power = cp*level;
}
//------------------------------------------------------------------------------
Pokemon::Pokemon(const Pokemon& pokemon) {
    this->species = pokemon.species;
    this->level = pokemon.level;
    this->cp = pokemon.cp;
    this->hp = pokemon.hp;
    this->types = pokemon.types;
    this->hit_power = pokemon.hit_power;
}
// -------------------------------------------------------------------------- //
//                                FUNCTIONS                                   //
// -------------------------------------------------------------------------- //

void Pokemon::Train(const double& boost) {
    if (boost <= 1) {
        throw PokemonInvalidArgsException();
    }
    this->cp *= boost;
}
//------------------------------------------------------------------------------
void Pokemon::Heal() {
    this->hp = MAX_HP;
}
//------------------------------------------------------------------------------
bool Pokemon::Hit(Pokemon& victim) {
    victim.hp = victim.hp - this->hit_power;
    return victim.hp <= NO_HP;
}
// -------------------------------------------------------------------------- //
//                                OPERATORS                                   //
// -------------------------------------------------------------------------- //
Pokemon& Pokemon::operator=(const Pokemon& pokemon) {
    int new_level = pokemon.level;
    double new_hp = pokemon.hp;
    double new_cp = pokemon.cp;

    if (this == &pokemon) { // Self check
        return *this;
    }

    // Clear old data
    this->types.clear();
    this->species.clear();

    // Hard copy of data
    this->species = pokemon.species;
    this->types = pokemon.types;
    this->hp = new_hp;
    this->cp = new_cp;
    this->level = new_level;
    this->hit_power = new_hp*new_level;
    return *this;
}
//------------------------------------------------------------------------------
bool Pokemon::operator==(const Pokemon& rhs) const {
    if (this->hit_power == rhs.hit_power ) {
        if (calcTypesValue(*this) == calcTypesValue(rhs)) {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool Pokemon::operator!=(const Pokemon& rhs) const {
    return !(*this == rhs);
}
//------------------------------------------------------------------------------
bool Pokemon::operator<(const Pokemon& rhs) const {
    return this->hit_power < rhs.hit_power;
}
//------------------------------------------------------------------------------
bool Pokemon::operator>(const Pokemon& rhs) const {
    return this->hit_power > rhs.hit_power;
}
//------------------------------------------------------------------------------
bool Pokemon::operator<=(const Pokemon& rhs) const {
    return !(*this > rhs);
}
//------------------------------------------------------------------------------
bool Pokemon::operator>=(const Pokemon& rhs) const {
    return !(*this < rhs);
}
//------------------------------------------------------------------------------
ostream& mtm::pokemongo::operator<<(ostream& output, const Pokemon& pokemon) {
    output << pokemon.species << "(" << pokemon.level << "/"
           << pokemon.cp << "/" << pokemon.hp << ")";
    SET_FOREACH (PokemonType,iterator,pokemon.types) {
        output << " ";
        switch (*iterator) {
            case NORMAL:   output << "NORMAL"; break;
            case ROCK:     output << "ROCK"; break;
            case BUG:      output << "BUG"; break;
            case FAIRY:    output << "FAIRY"; break;
            case GROUND:   output << "GROUND"; break;
            case GRASS:    output << "GRASS"; break;
            case WATER:    output << "WATER"; break;
            case ICE:      output << "ICE"; break;
            case GHOST:    output << "GHOST"; break;
            case POISON:   output << "POISON"; break;
            case ELECTRIC: output << "ELECTRIC"; break;
            case FIRE:     output << "FIRE"; break;
            case FLYING:   output << "FLYING"; break;
            case PSYCHIC:  output << "PSYCHIC"; break;
        }
    }
    return output;
}
//------------------------------------------------------------------------------
int Pokemon::Level() const {
    return level;
}
//------------------------------------------------------------------------------