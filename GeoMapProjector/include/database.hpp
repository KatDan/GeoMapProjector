//
// Created by Ja on 05.03.2021.
//

#ifndef GEOMAPPROJECTOR_DATABASE_HPP
#define GEOMAPPROJECTOR_DATABASE_HPP

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
    CARTESIAN,
    POLAR,
};

class Data{
public:
    enum data_type type;

    Data(data_type type);

    virtual string print_coords() const = 0;

    ~Data(){}
};

class Point : public Data{
public:
    shared_ptr<Coords> coords;

    Point(double c1, double c2);

    Point(double c1, double c2, coords_type type);

    string print_coords() const;

    ~Point(){}
};

class Region : public Data{
public:
    shared_ptr<Coords> south;
    shared_ptr<Coords> north;
    shared_ptr<Coords> east;
    shared_ptr<Coords> west;

    shared_ptr<Coords> centroid;

    string capital_name;
    shared_ptr<Point> capital;

    Region(double s, double n, double e, double w);

    Region(double s, double n, double e, double w, coords_type type);

    Region(double s, double n, double e, double w, const string &capital_name, double c1, double c2);

    Region(double s, double n, double e, double w, const string &capital_name, double c1, double c2,coords_type type);

    string print_coords() const;
};

enum object_type{
    CITY,
    CUSTOM,
    MOUNTAIN,
    LAKE,
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

    void add_data(const string &name, double c1, double c2, object_type obj_type);

    void add_data(const string &name, double s, double n, double e, double w, object_type obj_type);

    void add_data(const string &name, double s, double n, double e, double w,const string &capital_name, double c1, double c2);

    void print_countries() const;

    void print_cities() const;

    void print_mountains() const;

    void print_lakes() const;

    void print_continents() const;

    void print_custom() const;
};




#endif //GEOMAPPROJECTOR_DATABASE_HPP
