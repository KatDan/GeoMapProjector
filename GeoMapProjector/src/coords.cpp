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

string Coords::get_coords() const {
    if(singleton_type == BOTH){
        return to_string(coords_pair.first) + " "+to_string(coords_pair.second);
    }
    else if(singleton_type == LATITUDE) return to_string(coords_pair.first);
    else return to_string(coords_pair.second);
}

RealCoords::RealCoords(double lat, double lon):Coords(normalize_coords(lat,lon).first,normalize_coords(lat,lon).second){
    auto normalized = normalize_coords(lat,lon);
    latitude = normalized.first;
    longitude = normalized.second;
}
RealCoords::RealCoords(double value, enum lat_or_long type):Coords(type) {
    if (type == LATITUDE) {
        latitude = normalize_coords(value,0).first;
        longitude = -1;
    }
    else if (type == LONGITUDE){
        longitude = normalize_coords(0,value).second;
        latitude = -1;
    }
}

double RealCoords::get_singleton_value() {
    if(singleton_type == LATITUDE) return latitude;
    else if(singleton_type == LONGITUDE) return longitude;
    else return 0;
}

pair<double, double> RealCoords::normalize_coords(double lat, double lon) {
    while(lat < -90){
        lat += 180;
    }
    while(lat > 90){
        lat -= 180;
    }
    while(lon < -180){
        lon += 360;
    }
    while(lon > 180){
        lon -= 360;
    }
    return make_pair(lat,lon);
}


PolarCoords::PolarCoords(double e, double r):Coords(normalize_coords(e,r).first,normalize_coords(e,r).second){
    auto normalized = normalize_coords(e,r);
    epsilon = normalized.second;
    rho = normalized.first;
}
PolarCoords::PolarCoords(double value, enum lat_or_long type):Coords(type){
    if(type == LATITUDE) rho = normalize_coords(0,value).second;
    else if(type == LONGITUDE) epsilon = normalize_coords(value,0).first;
}

double PolarCoords::get_singleton_value() {
    if(singleton_type == LATITUDE) return rho;
    else if(singleton_type == LONGITUDE) return epsilon;
    else return 0;
}

pair<double, double> PolarCoords::normalize_coords(double r, double e) {
    while(e < -180){
        e += 360;
    }
    while(e > 180){
        e -= 360;
    }
    return make_pair(r,e);
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

pair<double, double> CartesianCoords::normalize_coords(double x, double y) {
    return make_pair(x,y);
}


template<typename T>
double get_distance(T p0, T p1);

template<>
double get_distance<RealCoords>(RealCoords p0, RealCoords p1){
    double sigma = acos(deg_sin(90-p0.latitude)*deg_sin(90-p1.latitude)*deg_cos(abs(p0.longitude-p1.longitude))+
            deg_cos(90-p0.latitude)*deg_cos(90-p1.latitude));
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
    return (abs(e.epsilon-w.epsilon)/360)*M_PI*(abs(pow(s.rho,2)-pow(n.rho,2)));
}

template<>
double get_rectangular_area<CartesianCoords>(CartesianCoords s, CartesianCoords n, CartesianCoords e, CartesianCoords w){
    return (abs(s.y-n.y))*(abs(e.x-w.x));
}

template<>
double get_rectangular_area<RealCoords>(RealCoords s, RealCoords n, RealCoords e, RealCoords w){
    return 4*pow(EARTH_PERIMETER,2)*asin(deg_tan((abs(e.longitude-w.longitude))/(2*EARTH_PERIMETER))
            *deg_tan((abs(s.latitude-n.latitude))/(2*EARTH_PERIMETER)));
}