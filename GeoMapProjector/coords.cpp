//
// Created by Ja on 28.02.2021.
//
#include<utility>

using namespace std;

class Coords{
public:
    pair<double,double> coords_pair;

    Coords(){}
    Coords(double c1, double c2):coords_pair(make_pair(c1,c2)){}
};

class RealCoords : public Coords{
public:
    RealCoords(double lat, double lon):Coords(lat,lon),latitude{lat},longitude{lon}{}

    double latitude;
    double longitude;
};

class PolarCoords: public Coords{
public:
    double epsilon;
    double rho;

    PolarCoords():Coords(){}
    PolarCoords(double e, double r):Coords(r,e),epsilon{e},rho{r}{}
};

class CartesianCoords : public Coords{
public:
    double x;
    double y;

    CartesianCoords(){}
    CartesianCoords(double x, double y):Coords(y,x),x{x},y{y}{}
};

