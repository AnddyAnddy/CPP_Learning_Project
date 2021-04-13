#include "aircraft_manager.hpp"

#include <utility>

void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

bool AircraftManager::update()
{

    // On doit déréférencer 2x pour obtenir une référence sur l'Aircraft : une fois pour déréférencer
    // l'itérateur, et une deuxième fois pour déréférencer le unique_ptr.
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                   [](auto aircraft_it)
                                   {
                                       auto& aircraft = **aircraft_it;
                                       return !aircraft.update();
                                   }),
                    aircrafts.end());

    return true;
}
