#pragma once
//
// Created by anddy on 18/04/2021.
//

#include "aircraft.hpp"
#include "aircraft_types.hpp"
#include "airport.hpp"

#include <unordered_set>

class AircraftFactory
{
private:
    static constexpr unsigned int NB_TYPES_AIRCRAFT = 3;
    std::unordered_set<std::string> registered_aircraft_names;
    const AircraftType* aircraft_types[NB_TYPES_AIRCRAFT] {};

    bool is_aircraft_registered(const std::string& aircraft_name) const
    {
        return registered_aircraft_names.find(aircraft_name) != registered_aircraft_names.end();
    }

    std::string new_unique_random_aircraft_name()
    {
        std::string flight_number;
        do
        {
            flight_number =
                prefixes[rand() % NB_POSSIBLE_AIRCRAFT_PREFIX] + std::to_string(1000 + (rand() % 9000));
        }
        while (is_aircraft_registered(flight_number));

        registered_aircraft_names.emplace(flight_number);
        return flight_number;
    }

public:
    static constexpr unsigned int NB_POSSIBLE_AIRCRAFT_PREFIX = 8;
    const std::vector<std::string> prefixes = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };

    [[nodiscard]] std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type,
                                                            std::shared_ptr<Airport> airport)
    {
        assert(airport);
        const auto flight_name = new_unique_random_aircraft_name();
        const auto angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
        const auto start       = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
        const auto direction   = (-start).normalize();

        return std::make_unique<Aircraft>(type, flight_name, start, direction, airport->get_tower());
    }

    std::unique_ptr<Aircraft> create_random_aircraft(std::shared_ptr<Airport> airport)
    {
        return create_aircraft(*(aircraft_types[rand() % NB_TYPES_AIRCRAFT]), airport);
    }
    // note: textures have to be initialized after initializing glut, so we'll need
    // our own init here
    void init_aircraft_types()
    {
        // TASK_0: C-1.
        aircraft_types[0] = new AircraftType { .02f, .05f, .02f, 1, MediaPath { "l1011_48px.png" } };
        aircraft_types[1] = new AircraftType { .02f, .05f, .02f, 2, MediaPath { "b707_jat.png" } };
        aircraft_types[2] = new AircraftType { .02f, .08f, .03f, 3, MediaPath { "concorde_af.png" } };
    }

    unsigned int get_nb_possible_aircraft_prefix() const { return NB_POSSIBLE_AIRCRAFT_PREFIX; }
};
