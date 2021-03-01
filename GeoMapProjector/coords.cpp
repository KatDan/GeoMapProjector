//
// Created by Ja on 28.02.2021.
//

class Coords{
};

class RealCoords : public Coords{
public:
    RealCoords(double lat, double lon):latitude{lat},longitude{lon}{}

    double latitude;
    double longitude;
};

class PolarCoords: public Coords{
public:
    double epsilon;
    double rho;

    PolarCoords(){}
    PolarCoords(double e, double r):epsilon{e},rho{r}{}
};

class CartesianCoords : public Coords{
public:
    double x;
    double y;

    CartesianCoords(){}
    CartesianCoords(double x, double y):x{x},y{y}{}
};

