//
// Created by Ja on 04.03.2021.
//

#ifndef GEOMAPPROJECTOR_COORDS_HPP
#define GEOMAPPROJECTOR_COORDS_HPP

#include<iostream>
#include<utility>
#include<cmath>
#include "my_math.hpp"

using namespace std;

enum lat_or_long{
    BOTH,
    LATITUDE,
    LONGITUDE
};

class Coords{
public:
    pair<double,double> coords_pair;
    enum lat_or_long singleton_type = BOTH;

    Coords(){}
    Coords(enum lat_or_long type);
    Coords(double c1, double c2);

    virtual double get_singleton_value(){}

    string get_coords() const;

    virtual ~Coords(){}
};

class RealCoords : public Coords{
public:
    double latitude;
    double longitude;

    RealCoords(double lat, double lon);
    RealCoords(double value, enum lat_or_long type);

    double get_singleton_value();

    ~RealCoords(){}
};

class PolarCoords: public Coords{
public:
    double epsilon;
    double rho;

    PolarCoords():Coords(){}
    PolarCoords(double e, double r);
    PolarCoords(double value, enum lat_or_long type);

    double get_singleton_value();

    ~PolarCoords(){};
};

class CartesianCoords : public Coords{
public:
    double x;
    double y;

    CartesianCoords(){}
    CartesianCoords(double x, double y);
    CartesianCoords(double value, enum lat_or_long type);

    double get_singleton_value();

    ~CartesianCoords(){};
};


template<typename T>
double get_distance(T p0, T p1);

template<>
double get_distance<RealCoords>(RealCoords p0, RealCoords p1);

template<>
double get_distance<PolarCoords>(PolarCoords p0, PolarCoords p1);

template<>
double get_distance<CartesianCoords>(CartesianCoords p0, CartesianCoords p1);

template<typename T>
double get_rectangular_area(T b0, T b1, T b2, T b3);

template<>
double get_rectangular_area<PolarCoords>(PolarCoords s, PolarCoords n, PolarCoords e, PolarCoords w);

template<>
double get_rectangular_area<CartesianCoords>(CartesianCoords s, CartesianCoords n, CartesianCoords e, CartesianCoords w);

template<>
double get_rectangular_area<RealCoords>(RealCoords s, RealCoords n, RealCoords e, RealCoords w);


#endif //GEOMAPPROJECTOR_COORDS_HPP
