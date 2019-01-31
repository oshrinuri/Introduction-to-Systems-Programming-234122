#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
#include "exceptions.h"
#include "pokemon.h"

namespace mtm {
    namespace pokemongo {
        class Item {

        protected:
            int level;

        public:
            Item(const int& level): level(level) {
                if (level < 1) throw ItemInvalidArgsException();
            }
            Item(const Item& item) = default;
            virtual ~Item() {};
            virtual void useItem(Pokemon& pokemon) = 0;
            const int& Level() const { return level; }
            const int& Level() { return level; }
        };

        class Candy: public Item {
        public:
            Candy(const int& level): Item(level) {}
            Candy(const Candy& candy) = default;
            void useItem(Pokemon& pokemon) override {
                pokemon.Train(1+pokemon.Level()/10.0);
            }
        };

        class Potion: public Item {
        public:
            Potion(const int& level): Item(level) {}
            Potion(const Potion& potion) = default;
            void useItem(Pokemon& pokemon) override {
                pokemon.Heal();
            }
        };
    }
}
#endif  // ITEM_H
