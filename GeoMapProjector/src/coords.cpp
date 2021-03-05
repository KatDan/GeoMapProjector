//
// Created by Ja on 28.02.2021.
//
/*#include<iostream>
#include<utility>
#include<cmath>
#include "my_math.hpp"*/
#include "../include/coords.hpp"

using namespace std;

Coords::Coords(enum lat_or_long type):singleton_type{type}{}
Coords::Coords(double c1, double c2):coords_pair(make_pair(c1,c2)){}

RealCoords::RealCoords(double lat, double lon):Coords(lat,lon),latitude{lat},longitude{lon}{}
RealCoords::RealCoords(double value, enum lat_or_long type):Coords(type) {
    if (type == LATITUDE) latitude = value;
    else if (type == LONGITUDE) longitude = value;
}

double RealCoords::get_singleton_value() {
    if(singleton_type == LATITUDE) return latitude;
    else if(singleton_type == LONGITUDE) return longitude;
    else return 0;
}


PolarCoords::PolarCoords(double e, double r):Coords(r,e),epsilon{e},rho{r}{}
PolarCoords::PolarCoords(double value, enum lat_or_long type):Coords(type){
    if(type == LATITUDE) rho = value;
    else if(type == LONGITUDE) epsilon = value;
}

double PolarCoords::get_singleton_value() {
    if(singleton_type == LATITUDE) return rho;
    else if(singleton_type == LONGITUDE) return epsilon;
    else return 0;
}


CartesianCoords::CartesianCoords(double x, double y):Coords(y,x),x{x},y{y}{}
CartesianCoords::CartesianCoords(double value, enum lat_or_long type):Coords(type){
    if(type == LATITUDE) y = value;
    else if(type == LONGITUDE) x = value;
}

double CartesianCoords::get_singleton_value() {
    if(singleton_type == LATITUDE) return y;
    else if(singleton_type == LONGITUDE) return x;
    else return 0;
}


template<typename T>
double get_distance(T p0, T p1);

template<>
double get_distance<RealCoords>(RealCoords p0, RealCoords p1){
    double sigma = deg_arccos(deg_sin(p0.latitude)*deg_sin(p1.latitude)+
            deg_cos(p0.latitude)*deg_cos(p1.latitude)*deg_cos(abs(p0.longitude-p1.longitude)));
    return sigma * EARTH_PERIMETER;
}

template<>
double get_distance<PolarCoords>(PolarCoords p0, PolarCoords p1){
    return sqrt(pow(p0.rho,2)+pow(p1.rho,2)-2*p0.rho*p1.rho*deg_cos(abs(p0.epsilon-p1.epsilon)));
}

template<>
double get_distance<CartesianCoords>(CartesianCoords p0, CartesianCoords p1){
    return sqrt(pow(p0.x-p1.x,2)+pow(p0.y-p1.y,2));

}

template<typename T>
double get_rectangular_area(T b0, T b1, T b2, T b3);

template<>
double get_rectangular_area<PolarCoords>(PolarCoords s, PolarCoords n, PolarCoords e, PolarCoords w){
    return (M_PI/(abs(e.epsilon-w.epsilon)))*(abs(pow(s.rho,2)-pow(n.rho,2)));
}

template<>
double get_rectangular_area<CartesianCoords>(CartesianCoords s, CartesianCoords n, CartesianCoords e, CartesianCoords w){
    return (abs(s.y-n.y))*(abs(e.x-w.x));
}

template<>
double get_rectangular_area<RealCoords>(RealCoords s, RealCoords n, RealCoords e, RealCoords w){
    return 4*pow(EARTH_PERIMETER,2)*deg_arcsin(deg_tan((abs(e.longitude-w.longitude))/(2*EARTH_PERIMETER))
            *deg_tan((abs(s.latitude-n.latitude))/(2*EARTH_PERIMETER)));
}