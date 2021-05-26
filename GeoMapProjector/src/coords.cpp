//
// Created by Ja on 28.02.2021.
//

#include "../include/coords.hpp"

using namespace std;

Coords::Coords(lat_or_long type):singleton_type{type}{}
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
RealCoords::RealCoords(double value, lat_or_long type):Coords(type) {
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

string RealCoords::get_coords() const {
    return "lat="+to_string(latitude)+", lon="+to_string(longitude);
}


PolarCoords::PolarCoords(double r, double e):Coords(normalize_coords(r,e).first,normalize_coords(r,e).second){
    auto normalized = normalize_coords(r,e);
    epsilon = normalized.second;
    rho = normalized.first;
}
PolarCoords::PolarCoords(double value, lat_or_long type):Coords(type){
    if(type == LATITUDE) rho = value;
    else if(type == LONGITUDE) epsilon = normalize_coords(0,value).second;
}

double PolarCoords::get_singleton_value() {
    if(singleton_type == LATITUDE) return rho;
    else if(singleton_type == LONGITUDE) return epsilon;
    else return 0;
}

pair<double, double> PolarCoords::normalize_coords(double r, double e) {
    while(e < 0){
        e += 360;
    }
    while(e > 360){
        e -= 360;
    }
    return make_pair(r,e);
}

string PolarCoords::get_coords() const {
    return "rho="+to_string(rho)+", e="+to_string(epsilon);
}


CartesianCoords::CartesianCoords(double x, double y):Coords(y,x),x{x},y{y}{}
CartesianCoords::CartesianCoords(double value, lat_or_long type):Coords(type){
    if(type == LATITUDE) y = value;
    else if(type == LONGITUDE) x = value;
}

double CartesianCoords::get_singleton_value() {
    if(singleton_type == LATITUDE) return y;
    else if(singleton_type == LONGITUDE) return x;
    else return 0;
}

string CartesianCoords::get_coords() const {
    return "y="+to_string(y)+", x="+to_string(x);
}



template<>
double get_distance<RealCoords>(const RealCoords &p0, const RealCoords &p1){
    double sigma = acos(deg_sin(90-p0.latitude)*deg_sin(90-p1.latitude)*deg_cos(abs(p0.longitude-p1.longitude))+
            deg_cos(90-p0.latitude)*deg_cos(90-p1.latitude));
    return sigma * EARTH_PERIMETER;
}

template<>
double get_distance<PolarCoords>(const PolarCoords &p0, const PolarCoords &p1){
    return sqrt(pow(p0.rho,2)+pow(p1.rho,2)-2*p0.rho*p1.rho*deg_cos(abs(p0.epsilon-p1.epsilon)));
}

template<>
double get_distance<CartesianCoords>(const CartesianCoords &p0, const CartesianCoords &p1){
    return sqrt(pow(p0.x-p1.x,2)+pow(p0.y-p1.y,2));

}

template<>
double get_rectangular_area<PolarCoords>(const PolarCoords &s, const PolarCoords &n, const PolarCoords &e, const PolarCoords &w){
    return (abs(e.epsilon-w.epsilon)/360)*M_PI*(abs(pow(s.rho,2)-pow(n.rho,2)));
}

template<>
double get_rectangular_area<CartesianCoords>(const CartesianCoords &s, const CartesianCoords &n, const CartesianCoords &e, const CartesianCoords &w){
    return (abs(s.y-n.y))*(abs(e.x-w.x));
}

template<>
double get_rectangular_area<RealCoords>(const RealCoords &s, const RealCoords &n, const RealCoords &e, const RealCoords &w){
    double theta_1 = 90 - s.latitude;
    double theta_2 = 90 - n.latitude;
    double segment_area = 2*pow(EARTH_PERIMETER,2)*M_PI*(deg_cos(theta_2)-deg_cos(theta_1));
    return segment_area * (abs(e.longitude-w.longitude)/360);
}