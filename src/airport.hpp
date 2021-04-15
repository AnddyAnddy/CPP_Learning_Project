#pragma once

#include "GL/displayable.hpp"
#include "GL/dynamic_object.hpp"
#include "GL/texture.hpp"
#include "aircraft_manager.hpp"
#include "airport_type.hpp"
#include "geometry.hpp"
#include "img/image.hpp"
#include "runway.hpp"
#include "terminal.hpp"
#include "tower.hpp"

#include <vector>

class Airport : public GL::Displayable, public GL::DynamicObject
{
private:
    const AirportType& type;
    const Point3D pos;
    const GL::Texture2D texture;
    unsigned int fuel_stock       = 0;
    unsigned int ordered_fuel     = 0;
    unsigned int next_refill_time = 0;
    std::vector<Terminal> terminals;
    Tower tower;
    const AircraftManager& manager;
    static constexpr unsigned int DELIVERED_FUEL = 5000;
    static constexpr unsigned int REFILL_TIME    = 100;

    // reserve a terminal
    // if a terminal is free, return
    // 1. a sequence of waypoints reaching the terminal from the runway-end and
    // 2. the number of the terminal (used for liberating the terminal later)
    // otherwise, return an empty waypoint-vector and any number
    std::pair<WaypointQueue, size_t> reserve_terminal(Aircraft& aircraft)
    {
        const auto it =
            std::find_if(terminals.begin(), terminals.end(), [](const Terminal& t) { return !t.in_use(); });

        if (it != terminals.end())
        {
            it->assign_craft(aircraft);
            const auto term_idx = std::distance(terminals.begin(), it);
            return { type.air_to_terminal(pos, 0, term_idx), term_idx };
        }
        else
        {
            return { {}, 0u };
        }
    }

    WaypointQueue start_path(const size_t terminal_number)
    {
        return type.terminal_to_air(pos, 0, terminal_number);
    }

    Terminal& get_terminal(const size_t terminal_num) { return terminals.at(terminal_num); }

public:
    Airport(const AirportType& type_, const Point3D& pos_, const img::Image* image,
            const AircraftManager& manager_, const float z_ = 1.0f) :
        GL::Displayable { z_ },
        type { type_ },
        pos { pos_ },
        texture { image },
        terminals { type.create_terminals() },
        tower { *this },
        manager { manager_ }
    {}

    Tower& get_tower() { return tower; }

    unsigned int get_fuel_stock() const { return fuel_stock; }

    void display() const override { texture.draw(project_2D(pos), { 2.0f, 2.0f }); }

    bool update() override
    {
        for (auto& t : terminals)
        {
            if (next_refill_time == 0)
            {
                fuel_stock += ordered_fuel;
                ordered_fuel     = std::min(manager.get_required_fuel(), DELIVERED_FUEL);
                next_refill_time = REFILL_TIME;
                // * La quantité d'essence reçue, la quantité d'essence en stock et la nouvelle quantité
                // d'essence commandée sont affichées dans la console.
                // TODO: pas trop compris quoi afficher
                if (ordered_fuel != 0)
                {
                    std::cout << "In stock: " << fuel_stock << " fuel" << std::endl;
                    std::cout << "Ordered " << ordered_fuel << " fuel" << std::endl;
                }
            }
            else
            {
                next_refill_time--;
            }
            t.refill_aircraft_if_needed(fuel_stock);
            t.update();
        }

        return true;
    }

    friend class Tower;
};
