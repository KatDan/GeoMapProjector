#include <limits>
#include "../include/projections.hpp"

using namespace std;

shared_ptr<Region> RealProjection::find_region(const string &name) const{
    return get_region_from_db(name);
}


AzimuthalProjection::AzimuthalProjection() : Projection() {}


void AzimuthalProjection::add_point(string &name, double c1, double c2){
    if(points.find(name) == points.end()){
        shared_ptr<PolarCoords> coords = make_shared<PolarCoords>(c1,c2);
        if(coords == nullptr){
            cout <<"unable to project this point at the current projection."<<endl;
            return;
        }
        points[name] = coords;
    }
}

shared_ptr<Coords> AzimuthalProjection::find_point(const string &p0) const{
    shared_ptr<PolarCoords> p0_ptr;
    if(p0.find('.') != string::npos){
        auto dot_it = p0.find('.');

        string country = p0.substr(0,dot_it);
        string is_capital = p0.substr(dot_it+1);
        if(is_capital != "capital") return nullptr;

        if(db->countries.find(country) == db->countries.end()) return nullptr;
        auto capital = static_pointer_cast<Region>(db->countries[country])->capital;
        p0_ptr = compute_coords(dynamic_cast<RealCoords &>(*capital->coords));
    }
    else if(points.find(p0) == points.end()){
        if(db->data.find(p0) == Projection::db->data.end()){
            return nullptr;
        }
        else if(db->data[p0]->type == POINT) {
            shared_ptr<Point>  point_p0 = static_pointer_cast<Point>(db->data[p0]);
            p0_ptr = compute_coords(dynamic_cast<RealCoords &>(*point_p0->coords));
        }
        else{
            shared_ptr<Region>  point_p0 = static_pointer_cast<Region>(db->data[p0]);
            p0_ptr = compute_coords(*(static_pointer_cast<RealCoords>(point_p0->centroid)));
        }
    }
    else{
        p0_ptr = points.at(p0);
    }
    return p0_ptr;
}

double AzimuthalProjection::calculate_distance(const string &p0, const string &p1) const{
    shared_ptr<PolarCoords> p0_ptr = static_pointer_cast<PolarCoords>(find_point(p0));
    shared_ptr<PolarCoords> p1_ptr = static_pointer_cast<PolarCoords>(find_point(p1));
    if(p0_ptr == nullptr || p1_ptr == nullptr){
        cout <<"at least one of the points does not exist."<<endl;
        return 0;
    }

    return get_distance<PolarCoords>(*p0_ptr,*p1_ptr);
}

double AzimuthalProjection::calculate_rectangular_area(const string &name) const{
    shared_ptr<Region> region = find_region(name);
    if(region == nullptr) return 0;

    auto s = static_pointer_cast<PolarCoords>(region->south);
    auto n = static_pointer_cast<PolarCoords>(region->north);
    auto e = static_pointer_cast<PolarCoords>(region->east);
    auto w = static_pointer_cast<PolarCoords>(region->west);

    double result = get_rectangular_area<PolarCoords>(*s,*n,*e,*w);

    return result;
}

void AzimuthalProjection::print_local() const {
    cout << "-------------------------------------------"<<endl;
    cout << points.size()<<" local point(s) saved:"<<endl;
    for(auto &point : points){
        cout <<" "<<point.first<<": "<<point.second->get_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

shared_ptr<RealCoords> AzimuthalProjection::convert_to_real_coords(const string &p) const{
    shared_ptr<PolarCoords> p_ptr = static_pointer_cast<PolarCoords>(find_point(p));
    if(p_ptr == nullptr){
        cout <<"the point does not exist."<<endl;
        return nullptr;
    }
    auto result = decompute_coords(*p_ptr);
    return result;
}

shared_ptr<Region> AzimuthalProjection::find_region(const string &name) const{
    auto region = get_region_from_db(name);
    if(region == nullptr) return nullptr;

    auto polar_s = compute_coords(dynamic_cast<RealCoords &>(*region->south));
    if(polar_s == nullptr){
        cout << "unable to project south in this type of projection."<<endl;
        return nullptr;
    }

    auto polar_n = compute_coords(dynamic_cast<RealCoords &>(*region->north));
    if(polar_n == nullptr){
        cout << "unable to project north in this type of projection."<<endl;
        return nullptr;
    }

    auto polar_e = compute_coords(dynamic_cast<RealCoords &>(*region->east));
    if(polar_e == nullptr){
        cout << "unable to project east in this type of projection."<<endl;
        return nullptr;
    }

    auto polar_w = compute_coords(dynamic_cast<RealCoords &>(*region->west));
    if(polar_w == nullptr){
        cout << "unable to project west in this type of projection."<<endl;
        return nullptr;
    }

    auto s = polar_s->rho;
    auto n = polar_n->rho;
    auto e = polar_e->epsilon;
    auto w = polar_w->epsilon;

    if(!region->capital_name.empty()){
         auto capital = compute_coords(dynamic_cast<RealCoords &>(*region->capital->coords));
         return make_shared<Region>(s,n,e,w,region->capital_name,capital->rho,capital->epsilon,coords_type::POLAR);
    }
    else return make_shared<Region>(s,n,e,w,POLAR);
}


GnomonicProjection::GnomonicProjection() : AzimuthalProjection(){}

shared_ptr<PolarCoords> GnomonicProjection::compute_coords(RealCoords &coords) const{
    if(coords.latitude == 0) {
        return nullptr;
    }
    double epsilon = coords.longitude;
    double delta = 90 - coords.latitude;
    double rho = EARTH_PERIMETER * deg_tan(delta);
    return make_shared<PolarCoords>(rho,epsilon);
}

shared_ptr<RealCoords> GnomonicProjection::decompute_coords(PolarCoords &coords) const{
    double lambda = coords.epsilon;
    double phi = 90 - rad_to_deg(atan(coords.rho/EARTH_PERIMETER));
    return make_shared<RealCoords>(phi,lambda);
}

StereoGraphicProjection::StereoGraphicProjection() : AzimuthalProjection(){}

shared_ptr<PolarCoords> StereoGraphicProjection::compute_coords(RealCoords &coords) const{
    if(coords.latitude == -90) {
        return nullptr;
    }
    double epsilon = coords.longitude;
    double delta = 90 - coords.latitude;
    double rho = 2 * EARTH_PERIMETER * deg_tan(delta/2);
    return make_shared<PolarCoords>(rho,epsilon);
}

shared_ptr<RealCoords> StereoGraphicProjection::decompute_coords(PolarCoords &coords) const{
    double lambda = coords.epsilon;
    double phi = 90 - 2 * rad_to_deg(atan(coords.rho/(2*EARTH_PERIMETER)));
    return make_shared<RealCoords>(phi,lambda);
}

OrthographicProjection::OrthographicProjection():AzimuthalProjection() {}

shared_ptr<PolarCoords> OrthographicProjection::compute_coords(RealCoords &coords) const{
    double epsilon = coords.longitude;
    double delta = 90 - coords.latitude;
    double rho = EARTH_PERIMETER * deg_sin(delta);
    return make_shared<PolarCoords>(rho,epsilon);
}

shared_ptr<RealCoords> OrthographicProjection::decompute_coords(PolarCoords &coords) const{
    double lambda = coords.epsilon;
    double phi = 90 - rad_to_deg(asin(coords.rho/EARTH_PERIMETER));
    return make_shared<RealCoords>(phi,lambda);
}

PostelProjection::PostelProjection() : AzimuthalProjection(){}

shared_ptr<PolarCoords> PostelProjection::compute_coords(RealCoords &coords) const{
    double epsilon = coords.longitude;
    double delta = 90 - coords.latitude;
    double rho = EARTH_PERIMETER * deg_to_rad(delta);
    return make_shared<PolarCoords>(rho,epsilon);
}

shared_ptr<RealCoords> PostelProjection::decompute_coords(PolarCoords &coords) const{
    double lambda = coords.epsilon;
    double phi = 90 - rad_to_deg(coords.rho/EARTH_PERIMETER);
    return make_shared<RealCoords>(phi,lambda);
}

LambertAzimuthalProjection::LambertAzimuthalProjection() : AzimuthalProjection(){}

shared_ptr<PolarCoords> LambertAzimuthalProjection::compute_coords(RealCoords &coords) const{
    double epsilon = coords.longitude;
    double delta = 90 - coords.latitude;
    double rho = 2*EARTH_PERIMETER * deg_sin(delta/2);
    return make_shared<PolarCoords>(rho,epsilon);
}

shared_ptr<RealCoords> LambertAzimuthalProjection::decompute_coords(PolarCoords &coords) const{
    double lambda = coords.epsilon;
    double phi = 90 - 2*rad_to_deg(asin(coords.rho/(2*EARTH_PERIMETER)));
    return make_shared<RealCoords>(phi,lambda);
}


CylindricalProjection::CylindricalProjection() : Projection(){}

void CylindricalProjection::add_point(string &name, double c1, double c2){
    if(points.find(name) == points.end()){
        shared_ptr<CartesianCoords> coords = make_shared<CartesianCoords>(c2,c1);
        if(coords == nullptr){
            cout << "unable to project this point in the current projection."<<endl;
            return;
        }
        points[name] = coords;
    }
}

shared_ptr<Coords> CylindricalProjection::find_point(const string &p0) const{
    shared_ptr<CartesianCoords> p0_ptr;
    if(p0.find('.') != string::npos){
        auto dot_it = p0.find('.');

        string country = p0.substr(0,dot_it);
        string is_capital = p0.substr(dot_it+1);
        if(is_capital != "capital") return nullptr;

        if(db->countries.find(country) == db->countries.end()) return nullptr;
        auto capital = static_pointer_cast<Region>(db->countries[country])->capital;
        p0_ptr = compute_coords(dynamic_cast<RealCoords &>(*capital->coords));
    }
    else if(points.find(p0) == points.end()){
        if(db->data.find(p0) == db->data.end()){
            return nullptr;
        }
        else if(db->data[p0]->type == POINT) {
            shared_ptr<Point>  point_p0 = static_pointer_cast<Point>(db->data[p0]);
            p0_ptr = compute_coords(dynamic_cast<RealCoords &>(*point_p0->coords));
        }
        else{
            shared_ptr<Region>  point_p0 = static_pointer_cast<Region>(db->data[p0]);
            p0_ptr = compute_coords(*static_pointer_cast<RealCoords>(point_p0->centroid));
        }
    }
    else{
        p0_ptr = points.at(p0);
    }
    return p0_ptr;
}

double CylindricalProjection::calculate_distance(const string &p0, const string &p1) const{
    shared_ptr<CartesianCoords> p0_ptr = static_pointer_cast<CartesianCoords>(find_point(p0));
    shared_ptr<CartesianCoords> p1_ptr = static_pointer_cast<CartesianCoords>(find_point(p1));
    if(p0_ptr == nullptr || p1_ptr == nullptr){
        cout <<"at least one of the points does not exist."<<endl;
        return 0;
    }

    return get_distance<CartesianCoords>(*p0_ptr,*p1_ptr);
}

double CylindricalProjection::calculate_rectangular_area(const string &name) const{
    shared_ptr<Region> region = find_region(name);
    if(region == nullptr) return 0;

    auto s = static_pointer_cast<CartesianCoords>(region->south);
    auto n = static_pointer_cast<CartesianCoords>(region->north);
    auto e = static_pointer_cast<CartesianCoords>(region->east);
    auto w = static_pointer_cast<CartesianCoords>(region->west);

    double result = get_rectangular_area<CartesianCoords>(*s,*n,*e,*w);

    return result;
}

void CylindricalProjection::print_local() const {
    cout << "-------------------------------------------"<<endl;
    cout << points.size()<<" local point(s) saved:"<<endl;
    for(auto &point : points){
        cout <<" "<<point.first<<": "<<point.second->get_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

shared_ptr<RealCoords> CylindricalProjection::convert_to_real_coords(const string &p) const{
    shared_ptr<CartesianCoords> p_ptr = static_pointer_cast<CartesianCoords>(find_point(p));
    if(p_ptr == nullptr){
        cout <<"the point does not exist."<<endl;
        return nullptr;
    }
    auto result = decompute_coords(*p_ptr);
    return result;
}

shared_ptr<Region> CylindricalProjection::find_region(const string &name) const{
    auto region = get_region_from_db(name);
    if(region == nullptr) return nullptr;

    auto polar_s = compute_coords(dynamic_cast<RealCoords &>(*region->south));
    if(polar_s == nullptr){
        cout << "unable to project south in this type of projection."<<endl;
        return nullptr;
    }

    auto polar_n = compute_coords(dynamic_cast<RealCoords &>(*region->north));
    if(polar_n == nullptr){
        cout << "unable to project north in this type of projection."<<endl;
        return nullptr;
    }

    auto polar_e = compute_coords(dynamic_cast<RealCoords &>(*region->east));
    if(polar_e == nullptr){
        cout << "unable to project east in this type of projection."<<endl;
        return nullptr;
    }

    auto polar_w = compute_coords(dynamic_cast<RealCoords &>(*region->west));
    if(polar_w == nullptr){
        cout << "unable to project west in this type of projection."<<endl;
        return nullptr;
    }

    auto s = polar_s->y;
    auto n = polar_n->y;
    auto e = polar_e->x;
    auto w = polar_w->x;

    if(!region->capital_name.empty()){
        auto capital = compute_coords(dynamic_cast<RealCoords &>(*region->capital->coords));
        return make_shared<Region>(s,n,e,w,region->capital_name,capital->y,capital->x,coords_type::CARTESIAN);
    }
    else return make_shared<Region>(s,n,e,w,CARTESIAN);
}

EquirectangularProjection::EquirectangularProjection() : CylindricalProjection(){}

shared_ptr<CartesianCoords> EquirectangularProjection::compute_coords(RealCoords &coords) const{
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
    double y = EARTH_PERIMETER * deg_to_rad(coords.latitude);
    return make_shared<CartesianCoords>(x,y);
}

shared_ptr<RealCoords> EquirectangularProjection::decompute_coords(CartesianCoords &coords) const{
    double lambda = rad_to_deg(coords.x/EARTH_PERIMETER);
    double phi = rad_to_deg(coords.y/EARTH_PERIMETER);
    return make_shared<RealCoords>(phi,lambda);
}

LambertCylindricalProjection::LambertCylindricalProjection():CylindricalProjection(){}

shared_ptr<CartesianCoords> LambertCylindricalProjection::compute_coords(RealCoords &coords) const{
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
    double y = EARTH_PERIMETER * deg_sin(coords.latitude);
    return make_shared<CartesianCoords>(x,y);
}

shared_ptr<RealCoords> LambertCylindricalProjection::decompute_coords(CartesianCoords &coords) const{
    double lambda = rad_to_deg(coords.x/EARTH_PERIMETER);
    double phi = rad_to_deg(asin(coords.y/EARTH_PERIMETER));
    return make_shared<RealCoords>(phi,lambda);
}

MercatorProjection::MercatorProjection() : CylindricalProjection(){}

shared_ptr<CartesianCoords> MercatorProjection::compute_coords(RealCoords &coords) const{
    if(coords.latitude == 0 || coords.latitude == 180){
        return nullptr;
    }
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
    double delta = 90 - coords.latitude;
    double y = EARTH_PERIMETER * log(deg_cot(delta/2));
    return make_shared<CartesianCoords>(x,y);
}

shared_ptr<RealCoords> MercatorProjection::decompute_coords(CartesianCoords &coords) const{
    double lambda = rad_to_deg(coords.x/EARTH_PERIMETER);
    double phi = 90-2*rad_to_deg(atan(1/exp(coords.y/EARTH_PERIMETER)));
    return make_shared<RealCoords>(phi,lambda);
}

PerspectiveProjection::PerspectiveProjection():CylindricalProjection(){}

shared_ptr<CartesianCoords> PerspectiveProjection::compute_coords(RealCoords &coords) const{
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude);
    double y = 2 * EARTH_PERIMETER * deg_tan(coords.latitude/2);
    return make_shared<CartesianCoords>(x,y);
}

shared_ptr<RealCoords> PerspectiveProjection::decompute_coords(CartesianCoords &coords) const{
    double lambda = rad_to_deg(coords.x/EARTH_PERIMETER);
    double phi = 2*rad_to_deg(atan(coords.y/(2*EARTH_PERIMETER)));
    return make_shared<RealCoords>(phi,lambda);
}

BehrmannProjection::BehrmannProjection():CylindricalProjection(){}

shared_ptr<CartesianCoords> BehrmannProjection::compute_coords(RealCoords &coords) const{
    double phi_0 = 30;
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude) * deg_cos(phi_0);
    double y = EARTH_PERIMETER * (deg_sin(coords.latitude)/deg_cos(phi_0));
    return make_shared<CartesianCoords>(x,y);
}

shared_ptr<RealCoords> BehrmannProjection::decompute_coords(CartesianCoords &coords) const{
    double lambda = rad_to_deg(coords.x/(EARTH_PERIMETER*deg_cos(30)));
    double phi = rad_to_deg(asin(coords.y*deg_cos(30)/EARTH_PERIMETER));
    return make_shared<RealCoords>(phi,lambda);
}

TrystanEdwardsProjection::TrystanEdwardsProjection():CylindricalProjection(){}

shared_ptr<CartesianCoords> TrystanEdwardsProjection::compute_coords(RealCoords &coords) const{
    double n = sqrt(2/M_PI);
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude)*n;
    double y = EARTH_PERIMETER * deg_sin(coords.latitude)*(1/n);
    return make_shared<CartesianCoords>(x,y);
}

shared_ptr<RealCoords> TrystanEdwardsProjection::decompute_coords(CartesianCoords &coords) const{
    double n = sqrt(2/M_PI);
    double lambda = rad_to_deg(coords.x/(EARTH_PERIMETER*n));
    double phi = rad_to_deg(asin(n*coords.y/EARTH_PERIMETER));
    return make_shared<RealCoords>(phi,lambda);
}

GallProjection::GallProjection(): CylindricalProjection(){}

shared_ptr<CartesianCoords> GallProjection::compute_coords(RealCoords &coords) const{
    double phi_0 = 45;
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude) * deg_cos(phi_0);
    double y = EARTH_PERIMETER * (1+deg_cos(phi_0))*deg_tan(coords.latitude/2);
    return make_shared<CartesianCoords>(x,y);
}

shared_ptr<RealCoords> GallProjection::decompute_coords(CartesianCoords &coords) const{
    double lambda = rad_to_deg(coords.x/(EARTH_PERIMETER*deg_cos(45)));
    double phi = 2*rad_to_deg(atan(coords.y/(EARTH_PERIMETER*(1+deg_cos(45)))));
    return make_shared<RealCoords>(phi,lambda);
}


shared_ptr<PolarCoords> PtolemyProjection::compute_coords(RealCoords &coords) const{
    double phi_0 = 30;
    double delta_0 = 90 - phi_0;
    double epsilon = coords.longitude * deg_cos(delta_0);
    double rho = EARTH_PERIMETER * (deg_tan(delta_0)+deg_to_rad((90-coords.latitude)-delta_0));
    return make_shared<PolarCoords>(rho,epsilon);
}

shared_ptr<RealCoords> PtolemyProjection::decompute_coords(PolarCoords &coords) const{
    double lambda = 2*coords.epsilon;
    double phi = 90 - 60 - rad_to_deg((coords.rho/EARTH_PERIMETER)-deg_tan(60));
    return make_shared<RealCoords>(phi,lambda);
}

shared_ptr<PolarCoords> LambertConicProjection::compute_coords(RealCoords &coords) const{
    double phi_0 = 30;
    double delta_0 = 90 - phi_0;
    double epsilon = coords.longitude * pow(deg_cos(delta_0/2),2);
    double rho = 2 * EARTH_PERIMETER * (deg_sin((90-coords.latitude)/2)/(deg_cos(delta_0/2)));
    return make_shared<PolarCoords>(rho,epsilon);
}

shared_ptr<RealCoords> LambertConicProjection::decompute_coords(PolarCoords &coords) const{
    double lambda = coords.epsilon/(pow(deg_cos(30),2));
    double phi = 90 - 2 * rad_to_deg(asin(deg_cos(30)*coords.rho/(2*EARTH_PERIMETER)));
    return make_shared<RealCoords>(phi,lambda);
}


shared_ptr<CartesianCoords> SansonProjectionSpecial::compute_coords(RealCoords &coords) const{
    double x = EARTH_PERIMETER * deg_to_rad(coords.longitude) * deg_cos(coords.latitude);
    double y = EARTH_PERIMETER * deg_to_rad(coords.latitude);
    return make_shared<CartesianCoords>(x,y);
}

shared_ptr<RealCoords> SansonProjectionSpecial::decompute_coords(CartesianCoords &coords) const{
    double phi = rad_to_deg(coords.y/EARTH_PERIMETER);
    double lambda = rad_to_deg(coords.x/(EARTH_PERIMETER*deg_cos(phi)));
    return make_shared<RealCoords>(phi,lambda);
}

shared_ptr<PolarCoords> WernerStabProjectionSpecial::compute_coords(RealCoords &coords) const{
    if(coords.latitude == 90) return nullptr;

    double delta = 90 - coords.latitude;
    double epsilon = 360 * deg_sin(delta) / deg_to_rad(delta);
    double rho = EARTH_PERIMETER * deg_to_rad(delta);
    return make_shared<PolarCoords>(rho,epsilon);
}

shared_ptr<RealCoords> WernerStabProjectionSpecial::decompute_coords(PolarCoords &coords) const{
    double phi = 90 - rad_to_deg(coords.rho/EARTH_PERIMETER);
    return make_shared<RealCoords>(phi,0);
}


void ConicProjection::add_point(string &name, double c1, double c2){
    if(points.find(name) == points.end()){
        shared_ptr<PolarCoords> coords = make_shared<PolarCoords>(c1,c2);
        if(coords == nullptr){
            cout << "unable to project this point at the current projections."<<endl;
            return;
        }
        points[name] = coords;
    }
}

shared_ptr<Coords> ConicProjection::find_point(const string &p0) const{
    shared_ptr<PolarCoords> p0_ptr;
    if(p0.find('.') != string::npos){
        auto dot_it = p0.find('.');

        string country = p0.substr(0,dot_it);
        string is_capital = p0.substr(dot_it+1);
        if(is_capital != "capital") return nullptr;

        if(db->countries.find(country) == db->countries.end()) return nullptr;
        auto capital = static_pointer_cast<Region>(db->countries[country])->capital;
        p0_ptr = compute_coords(dynamic_cast<RealCoords &>(*capital->coords));
    }
    else if(points.find(p0) == points.end()){
        if(db->data.find(p0) == Projection::db->data.end()){
            return nullptr;
        }
        else if(db->data[p0]->type == POINT) {
            shared_ptr<Point>  point_p0 = static_pointer_cast<Point>(db->data[p0]);
            p0_ptr = compute_coords(dynamic_cast<RealCoords &>(*point_p0->coords));
        }
        else{
            shared_ptr<Region>  point_p0 = static_pointer_cast<Region>(db->data[p0]);
            p0_ptr = compute_coords(*static_pointer_cast<RealCoords>(point_p0->centroid));
        }
    }
    else{
        p0_ptr = points.at(p0);
    }
    return p0_ptr;
}

double ConicProjection::calculate_distance(const string &p0, const string &p1) const{
    shared_ptr<PolarCoords> p0_ptr = static_pointer_cast<PolarCoords>(find_point(p0));
    shared_ptr<PolarCoords> p1_ptr = static_pointer_cast<PolarCoords>(find_point(p1));
    if(p0_ptr == nullptr || p1_ptr == nullptr){
        cout <<"at least one of the points does not exist."<<endl;
        return 0;
    }

    return get_distance<PolarCoords>(*p0_ptr,*p1_ptr);
}

double ConicProjection::calculate_rectangular_area(const string &name) const{
    shared_ptr<Region> region = find_region(name);
    if(region == nullptr) return 0;

    auto s = static_pointer_cast<PolarCoords>(region->south);
    auto n = static_pointer_cast<PolarCoords>(region->north);
    auto e = static_pointer_cast<PolarCoords>(region->east);
    auto w = static_pointer_cast<PolarCoords>(region->west);

    double result = get_rectangular_area<PolarCoords>(*s,*n,*e,*w);

    return result;
}

void ConicProjection::print_local() const {
    cout << "-------------------------------------------"<<endl;
    cout << points.size()<<" local point(s) saved:"<<endl;
    for(auto &point : points){
        cout <<" "<<point.first<<": "<<point.second->get_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

shared_ptr<RealCoords> ConicProjection::convert_to_real_coords(const string &p) const{
    shared_ptr<PolarCoords> p_ptr = static_pointer_cast<PolarCoords>(find_point(p));
    if(p_ptr == nullptr){
        cout <<"the point does not exist."<<endl;
        return nullptr;
    }
    auto result = decompute_coords(*p_ptr);
    return result;
}

shared_ptr<Region> ConicProjection::find_region(const string &name) const{
    auto region = get_region_from_db(name);
    if(region == nullptr) return nullptr;

    auto polar_s = compute_coords(dynamic_cast<RealCoords &>(*region->south));
    if(polar_s == nullptr){
        cout << "unable to project south in this type of projection."<<endl;
        return nullptr;
    }

    auto polar_n = compute_coords(dynamic_cast<RealCoords &>(*region->north));
    if(polar_n == nullptr){
        cout << "unable to project north in this type of projection."<<endl;
        return nullptr;
    }

    auto polar_e = compute_coords(dynamic_cast<RealCoords &>(*region->east));
    if(polar_e == nullptr){
        cout << "unable to project east in this type of projection."<<endl;
        return nullptr;
    }

    auto polar_w = compute_coords(dynamic_cast<RealCoords &>(*region->west));
    if(polar_w == nullptr){
        cout << "unable to project west in this type of projection."<<endl;
        return nullptr;
    }

    auto s = polar_s->rho;
    auto n = polar_n->rho;
    auto e = polar_e->epsilon;
    auto w = polar_w->epsilon;

    if(!region->capital_name.empty()){
        auto capital = compute_coords(dynamic_cast<RealCoords &>(*region->capital->coords));
        return make_shared<Region>(s,n,e,w,region->capital_name,capital->rho,capital->epsilon,coords_type::POLAR);
    }
    else return make_shared<Region>(s,n,e,w,POLAR);
}

shared_ptr<Database> Projection::db = nullptr;

Projection::Projection() {
    if(!db){
        Database database;
        cout << "Database is being loaded. We can load more than 20 000 cities, but as \n"
                "default, only 100 biggest cities is loaded. Would you like to change the amount\n"
                "of loaded cities [y/n]?"<<endl;
        string answer;
        cin >> answer;
        if(answer != "y"){
            if (answer != "n"){
                cout << "invalid option. The database is loading in default mode."<<endl;
            }
            db_loader::add_initial_data(database);
        }
        else{
            cout <<"type a desired number of cities in the database: ";
            int number;
            cin >> number;
            if(cin.fail()){
                cout << "invalid number. The database is loading in default mode."<<endl;
                db_loader::add_initial_data(database);
            }
            else db_loader::add_initial_data(database,number);
        }
        db = make_shared<Database>(database);
    }
}

shared_ptr<Region> Projection::find_region(const string &name) const{
    shared_ptr<Region> region_ptr;
    if(db->continents.find(name) == db->continents.end()){
        if(db->countries.find(name) == db->countries.end()){
            if(db->lakes.find(name) == db->lakes.end()){
                if(db->custom_regions.find(name) == db->custom_regions.end()){
                    cout <<"Region with name \""<<name<<"\" does not exist."<<endl;
                    return nullptr;
                }
                else region_ptr = static_pointer_cast<Region>(db->custom_regions[name]);
            }
            else region_ptr = static_pointer_cast<Region>(db->lakes[name]);
        }
        else region_ptr = static_pointer_cast<Region>(db->countries[name]);
    }
    else region_ptr = static_pointer_cast<Region>(db->continents[name]);

    return region_ptr;
}

shared_ptr<Region> Projection::get_region_from_db(const string &name) {
    shared_ptr<Region> region_ptr;
    if(db->continents.find(name) == db->continents.end()){
        if(db->countries.find(name) == db->countries.end()){
            if(db->lakes.find(name) == db->lakes.end()){
                if(db->custom_regions.find(name) == db->custom_regions.end()){
                    cout <<"Region with name \""<<name<<"\" does not exist."<<endl;
                    return nullptr;
                }
                else region_ptr = static_pointer_cast<Region>(db->custom_regions[name]);
            }
            else region_ptr = static_pointer_cast<Region>(db->lakes[name]);
        }
        else region_ptr = static_pointer_cast<Region>(db->countries[name]);
    }
    else region_ptr = static_pointer_cast<Region>(db->continents[name]);

    return region_ptr;
}

RealProjection::RealProjection() : Projection(){

}

shared_ptr<RealCoords> RealProjection::compute_coords(RealCoords &coords) {
    return make_shared<RealCoords>(coords);
}

shared_ptr<Coords> RealProjection::find_point(const string &p0) const{
    shared_ptr<RealCoords> p0_ptr;
    if(p0.find('.') != string::npos){
        auto dot_it = p0.find('.');

        string country = p0.substr(0,dot_it);
        string is_capital = p0.substr(dot_it+1);
        if(is_capital != "capital") return nullptr;

        if(db->countries.find(country) == db->countries.end()) return nullptr;
        auto capital = static_pointer_cast<Region>(db->countries[country])->capital;
        p0_ptr = compute_coords(dynamic_cast<RealCoords &>(*capital->coords));
    }
    else if(db->data.find(p0) == db->data.end()){
        return nullptr;
    }
    else if(db->data[p0]->type == POINT) {
        shared_ptr<Point>  point_p0 = static_pointer_cast<Point>(db->data[p0]);
        p0_ptr = compute_coords(dynamic_cast<RealCoords &>(*point_p0->coords));
    }
    else{
        shared_ptr<Region>  point_p0 = static_pointer_cast<Region>(db->data[p0]);
        p0_ptr = compute_coords(*static_pointer_cast<RealCoords>(point_p0->centroid));
    }

    return p0_ptr;
}

double RealProjection::calculate_distance(const string &p0, const string &p1) const{
    shared_ptr<RealCoords> p0_ptr = static_pointer_cast<RealCoords>(find_point(p0));
    shared_ptr<RealCoords> p1_ptr = static_pointer_cast<RealCoords>(find_point(p1));
    if(p0_ptr == nullptr || p1_ptr == nullptr){
        cout <<"at least one of the points does not exist."<<endl;
        return 0;
    }

    return get_distance<RealCoords>(*p0_ptr,*p1_ptr);
}

double RealProjection::calculate_rectangular_area(const string &name) const{
    shared_ptr<Region> region = find_region(name);
    if(region == nullptr) return 0;

    auto s = static_pointer_cast<RealCoords>(region->south);
    auto n = static_pointer_cast<RealCoords>(region->north);
    auto e = static_pointer_cast<RealCoords>(region->east);
    auto w = static_pointer_cast<RealCoords>(region->west);

    double result = get_rectangular_area<RealCoords>(*s,*n,*e,*w);

    return result;
}

void RealProjection::print_local() const {
    cout <<"There are no local points in this type of projection. Everything is automatically moved to the main database."<<endl;
}

shared_ptr<RealCoords> RealProjection::convert_to_real_coords(const string &p) const{
    return Projection::convert_to_real_coords(p);
}
