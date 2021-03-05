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
#include <cmath>

using namespace std;

class Projection{
public:

    static shared_ptr<Database> db;

    double scale;

    Projection(){};

    //Projection(shared_ptr<Database> &db):db{db}{}

    virtual double calculate_distance(const string &p0, const string &p1){};

    virtual double calculate_rectangular_area(const string &name){}

    virtual void add_point(string &name, double c1, double c2){}

    virtual shared_ptr<Coords> find_point(const string &name){};
};

shared_ptr<Database> Projection::db;

//main type
class AzimuthalProjection : public Projection{
public:
    map<string,shared_ptr<PolarCoords>> points;

    AzimuthalProjection();

    virtual shared_ptr<PolarCoords> compute_coords(RealCoords &coords){
    }

    void add_point(const string &name, double c1, double c2){
        if(points.find(name) == points.end()){
            RealCoords real_coords(c1,c2);
            shared_ptr<PolarCoords> coords = compute_coords(real_coords);
            points[name] = coords;
        }
    }

    void print_points() const{
        for(auto &point : points){
            cout << point.first<<": e="<<point.second->epsilon<<", r="<<point.second->rho<<endl;
        }
    }

    shared_ptr<Coords> find_point(const string &p0){
        shared_ptr<PolarCoords> p0_ptr;
        //auto stuff = points.find(p0);
        if(points.find(p0) == points.end()){
            if(db->data.find(p0) == Projection::db->data.end()){
                return nullptr;
            }
            else if(db->data[p0]->type == POINT) {
                shared_ptr<Point>  point_p0 = static_pointer_cast<Point>(db->data[p0]);
                p0_ptr = compute_coords(*point_p0->coords);
            }
            else{
                shared_ptr<Region>  point_p0 = static_pointer_cast<Region>(db->data[p0]);
                p0_ptr = compute_coords(*point_p0->centroid);
            }
        }
        else{
            p0_ptr = points[p0];
        }
        return p0_ptr;
    }

    double calculate_distance(const string &p0, const string &p1){
        shared_ptr<PolarCoords> p0_ptr = static_pointer_cast<PolarCoords>(find_point(p0));
        shared_ptr<PolarCoords> p1_ptr = static_pointer_cast<PolarCoords>(find_point(p1));
        if(p0_ptr == nullptr || p1_ptr == nullptr){
            cout <<"at least one of the points does not exist."<<endl;
            return 0;
        }

        return get_distance<PolarCoords>(*p0_ptr,*p1_ptr);
    }
};

class GnomonicProjection : public AzimuthalProjection{
public:
    GnomonicProjection() : AzimuthalProjection(){}

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override{
        if(coords.latitude == 0) {
            return nullptr;
        }
        double epsilon = coords.longitude;
        double delta = 90 - coords.latitude;
        double rho = EARTH_PERIMETER * deg_tan(delta);
        return make_shared<PolarCoords>(epsilon,rho);
    }
};

class StereoGraphicProjection : public AzimuthalProjection{
public:
    StereoGraphicProjection() : AzimuthalProjection(){}

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override{
        if(coords.latitude == -90) {
            return nullptr;
        }
        double epsilon = coords.longitude;
        double delta = 90 - coords.latitude;
        double rho = 2 * EARTH_PERIMETER * deg_tan(delta/2);
        return make_shared<PolarCoords>(epsilon,rho);
    }
};

class OrthographicProjection : public AzimuthalProjection{
public:
    OrthographicProjection():AzimuthalProjection() {}

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override{
        double epsilon = coords.longitude;
        double delta = 90 - coords.latitude;
        double rho = EARTH_PERIMETER * deg_sin(delta);
        return make_shared<PolarCoords>(epsilon,rho);
    }
};

class PostelProjection : public AzimuthalProjection{
public:
    PostelProjection() : AzimuthalProjection(){}

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override{
        double epsilon = coords.longitude;
        double delta = 90 - coords.latitude;
        double rho = EARTH_PERIMETER * deg_to_rad(delta);
        return make_shared<PolarCoords>(epsilon,rho);
    }
};

class LambertAzimuthalProjection : public AzimuthalProjection{
public:
    LambertAzimuthalProjection() : AzimuthalProjection(){}

    shared_ptr<PolarCoords> compute_coords(RealCoords &coords) override{
        double epsilon = coords.longitude;
        double delta = 90 - coords.latitude;
        double rho = 2*EARTH_PERIMETER * deg_sin(delta/2);
        return make_shared<PolarCoords>(epsilon,rho);
    }
};


//main type
class CylindricalProjection : public Projection{
    map<string,shared_ptr<CartesianCoords>> points;

    virtual shared_ptr<CartesianCoords> compute_coords(RealCoords &coords){
    }

    void add_point(const string &name, double c1, double c2){
        if(points.find(name) == points.end()){
            RealCoords real_coords(c1,c2);
            shared_ptr<CartesianCoords> coords = compute_coords(real_coords);
            points[name] = coords;
        }
    }

    shared_ptr<Coords> find_point(const string &p0){
        shared_ptr<CartesianCoords> p0_ptr;
        if(points.find(p0) == points.end()){
            if(db->data.find(p0) == db->data.end()){
                return nullptr;
            }
            else if(db->data[p0]->type == POINT) {
                shared_ptr<Point>  point_p0 = static_pointer_cast<Point>(db->data[p0]);
                p0_ptr = compute_coords(*point_p0->coords);
            }
            else{
                shared_ptr<Region>  point_p0 = static_pointer_cast<Region>(db->data[p0]);
                p0_ptr = compute_coords(*point_p0->centroid);
            }
        }
        else{
            p0_ptr = points[p0];
        }
        return p0_ptr;
    }

    double calculate_distance(const string &p0, const string &p1){
        shared_ptr<CartesianCoords> p0_ptr = static_pointer_cast<CartesianCoords>(find_point(p0));
        shared_ptr<CartesianCoords> p1_ptr = static_pointer_cast<CartesianCoords>(find_point(p1));
        if(p0_ptr == nullptr || p1_ptr == nullptr){
            cout <<"at least one of the points does not exist."<<endl;
            return 0;
        }

        return get_distance<CartesianCoords>(*p0_ptr,*p1_ptr);
    }
};

class EquirectangularProjection : public CylindricalProjection{
    EquirectangularProjection() : CylindricalProjection(){}

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override{
        double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
        double y = EARTH_PERIMETER * deg_to_rad(coords.latitude);
        return make_shared<CartesianCoords>(x,y);
    }
};

class LambertCylindricalProjection : public CylindricalProjection{
public:
    LambertCylindricalProjection():CylindricalProjection(){}

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override{
        double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
        double y = EARTH_PERIMETER * deg_sin(coords.latitude);
        return make_shared<CartesianCoords>(x,y);
    }
};

class MercatorProjection : public CylindricalProjection{
public:
    MercatorProjection() : CylindricalProjection(){}

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override{
        if(coords.latitude == 0 || coords.latitude == 180){
            return nullptr;
        }
        double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
        double delta = 90 - coords.latitude;
        double y = EARTH_PERIMETER * log(deg_cot(delta/2));
        return make_shared<CartesianCoords>(x,y);
    }
};

class PerspectiveProjection : public CylindricalProjection{
public:
    PerspectiveProjection():CylindricalProjection(){}

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override{
        double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
        double y = 2 * EARTH_PERIMETER * deg_tan(coords.latitude/2);
        return make_shared<CartesianCoords>(x,y);
    }
};

class BehrmannProjection : public CylindricalProjection{
public:
    BehrmannProjection():CylindricalProjection(){}

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override{
        double phi_0 = 30;
        double x = EARTH_PERIMETER * deg_to_rad(coords.longitude) * deg_cos(phi_0);
        double y = EARTH_PERIMETER * (deg_sin(coords.latitude)/deg_cos(phi_0));
        return make_shared<CartesianCoords>(x,y);
    }
};

class TrystanEdwardsProjection : public CylindricalProjection{
public:
    TrystanEdwardsProjection():CylindricalProjection(){}

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override{
        double phi_0 = deg_arccos(sqrt(2/M_PI));
        double x = EARTH_PERIMETER * deg_to_rad(coords.longitude) * deg_cos(phi_0);
        double y = EARTH_PERIMETER * (deg_sin(coords.latitude)/deg_cos(phi_0));
        return make_shared<CartesianCoords>(x,y);
    }
};

class GallProjection : public CylindricalProjection{
public:
    GallProjection(): CylindricalProjection(){}

    shared_ptr<CartesianCoords> compute_coords(RealCoords &coords) override{
        double phi_0 = 45;
        double x = EARTH_PERIMETER * deg_to_rad(coords.longitude) * deg_cos(phi_0);
        double y = EARTH_PERIMETER * (1+deg_cos(phi_0))*deg_tan(coords.latitude/2);
        return make_shared<CartesianCoords>(x,y);
    }
};


//TODO doplnit projekcie ked zistim, aku hodnotu ma phi_0
//main type
class ConicProjection : public Projection{
    map<string,shared_ptr<PolarCoords>> points;

    virtual shared_ptr<PolarCoords> compute_coords(RealCoords &coords){
    }

    void add_point(const string &name, double c1, double c2){
        if(points.find(name) == points.end()){
            RealCoords real_coords(c1,c2);
            shared_ptr<PolarCoords> coords = compute_coords(real_coords);
            points[name] = coords;
        }
    }

    shared_ptr<Coords> find_point(const string &p0){
        shared_ptr<PolarCoords> p0_ptr;
        if(points.find(p0) == points.end()){
            if(db->data.find(p0) == db->data.end()){
                return nullptr;
            }
            else if(db->data[p0]->type == POINT) {
                shared_ptr<Point>  point_p0 = static_pointer_cast<Point>(db->data[p0]);
                p0_ptr = compute_coords(*point_p0->coords);
            }
            else{
                shared_ptr<Region>  point_p0 = static_pointer_cast<Region>(db->data[p0]);
                p0_ptr = compute_coords(*point_p0->centroid);
            }
        }
        else{
            p0_ptr = points[p0];
        }
        return p0_ptr;
    }

    double calculate_distance(const string &p0, const string &p1){
        shared_ptr<PolarCoords> p0_ptr = static_pointer_cast<PolarCoords>(find_point(p0));
        shared_ptr<PolarCoords> p1_ptr = static_pointer_cast<PolarCoords>(find_point(p1));
        if(p0_ptr == nullptr || p1_ptr == nullptr){
            cout <<"at least one of the points does not exist."<<endl;
            return 0;
        }

        return get_distance<PolarCoords>(*p0_ptr,*p1_ptr);
    }

};



#endif //GEOMAPPROJECTOR_PROJECTIONS_HPP
