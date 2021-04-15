#include "tower_sim.hpp"

// void test_generic_points()
//{
//     Point<int, 3> p1(1, 2, 3);
//     Point<int, 3> p2(1, 2, 3);
//     //    Point2D p4(1, 2, 3); // marche plus pcq point2d est un alias de Point<T, 2>
//     //    Point3D p5(1, 2);
//     [[maybe_unused]] auto p3 = p1 + p2;
//     p1 += p2;
//     p1 *= 3; // ou 3.f, ou 3.0 en fonction du type de Point
//     std::cout << p1.x() << std::endl;
// }

int main(int argc, char** argv)
{
    //    test_generic_points();
    TowerSimulation simulation { argc, argv };
    simulation.launch();

    return 0;
}