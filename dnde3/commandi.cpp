#include "main.h"

template<> commandi bsdata<commandi>::elements[] = {{1, 0, &gamei::addreputation},
{-1, 0, &gamei::addreputation},
{-4, &gamei::getreputation},
{4, &gamei::getreputation},
{10 * GP, &gamei::getmoney, &gamei::addmoney},
{20 * GP, &gamei::getmoney, &gamei::addmoney},
{50 * GP, &gamei::getmoney, &gamei::addmoney},
{-10 * GP, &gamei::getmoney, &gamei::addmoney},
{-20 * GP, &gamei::getmoney, &gamei::addmoney},
{-50 * GP, &gamei::getmoney, &gamei::addmoney},
};
assert_enum(commandi, LastCommand)