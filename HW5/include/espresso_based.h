#ifndef ESPRESSO_BASED_H
#define ESPRESSO_BASED_H

#include "ingredient.h"

#include <vector>

class EspressoBased {
public:
    std::string get_name() const {
        return this->name;
    }

    virtual double price() = 0;

    virtual void brew() = 0;

    std::vector<Ingredient *> &get_ingredients() {
        return this->ingredients;
    }

    virtual ~EspressoBased();

protected:
    EspressoBased() = default;

    EspressoBased(const std::string &name) : name(name) {};

    EspressoBased(const EspressoBased &esp);

    EspressoBased &operator=(const EspressoBased &esp) = delete;

    std::vector<Ingredient *> ingredients;
    std::string name;

};

#endif // ESPRESSO_BASED_H