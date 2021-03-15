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

    Projection();

    virtual double calculate_distance(const string &p0, const string &p1){return 0;};

    virtual shared_ptr<RealCoords> convert_to_real_coords(const string &p){return nullptr;}

    virtual double calculate_rectangular_area(const string &name){return 0;}

    virtual void add_point(string &name, double c1, double c2){};

    virtual shared_ptr<Coords> find_point(const string &name){return nullptr;};

    static shared_ptr<Region> find_region(const string &name);

    virtual ~Projection(){};

    virtual void print_local(){};

};

class RealProjection : public Projection{
public:
    RealProjection();

    static shared_ptr<RealCoords> compute_coords(RealCoords &coords);

    shared_ptr<RealCoords> convert_to_real_coords(const string &p) override;

    shared_ptr<Coords> find_point(const string &p0) override;

    double calculate_distance(const string &p0, const string &p1) override;

    double calculate_rectangular_area(const string &name) override;

    void print_local() override;
};

//main type
class AzimuthalProjection : public Projection{
public:
    map<string,shared_ptr<PolarCoords>> points;

    AzimuthalProjection();

    virtual shared_ptr<PolarCoords> compute_coords(RealCoords &coords){return nullptr;}

    shared_ptr<RealCoords> convert_to_real_coords(const string &p) override;

    virtual shared_ptr<RealCoords> decompute_coords(PolarCoords &coords){return nullptr;}

    void add_point(string &name, double c1, double c2) override;

    shared_ptr<Coords> find_point(const string &p0) override;

    double calculate_distance(const string &p0, const string &p1) override;

    double calculate_rectangular_area(const string &name) override;

    virtual ~AzimuthalProjection(){};

    void print_local() override;

};

class GnomonicProjection : public AzimuthalProjection{
public:
    GnomonicProjection();

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(PolarCoords &coords) override;
};

class StereoGraphicProjection : public AzimuthalProjection{
public:
    StereoGraphicProjection();

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(PolarCoords &coords) override;
};

class OrthographicProjection : public AzimuthalProjection{
public:
    OrthographicProjection();

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(PolarCoords &coords) override;
};

class PostelProjection : public AzimuthalProjection{
public:
    PostelProjection();

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(PolarCoords &coords) override;
};

class LambertAzimuthalProjection : public AzimuthalProjection{
public:
    LambertAzimuthalProjection();

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(PolarCoords &coords) override;
};

class WernerStabProjectionSpecial : public AzimuthalProjection{
public:
    WernerStabProjectionSpecial(){}

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(PolarCoords &coords) override;
};


//main type
class CylindricalProjection : public Projection{
public:
    CylindricalProjection();

    map<string,shared_ptr<CartesianCoords>> points;

    virtual shared_ptr<CartesianCoords> compute_coords(RealCoords &coords){return nullptr;}

    shared_ptr<RealCoords> convert_to_real_coords(const string &p) override;

    virtual shared_ptr<RealCoords> decompute_coords(CartesianCoords &coords){return nullptr;};

    void add_point(string &name, double c1, double c2) override;

    shared_ptr<Coords> find_point(const string &p0) override;

    double calculate_distance(const string &p0, const string &p1) override;

    double calculate_rectangular_area(const string &name) override;

    virtual ~CylindricalProjection(){};

    void print_local() override;

};

class EquirectangularProjection : public CylindricalProjection{
public:
    EquirectangularProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(CartesianCoords &coords) override;
};

class LambertCylindricalProjection : public CylindricalProjection{
public:
    LambertCylindricalProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(CartesianCoords &coords) override;
};

class MercatorProjection : public CylindricalProjection{
public:
    MercatorProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(CartesianCoords &coords) override;
};

class PerspectiveProjection : public CylindricalProjection{
public:
    PerspectiveProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(CartesianCoords &coords) override;
};

class BehrmannProjection : public CylindricalProjection{
public:
    BehrmannProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(CartesianCoords &coords) override;
};

class TrystanEdwardsProjection : public CylindricalProjection{
public:
    TrystanEdwardsProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(CartesianCoords &coords) override;
};

class GallProjection : public CylindricalProjection{
public:
    GallProjection();

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(CartesianCoords &coords) override;
};

class SansonProjectionSpecial : public CylindricalProjection{
public:
    SansonProjectionSpecial(){}

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(CartesianCoords &coords) override;
};


//main type
class ConicProjection : public Projection{
public:
    map<string,shared_ptr<PolarCoords>> points;

    virtual shared_ptr<PolarCoords> compute_coords(RealCoords &coords){return nullptr;}

    shared_ptr<RealCoords> convert_to_real_coords(const string &p) override;

    virtual shared_ptr<RealCoords> decompute_coords(PolarCoords &coords){return nullptr;}

    void add_point(string &name, double c1, double c2) override;

    shared_ptr<Coords> find_point(const string &p0) override;

    double calculate_distance(const string &p0, const string &p1) override;

    double calculate_rectangular_area(const string &name) override;

    virtual ~ConicProjection(){};

    void print_local() override;
};

class PtolemyProjection : public ConicProjection{
public:
    PtolemyProjection(){}

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(PolarCoords &coords) override;
};

class LambertConicProjection : public ConicProjection{
public:
    LambertConicProjection(){};

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override;

    shared_ptr<RealCoords> decompute_coords(PolarCoords &coords) override;
};



#endif //GEOMAPPROJECTOR_PROJECTIONS_HPP
