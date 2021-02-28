#include "coords.cpp"
#include "database.cpp"

#include<map>
#include<string>
#include<vector>

using namespace std;

class Projection{
public:
    map<string,Coords> points;
    database &db;

    Projection(database &db):db{db}{}

    static double calculate_distance(const string &p0, const string &points...){
        //TODO variadic function
        return 0.0;
    };

    static double calculate_area(const string &p0, const string &p1, const string &points...){
        //TODO variadic function 2
        return 0.0;
    }

    Coords compute_coords(const string &name){
    }

    Coords compute_coords(const string &name, RealCoords &coords){

    }

    //nefunguje ako by som chcela
    void add_point(const string &name, float c1, float c2){
        if(points.find(name) == points.end()){
            RealCoords real_coords(c1,c2);
            Coords coords = compute_coords(name,real_coords);
            points[name] = coords;
        }
    }

    //TODO pridat add_point(const string &name) pomocou db

};

class AzimuthalProjection : public Projection{

};





class CylindricalProjection : public Projection{

};

class ConicProjection : public Projection{

};



