#ifndef GYM_H
#define GYM_H

#include <iostream>
#include "location.h"
#include "trainer.h"

namespace mtm {
    namespace pokemongo {
        class Gym : public Location {

        private:
            const std::string name;
            Team gym_team_color;
            Trainer* gym_leader;
            bool is_taken;

        public:
            Gym() {}
            Gym(const std::string& name): name(name), gym_leader(NULL), is_taken(false) {}
            ~Gym() {}
            Gym &operator=(const Gym &gym);
            bool SameTeamTrainerExist(const Trainer& trainer);
            void SetNewGymLeader(Trainer* trainer);
            void ReplaceGymLeader(const Trainer& trainer);
            void Arrive(Trainer& trainer) override;
            void Leave(Trainer& trainer) override;
            };
    }  // namespace pokemongo
}  // namespace mtm

#endif  // GYM_H