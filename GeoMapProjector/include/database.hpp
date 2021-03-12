//
// Created by Ja on 05.03.2021.
//

#ifndef GEOMAPPROJECTOR_DATABASE_HPP
#define GEOMAPPROJECTOR_DATABASE_HPP


//
// Created by Ja on 28.02.2021.
//

#include<utility>
#include<memory>
#include<cmath>
#include<vector>
#include<map>
#include "../include/coords.hpp"

using namespace std;

enum data_type{
    POINT,
    REGION,
    COUNTRY
};

enum coords_type{
    REAL,
    POLAR,
    CARTESIAN
};

class Data{
public:
    enum data_type type;

    Data(enum data_type type);

    virtual string print_coords() const{};

    ~Data(){}
};

class Point : public Data{
public:
    shared_ptr<RealCoords> coords;

    Point(double c1, double c2, enum coords_type type);
    string print_coords() const;

    ~Point(){}
};

class Region : public Data{
public:
    shared_ptr<RealCoords> south;
    shared_ptr<RealCoords> north;
    shared_ptr<RealCoords> east;
    shared_ptr<RealCoords> west;

    shared_ptr<RealCoords> centroid;

    string capital_name;
    shared_ptr<Point> capital;

    Region(double s, double n, double e, double w);

    Region(double s, double n, double e, double w, string &capital_name, double c1, double c2);

    double get_singleton_val(shared_ptr<Coords> coord);

    string print_coords() const;


};

enum object_type{
    CITY,
    CUSTOM,
    MOUNTAIN,
    LAKE,
    STATE,
    CONTINENT
};

class Database{
public:
    map<string,shared_ptr<Data>> data;
    map<string, shared_ptr<Data>> countries;
    map<string, shared_ptr<Data>> lakes;
    map<string, shared_ptr<Data>> cities;
    map<string, shared_ptr<Data>> mountains;
    map<string, shared_ptr<Data>> custom_points;
    map<string, shared_ptr<Data>> custom_regions;
    map<string, shared_ptr<Data>> continents;

    Database(){}

    void add_data(string &name, double c1, double c2, coords_type type, enum object_type obj_type);

    void add_data(string &name, double s, double n, double e, double w, enum object_type obj_type);

    void add_data(string &name, double s, double n, double e, double w,string &capital_name, double c1, double c2);

    shared_ptr<Data> get_data(string &name);

    void print_countries() const;

    void print_cities() const;

    void print_mountains() const;

    void print_lakes() const;

    void print_continents() const;

    void print_custom() const;
};




#endif //GEOMAPPROJECTOR_DATABASE_HPP
