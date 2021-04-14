#include "aircraft_manager.hpp"

#include <algorithm>
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
                                   [](auto& aircraft) { return !aircraft->update(); }),
                    aircrafts.end());

    // On peut potentiellement supprimer des aircraft juste avant, autant ne pas les prendre en compte dans le
    // tri
    reorder();
    return true;
}
void AircraftManager::stats(const std::string& flight_number)
{

    auto nb_flight = std::count_if(aircrafts.begin(), aircrafts.end(),
                                   [flight_number](const auto& aircraft)
                                   { return aircraft->get_flight_num().substr(0, 2) == flight_number; });
    std::cout << "Number of flights of " << flight_number << ": " << nb_flight << std::endl;
}

void AircraftManager::display_aircrafts()
{
    auto has_terminal = [](const std::unique_ptr<Aircraft>& a)
    { return a->has_terminal() ? "Reserved" : "NotReserved"; };
    std::for_each(aircrafts.begin(), aircrafts.end(),
                  [has_terminal](const auto& a)
                  {
                      std::cout << a->get_flight_num() << " - " << has_terminal(a)
                                << " / Fuel: " << a->get_fuel() << std::endl;
                  });
}