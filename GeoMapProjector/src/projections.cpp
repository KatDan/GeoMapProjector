#include "../include/projections.hpp"

using namespace std;

AzimuthalProjection::AzimuthalProjection() : Projection() {}


void AzimuthalProjection::add_point(const string &name, double c1, double c2){
    if(points.find(name) == points.end()){
        RealCoords real_coords(c1,c2);
        shared_ptr<PolarCoords> coords = compute_coords(real_coords);
        points[name] = coords;
    }
}

void AzimuthalProjection::print_points() const{
    for(auto &point : points){
        cout << point.first<<": e="<<point.second->epsilon<<", r="<<point.second->rho<<endl;
    }
}

shared_ptr<Coords> AzimuthalProjection::find_point(const string &p0){
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

double AzimuthalProjection::calculate_distance(const string &p0, const string &p1){
    shared_ptr<PolarCoords> p0_ptr = static_pointer_cast<PolarCoords>(find_point(p0));
    shared_ptr<PolarCoords> p1_ptr = static_pointer_cast<PolarCoords>(find_point(p1));
    if(p0_ptr == nullptr || p1_ptr == nullptr){
        cout <<"at least one of the points does not exist."<<endl;
        return 0;
    }

    return get_distance<PolarCoords>(*p0_ptr,*p1_ptr);
}



GnomonicProjection::GnomonicProjection() : AzimuthalProjection(){}

shared_ptr<PolarCoords> GnomonicProjection::compute_coords(RealCoords &coords){
    if(coords.latitude == 0) {
        return nullptr;
    }
    double epsilon = coords.longitude;
    double delta = 90 - coords.latitude;
    double rho = EARTH_PERIMETER * deg_tan(delta);
    return make_shared<PolarCoords>(epsilon,rho);
}


StereoGraphicProjection::StereoGraphicProjection() : AzimuthalProjection(){}

shared_ptr<PolarCoords> StereoGraphicProjection::compute_coords(RealCoords &coords){
    if(coords.latitude == -90) {
        return nullptr;
    }
    double epsilon = coords.longitude;
    double delta = 90 - coords.latitude;
    double rho = 2 * EARTH_PERIMETER * deg_tan(delta/2);
    return make_shared<PolarCoords>(epsilon,rho);
}



OrthographicProjection::OrthographicProjection():AzimuthalProjection() {}

shared_ptr<PolarCoords> OrthographicProjection::compute_coords(RealCoords &coords){
    double epsilon = coords.longitude;
    double delta = 90 - coords.latitude;
    double rho = EARTH_PERIMETER * deg_sin(delta);
    return make_shared<PolarCoords>(epsilon,rho);
}


PostelProjection::PostelProjection() : AzimuthalProjection(){}

shared_ptr<PolarCoords> PostelProjection::compute_coords(RealCoords &coords){
    double epsilon = coords.longitude;
    double delta = 90 - coords.latitude;
    double rho = EARTH_PERIMETER * deg_to_rad(delta);
    return make_shared<PolarCoords>(epsilon,rho);
}


LambertAzimuthalProjection::LambertAzimuthalProjection() : AzimuthalProjection(){}

shared_ptr<PolarCoords> LambertAzimuthalProjection::compute_coords(RealCoords &coords){
    double epsilon = coords.longitude;
    double delta = 90 - coords.latitude;
    double rho = 2*EARTH_PERIMETER * deg_sin(delta/2);
    return make_shared<PolarCoords>(epsilon,rho);
}


CylindricalProjection::CylindricalProjection() : Projection(){}

void CylindricalProjection::add_point(const string &name, double c1, double c2){
    if(points.find(name) == points.end()){
        RealCoords real_coords(c1,c2);
        shared_ptr<CartesianCoords> coords = compute_coords(real_coords);
        points[name] = coords;
    }
}

shared_ptr<Coords> CylindricalProjection::find_point(const string &p0){
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

double CylindricalProjection::calculate_distance(const string &p0, const string &p1){
    shared_ptr<CartesianCoords> p0_ptr = static_pointer_cast<CartesianCoords>(find_point(p0));
    shared_ptr<CartesianCoords> p1_ptr = static_pointer_cast<CartesianCoords>(find_point(p1));
    if(p0_ptr == nullptr || p1_ptr == nullptr){
        cout <<"at least one of the points does not exist."<<endl;
        return 0;
    }

    return get_distance<CartesianCoords>(*p0_ptr,*p1_ptr);
}



EquirectangularProjection::EquirectangularProjection() : CylindricalProjection(){}

shared_ptr<CartesianCoords> EquirectangularProjection::compute_coords(RealCoords &coords){
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
    double y = EARTH_PERIMETER * deg_to_rad(coords.latitude);
    return make_shared<CartesianCoords>(x,y);
}



LambertCylindricalProjection::LambertCylindricalProjection():CylindricalProjection(){}

shared_ptr<CartesianCoords> LambertCylindricalProjection::compute_coords(RealCoords &coords){
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
    double y = EARTH_PERIMETER * deg_sin(coords.latitude);
    return make_shared<CartesianCoords>(x,y);
}



MercatorProjection::MercatorProjection() : CylindricalProjection(){}

shared_ptr<CartesianCoords> MercatorProjection::compute_coords(RealCoords &coords){
    if(coords.latitude == 0 || coords.latitude == 180){
        return nullptr;
    }
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
    double delta = 90 - coords.latitude;
    double y = EARTH_PERIMETER * log(deg_cot(delta/2));
    return make_shared<CartesianCoords>(x,y);
}


PerspectiveProjection::PerspectiveProjection():CylindricalProjection(){}

shared_ptr<CartesianCoords> PerspectiveProjection::compute_coords(RealCoords &coords){
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
    double y = 2 * EARTH_PERIMETER * deg_tan(coords.latitude/2);
    return make_shared<CartesianCoords>(x,y);
}



BehrmannProjection::BehrmannProjection():CylindricalProjection(){}

shared_ptr<CartesianCoords> BehrmannProjection::compute_coords(RealCoords &coords){
    double phi_0 = 30;
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude) * deg_cos(phi_0);
    double y = EARTH_PERIMETER * (deg_sin(coords.latitude)/deg_cos(phi_0));
    return make_shared<CartesianCoords>(x,y);
}


TrystanEdwardsProjection::TrystanEdwardsProjection():CylindricalProjection(){}

shared_ptr<CartesianCoords> TrystanEdwardsProjection::compute_coords(RealCoords &coords){
    double phi_0 = deg_arccos(sqrt(2/M_PI));
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude) * deg_cos(phi_0);
    double y = EARTH_PERIMETER * (deg_sin(coords.latitude)/deg_cos(phi_0));
    return make_shared<CartesianCoords>(x,y);
}



GallProjection::GallProjection(): CylindricalProjection(){}

shared_ptr<CartesianCoords> GallProjection::compute_coords(RealCoords &coords){
    double phi_0 = 45;
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude) * deg_cos(phi_0);
    double y = EARTH_PERIMETER * (1+deg_cos(phi_0))*deg_tan(coords.latitude/2);
    return make_shared<CartesianCoords>(x,y);
}


    void ConicProjection::add_point(const string &name, double c1, double c2){
        if(points.find(name) == points.end()){
            RealCoords real_coords(c1,c2);
            shared_ptr<PolarCoords> coords = compute_coords(real_coords);
            points[name] = coords;
        }
    }

    shared_ptr<Coords> ConicProjection::find_point(const string &p0){
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

    double ConicProjection::calculate_distance(const string &p0, const string &p1){
        shared_ptr<PolarCoords> p0_ptr = static_pointer_cast<PolarCoords>(find_point(p0));
        shared_ptr<PolarCoords> p1_ptr = static_pointer_cast<PolarCoords>(find_point(p1));
        if(p0_ptr == nullptr || p1_ptr == nullptr){
            cout <<"at least one of the points does not exist."<<endl;
            return 0;
        }

        return get_distance<PolarCoords>(*p0_ptr,*p1_ptr);
    }

shared_ptr<Database> Projection::db = nullptr;

Projection::Projection() {
    if(!db){
        db_loader loader;
        Database database;
        loader.add_initial_data(database);
        db = make_shared<Database>(database);
    }
}
