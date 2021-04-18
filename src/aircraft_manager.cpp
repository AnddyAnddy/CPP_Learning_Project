#include "aircraft_manager.hpp"

#include "aircraft_crash.hpp"

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
                                   [this](std::unique_ptr<Aircraft>& aircraft)
                                   {
                                       try
                                       {
                                           return !aircraft->update();
                                       } catch (const AircraftCrash& e)
                                       {
                                           nb_crash_aircrafts++;
                                           aircraft->control.remove_aircraft_from_terminals(aircraft.get());
                                           std::cerr << e.what() << std::endl;
                                           return true;
                                       }
                                   }),
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

unsigned int AircraftManager::get_required_fuel() const
{
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0U,
                           [](unsigned int sum, const std::unique_ptr<Aircraft>& aircraft)
                           {
                               if (aircraft->is_low_on_fuel() && aircraft->at_terminal())
                               {
                                   return sum + FULL_KEROSENE - aircraft->get_fuel();
                               }
                               return sum;
                           });
}

std::ostream& operator<<(std::ostream& stream, AircraftManager& manager)
{
    auto has_terminal = [](const std::unique_ptr<Aircraft>& a)
    { return a->has_terminal() ? "Reserved" : "NotReserved"; };
    std::for_each(manager.aircrafts.begin(), manager.aircrafts.end(),
                  [has_terminal](const auto& a)
                  {
                      std::cout << a->get_flight_num() << " - " << has_terminal(a)
                                << " / Fuel: " << a->get_fuel() << std::endl;
                  });
    return stream;
}