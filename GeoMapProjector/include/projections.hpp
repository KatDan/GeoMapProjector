//
// Created by Ja on 05.03.2021.
//

#ifndef GEOMAPPROJECTOR_PROJECTIONS_HPP
#define GEOMAPPROJECTOR_PROJECTIONS_HPP

#include "../include/coords.hpp"
#include "../include/database.hpp"
#include "../include/my_math.hpp"
#include "../src/db_loader.cpp"

#include<iostream>
#include<map>
#include<string>
#include<memory>
#include<cmath>

using namespace std;

class Projection{
public:

    static shared_ptr<Database> db;

    double scale;

    Projection();

    virtual double calculate_distance(const string &p0, const string &p1){};

    virtual double calculate_rectangular_area(const string &name){}

    virtual void add_point(string &name, double c1, double c2){}

    virtual shared_ptr<Coords> find_point(const string &name){};

    static shared_ptr<Region> find_region(const string &name);

    virtual ~Projection(){};
};

//main type
class AzimuthalProjection : public Projection{
public:
    map<string,shared_ptr<PolarCoords>> points;

    AzimuthalProjection();

    virtual shared_ptr<PolarCoords> compute_coords(RealCoords &coords){}

    void add_point(const string &name, double c1, double c2);

    void print_points() const;

    shared_ptr<Coords> find_point(const string &p0);

    double calculate_distance(const string &p0, const string &p1);

    double calculate_rectangular_area(const string &name);

    virtual ~AzimuthalProjection(){};

};

class GnomonicProjection : public AzimuthalProjection{
public:
    GnomonicProjection();

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;
};

class StereoGraphicProjection : public AzimuthalProjection{
public:
    StereoGraphicProjection();

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;
};

class OrthographicProjection : public AzimuthalProjection{
public:
    OrthographicProjection();

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;
};

class PostelProjection : public AzimuthalProjection{
public:
    PostelProjection();

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;
};

class LambertAzimuthalProjection : public AzimuthalProjection{
public:
    LambertAzimuthalProjection();

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;
};


//main type
class CylindricalProjection : public Projection{
public:
    CylindricalProjection();

    map<string,shared_ptr<CartesianCoords>> points;

    virtual shared_ptr<CartesianCoords> compute_coords(RealCoords &coords){}

    void add_point(const string &name, double c1, double c2);

    shared_ptr<Coords> find_point(const string &p0);

    double calculate_distance(const string &p0, const string &p1);

    double calculate_rectangular_area(const string &name);

    virtual ~CylindricalProjection(){};

};

class EquirectangularProjection : public CylindricalProjection{
    EquirectangularProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;
};

class LambertCylindricalProjection : public CylindricalProjection{
public:
    LambertCylindricalProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;
};

class MercatorProjection : public CylindricalProjection{
public:
    MercatorProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;
};

class PerspectiveProjection : public CylindricalProjection{
public:
    PerspectiveProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;
};

class BehrmannProjection : public CylindricalProjection{
public:
    BehrmannProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;
};

class TrystanEdwardsProjection : public CylindricalProjection{
public:
    TrystanEdwardsProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;
};

class GallProjection : public CylindricalProjection{
public:
    GallProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;
};


//TODO doplnit projekcie ked zistim, aku hodnotu ma phi_0
//main type
class ConicProjection : public Projection{
public:
    map<string,shared_ptr<PolarCoords>> points;

    virtual shared_ptr<PolarCoords> compute_coords(RealCoords &coords){}

    void add_point(const string &name, double c1, double c2);

    shared_ptr<Coords> find_point(const string &p0);

    double calculate_distance(const string &p0, const string &p1);

    double calculate_rectangular_area(const string &name);

    virtual ~ConicProjection(){};

};



#endif //GEOMAPPROJECTOR_PROJECTIONS_HPP
