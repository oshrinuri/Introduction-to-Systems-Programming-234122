#ifndef POKESTOP_H
#define POKESTOP_H

#include <iostream>
#include <list>
#include "location.h"
#include "trainer.h"
#include "item.h"
#include "exceptions.h"

namespace mtm {
    namespace pokemongo {
        class Pokestop : public Location {
        private:
            std::string name;
            std::list<Item*> items;

        public:
            Pokestop(const std::string& name, std::list<Item*> items):
                     name(name), items(items) {};
            ~Pokestop() {
                for (std::list<Item*>::iterator it = items.begin();
                        it != items.end(); ++it) {
                    delete *it;
                }
            }
            void Arrive(Trainer& trainer) override;
            void Leave(Trainer& trainer) override;
        };
    }  // namespace pokemongo
}  // namespace mtm

#endif  // POKESTOP_H