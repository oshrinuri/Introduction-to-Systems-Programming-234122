// -------------------------------------------------------------------------- //
//                             INCLUDES & DEFINES                             //
// -------------------------------------------------------------------------- //
#include <fstream>
#include "world.h"
#include "item.h"
#include <list>
#include "exceptions.h"

using namespace mtm::pokemongo;
using namespace std;

// -------------------------------------------------------------------------- //
//                             CONSTRUCTORS                                   //
// -------------------------------------------------------------------------- //
World::World(): KGraph<string,Location*,4>(nullptr) {}
// -------------------------------------------------------------------------- //
//                               AUX FUNCTIONS                                //
// -------------------------------------------------------------------------- //
namespace {
    bool isLocationAlreadyExist(const World &world,
                                const std::string &location_name) {
        return world.Contains(location_name);
    }
    bool IsValidNumber(const string &str) {
        std::string::const_iterator it = str.begin();
        while (it != str.end() && std::isdigit(*it)) {
            ++it;
        }
        return !str.empty() && it == str.end();
    }
}
// -------------------------------------------------------------------------- //
//                                FUNCTIONS                                   //
// -------------------------------------------------------------------------- //
void World::handleStarbucksLine(std::istream& input, string& current_word) {
    string location_name, pokemon_species;
    double pokemon_cp;
    int pokemon_level;
    input >> current_word;
    location_name = current_word;
    if (isLocationAlreadyExist(*this,location_name)) {
        throw WorldLocationNameAlreadyUsed();
    }
    std::list<Pokemon> pokemons;
    while (input >> current_word) {
        pokemon_species = current_word;
        if (pokemon_species.empty()) throw WorldInvalidInputLineException();
        input >> current_word;
        pokemon_cp = stod(current_word);
        if (pokemon_cp < 0.0) throw WorldInvalidInputLineException();
        input >> current_word;
        pokemon_level = stoi(current_word);
        if (pokemon_level < 1) throw WorldInvalidInputLineException();
        Pokemon pokemon(pokemon_species,pokemon_cp,pokemon_level);
        pokemons.push_back(pokemon);
    }
    Location* new_location = new Starbucks(location_name,pokemons);
    locations_to_free.push_back(location_name);
    this->Insert(location_name,new_location);
}
//------------------------------------------------------------------------------
void World::handlePokestopLine(std::istream& input, string& current_word) {
    string location_name, item_name;
    int item_level;
    input >> current_word;
    location_name = current_word;
    std::list<Item*> items;
    if (isLocationAlreadyExist(*this,location_name)) {
        throw WorldLocationNameAlreadyUsed();
    }
    while (input >> current_word) {
        item_name = current_word;
        input >> current_word;
        if (!IsValidNumber(current_word)) throw WorldInvalidInputLineException();
        item_level = stoi(current_word);
        if (item_name == "CANDY") {
            Candy candy(item_level);
            items.push_back(new Candy(item_level));
        } else if (item_name == "POTION") {
            items.push_back(new Potion(item_level));
        } else {
            throw WorldInvalidInputLineException();
        }
    }
    Location* new_location = new Pokestop(location_name,items);
    locations_to_free.push_back(location_name);
    this->Insert(location_name,new_location);
}
//------------------------------------------------------------------------------
void World::handleGymLine(std::istream& input, string& current_word) {
    string location_name;
    if (isLocationAlreadyExist(*this,location_name)) {
        throw WorldLocationNameAlreadyUsed();
    }
    input >> current_word;
    location_name = current_word;
    Location* new_location = new Gym(location_name);
    locations_to_free.push_back(location_name);
    this->Insert(location_name,new_location);
}
//------------------------------------------------------------------------------
istream& mtm::pokemongo::operator>>(std::istream& input, World& world) {
    std::string current_word;
    while (input >> current_word) {
            if (current_word == "GYM") {
                world.handleGymLine(input,current_word);
            } else if (current_word == "POKESTOP") {
                world.handlePokestopLine(input,current_word);
            } else if (current_word == "STARBUCKS") {
                world.handleStarbucksLine(input,current_word);
            } else {
                throw WorldInvalidInputLineException();
            }
        }
    return input;
}
//------------------------------------------------------------------------------