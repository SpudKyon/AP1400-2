#ifndef SUB_INGREDIENTS_H
#define SUB_INGREDIENTS_H

#include "ingredient.h"

#define STRING_TEMP(VAR) #VAR
#define STRING(VAR) STRING_TEMP(VAR)

#define DEFCLASS(NAME, PRICE)                                   \
    class NAME: public Ingredient                               \
    {                                                           \
    public:                                                     \
        NAME(size_t units) : Ingredient{PRICE, units}           \
        {                                                       \
            this->name = STRING(NAME);                          \
        }                                                       \
        virtual double price() override                         \
        {                                                       \
            return units * price_unit;                          \
        }                                                       \
        virtual Ingredient* clone() override                    \
        {                                                       \
            return new NAME(*this);                             \
        }                                                       \
    }

DEFCLASS(Cinnamon, 5);

DEFCLASS(Chocolate, 5);

DEFCLASS(Sugar, 1);

DEFCLASS(Cookie, 10);

DEFCLASS(Espresso, 15);

DEFCLASS(Milk, 10);

DEFCLASS(MilkFoam, 5);

DEFCLASS(Water, 1);

#endif // SUB_INGREDIENTS_H