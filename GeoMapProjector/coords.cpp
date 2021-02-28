//
// Created by Ja on 28.02.2021.
//

class Coords{
};

class RealCoords : public Coords{
public:
    RealCoords(float lat, float lon):latitude{lat},longitude{lon}{}

    float latitude;
    float longitude;
};

class PolarCoords: public Coords{
public:
    float epsilon;
    float rho;

    PolarCoords(){}
    PolarCoords(float e, float r):epsilon{e},rho{r}{}
};

class CartesianCoords : Coords{
public:
    float x;
    float y;

    CartesianCoords(){}
    CartesianCoords(float x, float y):x{x},y{y}{}
};

