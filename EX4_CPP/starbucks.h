#ifndef STARBUCKS_H
#define STARBUCKS_H

#include <iostream>
#include <list>
#include "location.h"
#include "trainer.h"
#include "item.h"

namespace mtm {
    namespace pokemongo {
        class Starbucks : public Location {

        private:
            std::string name;
            std::list<Pokemon> pokemons;

        public:
            Starbucks(const std::string& name, std::list<Pokemon> pokemons):
                    name(name), pokemons(pokemons) {};
            ~Starbucks() = default;
            void Arrive(Trainer& trainer) override;
            void Leave(Trainer& trainer) override;
        };
    }  // namespace pokemongo
}  // namespace mtm

#endif  // STARBUCKS_H