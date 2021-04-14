#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
    void reorder()
    {
        std::sort(aircrafts.begin(), aircrafts.end(),
                  [](const auto& a, const auto& b)
                  {
                      return std::make_tuple(!a->has_terminal(), a->get_fuel()) <
                             std::make_tuple(!b->has_terminal(), b->get_fuel());
                  });
    }

public:
    void add(std::unique_ptr<Aircraft> aircraft);

    bool update() override;
    void stats(const std::string& flight_number);
    void display_aircrafts();
};
