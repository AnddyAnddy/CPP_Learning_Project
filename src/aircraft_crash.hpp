//
// Created by anddy on 18/04/2021.
//
#pragma once

class AircraftCrash : public std::runtime_error
{
private:
    static std::string crash_message(const std::string& flight_name, Point3D& position, Point3D& speed,
                                     const std::string& reason)
    {
        using namespace std::string_literals;
        std::stringstream ss;
        ss << "CRASH: Aircraft n° " << flight_name << std::endl;
        ss << "\tPosition: " << position;
        ss << "\tSpeed: " << speed;
        ss << "\tReason: " << reason;

        return ss.str();
    }

public:
    AircraftCrash(const std::string& flight_number, Point3D& position, Point3D& speed,
                  const std::string& reason) :
        std::runtime_error { crash_message(flight_number, position, speed, reason) } {};
};
