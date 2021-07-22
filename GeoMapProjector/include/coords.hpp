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

enum class lat_or_long{
    BOTH,
    LATITUDE,
    LONGITUDE
};

class Coords{
public:
    pair<double,double> coords_pair;
    enum lat_or_long singleton_type = lat_or_long::BOTH;

    Coords(){}
    Coords(lat_or_long type);
    Coords(double c1, double c2);

    virtual double get_singleton_value() const = 0;

    virtual string get_coords() const;

    virtual ~Coords(){}
};

class RealCoords : public Coords{
public:
    double latitude;
    double longitude;

    RealCoords(double lat, double lon);
    RealCoords(double value, lat_or_long type);

    double get_singleton_value() const override;

    ~RealCoords(){}

    string get_coords() const override;

    static pair<double,double> normalize_coords(double a, double b);
};

class PolarCoords: public Coords{
public:
    double epsilon;
    double rho;

    PolarCoords():Coords(){}
    PolarCoords(double r, double e);
    PolarCoords(double value, lat_or_long type);

    double get_singleton_value() const override;

    ~PolarCoords(){};

    string get_coords() const override;

    static pair<double,double> normalize_coords(double a, double b);
};

class CartesianCoords : public Coords{
public:
    double x;
    double y;

    CartesianCoords(){}
    CartesianCoords(double x, double y);
    CartesianCoords(double value, lat_or_long type);

    double get_singleton_value() const override;

    ~CartesianCoords(){};

    string get_coords() const override;
};


template<typename T>
double get_distance(const T &p0, const T &p1);

template<>
double get_distance<RealCoords>(const RealCoords &p0, const RealCoords &p1);

template<>
double get_distance<PolarCoords>(const PolarCoords &p0, const PolarCoords &p1);

template<>
double get_distance<CartesianCoords>(const CartesianCoords &p0, const CartesianCoords &p1);

template<typename T>
double get_rectangular_area(const T &s, const T &n, const T &e, const T &w);

template<>
double get_rectangular_area<PolarCoords>(const PolarCoords &s, const PolarCoords &n, const PolarCoords &e, const PolarCoords &w);

template<>
double get_rectangular_area<CartesianCoords>(const CartesianCoords &s, const CartesianCoords &n, const CartesianCoords &e, const CartesianCoords &w);

template<>
double get_rectangular_area<RealCoords>(const RealCoords &s, const RealCoords &n, const RealCoords &e, const RealCoords &w);


#endif //GEOMAPPROJECTOR_COORDS_HPP
