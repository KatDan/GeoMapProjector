#include <iostream>
#include "../include/projections.hpp"
#include "controller.cpp"

using namespace std;

int main() {
    /*Database db;
    db_loader loader;
    loader.add_initial_data(db);*/

    /*GnomonicProjection gproj;
    gproj.add_point("p0",5,10.12);
    gproj.add_point("p1",-20,62);

    gproj.print_points();

    cout << gproj.calculate_distance("p0","p1")<<endl;


    /*db.print_cities();
    db.print_continents();
    db.print_countries();
    db.print_lakes();
    db.print_mountains();
    cout << gproj.calculate_distance("Slovakia", "Spain")<<endl;
    auto result = gproj.calculate_rectangular_area("Slovakia");
    cout.precision(15);
    cout << "result: "<<result;*/
    cout.precision(15);
    Controller c;
    c.process_input(cin);




    return 0;
}
