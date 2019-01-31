// -------------------------------------------------------------------------- //
//                             INCLUDES & DEFINES                             //
// -------------------------------------------------------------------------- //
#include <fstream>
#include <list>
#include "pokemon_go.h"

using namespace mtm::pokemongo;
using namespace std;

static const int LEADER_BONUS = 10;
typedef map<string,Trainer>::iterator Trainers_Itr;
typedef mtm::KGraph<std::string,Location*,4>::const_iterator World_Itr;
// -------------------------------------------------------------------------- //
//                             CONSTRUCTORS                                   //
// -------------------------------------------------------------------------- //
PokemonGo::PokemonGo(const World* world): world_ptr(world) {}
//------------------------------------------------------------------------------
void PokemonGo::AddTrainer(const std::string& name, const Team& team,
                           const std::string& location) {
    if (name.empty()) {
        throw PokemonGoInvalidArgsException();
    }
    Trainers_Itr it = trainers.find(name);
    if (it != trainers.end()) {
        throw PokemonGoTrainerNameAlreadyUsedExcpetion();
    }
    if (!world_ptr->Contains(location)) {
        throw PokemonGoLocationNotFoundException();
    }
    trainers.insert({name,Trainer(name,team)});
    Trainer& trainer = trainers.at(name);
    (*world_ptr)[location]->Arrive(trainer);
    trainer.SetLocation(location);
}
//------------------------------------------------------------------------------
void PokemonGo::MoveTrainer(const std::string& trainer_name, const Direction& dir) {
    Trainers_Itr it = trainers.find(trainer_name);
    if (it == trainers.end()) {
        throw PokemonGoTrainerNotFoundExcpetion();
    }
    Trainer& trainer = it->second;
    World_Itr world_itr = world_ptr->BeginAt(trainer.Location());
    world_itr.Move(dir);
    try {
        *world_itr;
    }
    catch (KGraphIteratorReachedEnd) {
        throw PokemonGoReachedDeadEndException();
    }
    (*world_ptr)[trainer.Location()]->Leave(trainer);
    trainer.SetLocation(*world_itr);
    (*world_ptr)[trainer.Location()]->Arrive(trainer);
}
//------------------------------------------------------------------------------
string PokemonGo::WhereIs(const std::string& trainer_name) {
    Trainers_Itr it = trainers.find(trainer_name);
    if (it == trainers.end()) {
        throw PokemonGoTrainerNotFoundExcpetion();
    }
    return it->second.Location();
}
//------------------------------------------------------------------------------
const std::vector<Trainer*>& PokemonGo::GetTrainersIn(const std::string& location) {
    if (!world_ptr->Contains(location)) {
        throw PokemonGoLocationNotFoundException();
    }
    return (*world_ptr)[location]->GetTrainers();
}
//------------------------------------------------------------------------------
int PokemonGo::numT(Team team) {
    int counter = 0;
    for (Trainers_Itr it = trainers.begin(); it != trainers.end(); ++it) {
        if (it->second.GetTeam() == team) {
            counter++;
        }
    }
    return counter;
}
//------------------------------------------------------------------------------
void PokemonGo::PrintTrainersByTeam(Team team) {
    for (Trainers_Itr it = trainers.begin(); it != trainers.end(); ++it) {
      //  if (it->second.GetTeam() == team) {
            cout << it->first << " " << &it->second << " " << it->second.GetName() << endl;
        //}
    }
}
//------------------------------------------------------------------------------
const int PokemonGo::GetScore(const Team& team) {
    int total_score, total_trainer_lvls, num_of_leaders;
    total_score = 0 ; total_trainer_lvls = 0 ; num_of_leaders = 0;
    for (Trainers_Itr it = trainers.begin(); it != trainers.end(); ++it) {
        Trainer& curr_trainer = it->second;
        if (curr_trainer.GetTeam() == team) {
            if (curr_trainer.IsGymLeader()) num_of_leaders++;
            total_trainer_lvls += curr_trainer.Level();
            total_score += curr_trainer.Score();
        }
    }
    PrintTrainersByTeam(team);
    cout << "(Num of trainers:" << numT(team) << ") | Total trainers score: " << total_score << " | Sum lvls:" << total_trainer_lvls << " | Num leaders: " << num_of_leaders << endl;
    return total_score + total_trainer_lvls + num_of_leaders * LEADER_BONUS;
}
//------------------------------------------------------------------------------