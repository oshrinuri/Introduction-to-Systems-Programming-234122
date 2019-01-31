// -------------------------------------------------------------------------- //
//                             INCLUDES & DEFINES                             //
// -------------------------------------------------------------------------- //
#include <cmath>
#include "trainer.h"

using namespace mtm::pokemongo;
using namespace std;

static const int BASE_SCORE = 0;
static const int BASE_LEVEL = 1;
static const int WIN_BONUS = 2;
static const int LOSS_PENALTY = -1;
typedef map<int,Pokemon>::const_iterator Map_Iterator;

#define MAP_FOREACH(itr_type,itr,map_name) \
        for (map<itr_type,Pokemon>::iterator itr = map_name.begin(); \
             itr!=map_name.end() ; ++itr)

#define MAP_FOREACH_CONST(itr_type,itr,map_name) \
        for (map<itr_type,Pokemon>::const_iterator itr = map_name.begin(); \
             itr!=map_name.end() ; ++itr)
// -------------------------------------------------------------------------- //
//                             CONSTRUCTORS                                   //
// -------------------------------------------------------------------------- //
Trainer::Trainer(const std::string& name, const Team& team) {
    if (name.empty()) {
        throw TrainerInvalidArgsException();
    }
    this->name = name;
    this->team = team;
    this->level = BASE_LEVEL;
    this->score = BASE_SCORE;
    this->has_won_last_battle = false;
    this->is_a_gym_leader = false;
}
// -------------------------------------------------------------------------- //
//                                FUNCTIONS                                   //
// -------------------------------------------------------------------------- //
Pokemon& Trainer::GetStrongestPokemon() {
    return const_cast<Pokemon&>(static_cast
                                <const Trainer*>(this)->GetStrongestPokemon());
}
//------------------------------------------------------------------------------
const Pokemon& Trainer::GetStrongestPokemon() const {
    if (pokemons.empty()) {
        throw TrainerNoPokemonsFoundException();
    }
    Map_Iterator strongest_itr = pokemons.begin();
    MAP_FOREACH_CONST(int,iterator,this->pokemons) {
        if (iterator->second > strongest_itr->second) {
            strongest_itr = iterator;
        }
    }
    return strongest_itr->second;
}
//------------------------------------------------------------------------------
void Trainer::KillStrongestPokemon() {
    Pokemon& strongest_pokemon = GetStrongestPokemon();
    int pokemon_id = -1;
    MAP_FOREACH(int,iterator,this->pokemons) {
        if (strongest_pokemon == iterator->second) {
            pokemon_id = iterator->first;
            break;
        }
    }
    this->pokemons.erase(pokemon_id);
}
//------------------------------------------------------------------------------
bool Trainer::IsAlly(const Trainer& trainer) const {
    return this->team == trainer.team;
}
//------------------------------------------------------------------------------
Team Trainer::GetTeam() const {
    return this->team;
}
//------------------------------------------------------------------------------
bool Trainer::TryToCatch(Pokemon& pokemon) {
    if (pokemon.Level() > this->level) {
        return false;
    }
    this->pokemons.insert({this->pokemons.size()+1,pokemon});
    return true;
}
// -------------------------------------------------------------------------- //
//                                OPERATORS                                   //
// -------------------------------------------------------------------------- //
bool Trainer::operator==(const Trainer& rhs) const {
    if (this->pokemons.empty() && rhs.pokemons.empty()) {
        return true;
    } else if (this->pokemons.empty() && !rhs.pokemons.empty()) {
        return false;
    } else if (!(this->pokemons.empty()) && rhs.pokemons.empty()) {
        return false;
    }
    const Pokemon& trainer1_strongest = this->GetStrongestPokemon();
    const Pokemon& trainer2_strongest = rhs.GetStrongestPokemon();
    return trainer1_strongest == trainer2_strongest;
}
//------------------------------------------------------------------------------
bool Trainer::operator!=(const Trainer& rhs) const {
    return !(*this == rhs);
}
//------------------------------------------------------------------------------
bool Trainer::operator>(const Trainer& rhs) const {
    if (!this->pokemons.empty() && rhs.pokemons.empty()) {
        return true;
    } else if (this->pokemons.empty() && !rhs.pokemons.empty()) {
        return false;
    } else if (this->pokemons.empty() && rhs.pokemons.empty()) {
        return false;
    } else {
        const Pokemon &trainer1_strongest = this->GetStrongestPokemon();
        const Pokemon &trainer2_strongest = rhs.GetStrongestPokemon();
        if (trainer1_strongest > trainer2_strongest) {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool Trainer::operator<(const Trainer& rhs) const {
    return rhs > *this;
}
//------------------------------------------------------------------------------
bool Trainer::operator<=(const Trainer& rhs) const {
    return !(*this > rhs);
}
//------------------------------------------------------------------------------
bool Trainer::operator>=(const Trainer& rhs) const {
    return !(*this < rhs);
}
//------------------------------------------------------------------------------
ostream& mtm::pokemongo::operator<<(std::ostream& output, const Trainer& trainer) {
    if (trainer.pokemons.empty()) {
        throw TrainerNoPokemonsFoundException();
    }
    string team;
    switch (trainer.team) {
        case YELLOW: team = "YELLOW"; break;
        case RED: team = "RED"; break;
        case BLUE: team = "BLUE"; break;
    }
    output << trainer.name << " ("
           << trainer.level << ") " << team << endl;
    MAP_FOREACH_CONST (int,pokemon_iterator,trainer.pokemons) {
        output << pokemon_iterator->second << endl;
    }
    return output;
}
//------------------------------------------------------------------------------
void Trainer::BattleReward(Trainer& other) {
    this->has_won_last_battle = true;
    other.has_won_last_battle = false;
    this->level = this->level + (int)ceil(other.level/2.0);
    this->score += WIN_BONUS;
    other.score += LOSS_PENALTY;
}
//------------------------------------------------------------------------------
int Trainer::GetPokemonMapIndex(const Pokemon& pokemon) {
    MAP_FOREACH_CONST(int,pokemon_iterator,this->pokemons) {
        if (&pokemon == &pokemon_iterator->second) {
            return pokemon_iterator->first;
        }
    }
    return -1;
}
//------------------------------------------------------------------------------
void Trainer::RemovePokemon(Pokemon& pokemon) {
    this->pokemons.erase(GetPokemonMapIndex(pokemon));
}
//------------------------------------------------------------------------------
void Trainer::DetermineByColor(Trainer& trainer1, Trainer& trainer2) {
    Team trainer1_team = trainer1.GetTeam();
    Team trainer2_team = trainer2.GetTeam();

    if (trainer1_team == BLUE && trainer2_team == RED) {
        trainer1.BattleReward(trainer2);
    } else if (trainer1_team == RED && trainer2_team == YELLOW) {
        trainer1.BattleReward(trainer2);
    } else if (trainer1_team == YELLOW && trainer2_team == BLUE) {
        trainer1.BattleReward(trainer2);
    }
}
//------------------------------------------------------------------------------
void Trainer::Battle(Trainer& rival) {
    if (this->pokemons.empty()) {
        rival.BattleReward(*this);
        return;
    }
    if (rival.pokemons.empty()) {
        this->BattleReward(rival);
        return;
    }
    Pokemon& strongest_self = GetStrongestPokemon();
    Pokemon& strongest_rival = rival.GetStrongestPokemon();
    if (this->items.size() != 0) {
        Item &oldest_item_self = *(this->items.front());
        oldest_item_self.useItem(strongest_self);
    }
    if (rival.items.size() != 0) {
        Item& oldest_item_rival = *(rival.items.front());
        oldest_item_rival.useItem(strongest_rival);
    }
    bool is_dead_self = strongest_self.Hit(strongest_rival);
    bool is_dead_rival = strongest_rival.Hit(strongest_self);
    if (is_dead_self) {
        this->RemovePokemon(strongest_self);
    }
    if (is_dead_rival) {
        rival.RemovePokemon(strongest_rival);
    }
    if (strongest_self > strongest_rival) {
        this->BattleReward(rival);
        return;
    } else if (strongest_self < strongest_rival) {
        rival.BattleReward(*this);
        return;
    } else {
        DetermineByColor(*this,rival);
        DetermineByColor(rival,*this);
    }
}
//------------------------------------------------------------------------------
const string& Trainer::Location() const {
    return this->location;
}
//------------------------------------------------------------------------------
int Trainer::Score() const {
    return this->score;
}
//------------------------------------------------------------------------------
int Trainer::Level() const {
    return this->level;
}
//------------------------------------------------------------------------------
list<Item*> Trainer::GetItems() const {
    return this->items;
}
//------------------------------------------------------------------------------
bool Trainer::IsGymLeader() const {
    return this->is_a_gym_leader;
}
//------------------------------------------------------------------------------
bool Trainer::HasWonLastBattle() const {
    return this->has_won_last_battle;
}
//------------------------------------------------------------------------------
void Trainer::SetAsGymLeader() {
    this->is_a_gym_leader = true;
}
//------------------------------------------------------------------------------
void Trainer::UnsetAsGymLeader() {
    this->is_a_gym_leader = false;
}
//------------------------------------------------------------------------------
void Trainer::SetLocation(const std::string& location) {
    this->location = location;
}
//------------------------------------------------------------------------------