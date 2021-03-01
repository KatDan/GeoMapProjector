#include <iostream>
#include "projections.cpp"

using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;

    GnomonicProjection gproj;
    gproj.add_point("p0",5,10.12);
    gproj.add_point("p1",-20,62);

    gproj.print_points();



    return 0;
}
