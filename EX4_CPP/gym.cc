// -------------------------------------------------------------------------- //
//                             INCLUDES & DEFINES                             //
// -------------------------------------------------------------------------- //
#include "gym.h"

using namespace mtm::pokemongo;
using namespace std;

typedef vector<Trainer*>::const_iterator Vec_Iterator;

#define VEC_FOREACH(vec_type,itr,vec_name) \
        for (vector<vec_type>::iterator itr = vec_name.begin(); \
             itr!=vec_name.end() ; ++itr)

// -------------------------------------------------------------------------- //
//                                FUNCTIONS                                   //
// -------------------------------------------------------------------------- //
void Gym::SetNewGymLeader(Trainer* trainer) {
    this->is_taken = true;
    trainer->SetAsGymLeader();
    this->gym_team_color = trainer->GetTeam();
    this->gym_leader = trainer;
}
//------------------------------------------------------------------------------
bool Gym::SameTeamTrainerExist(const Trainer& trainer) {
    int same_team_trainers = 0;
    VEC_FOREACH(Trainer*,vec_itr,trainers_) {
         Trainer& current_trainer = **vec_itr;
         if (current_trainer.GetTeam() == trainer.GetTeam() && current_trainer!= trainer){
             same_team_trainers++;
         }
    }
    return same_team_trainers != 0;
}
//------------------------------------------------------------------------------
void Gym::ReplaceGymLeader(const Trainer &trainer) {
    bool same_team_trainers = SameTeamTrainerExist(trainer);
    Trainer *strongest_trainer = trainers_.front();
    VEC_FOREACH(Trainer*, vec_itr, trainers_) {
        Trainer &current_trainer = **vec_itr;
        if (same_team_trainers) {
            if ((current_trainer > *strongest_trainer) &&
                (current_trainer.GetTeam() == trainer.GetTeam())) {
                *strongest_trainer = current_trainer;
            }
        } else {
            *strongest_trainer = current_trainer;
        }
    }
    SetNewGymLeader(strongest_trainer);
}
//------------------------------------------------------------------------------
void Gym::Arrive(Trainer& trainer) {
    if (trainers_.empty()) {
        SetNewGymLeader(&trainer);
    } else {
        if (trainer.GetTeam() != gym_leader->GetTeam()) {
            gym_leader->Battle(trainer);
            if (trainer.HasWonLastBattle()) {
                gym_leader->UnsetAsGymLeader();
                SetNewGymLeader(&trainer);
            }
        }
    }
    Location::Arrive(trainer);
}
//------------------------------------------------------------------------------
void Gym::Leave(Trainer& trainer) {
    if (is_taken == true) {
        if (gym_leader == &trainer && trainers_.size() == 1) {
            is_taken = false;
        }
        if (gym_leader == &trainer && trainers_.size() > 1) {
            ReplaceGymLeader(trainer);
        }
    }
    Location::Leave(trainer);
}
//------------------------------------------------------------------------------
