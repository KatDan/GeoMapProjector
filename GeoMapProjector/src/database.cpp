//
// Created by Ja on 28.02.2021.
//
#include "../include/database.hpp"

#include <utility>

Data::Data(enum data_type type):type{type}{}

Point::Point(double c1, double c2):Data(data_type::POINT){
    coords = make_shared<RealCoords>(c1,c2);
}

string Point::print_coords() const{
    return coords->get_coords();
}

Region::Region(double s, double n, double e, double w):Data(data_type::REGION),
  south{make_shared<RealCoords>(s,lat_or_long::LATITUDE)},north{make_shared<RealCoords>(n,lat_or_long::LATITUDE)},
    east{make_shared<RealCoords>(e,lat_or_long::LONGITUDE)},west{make_shared<RealCoords>(w,lat_or_long::LONGITUDE)}{

    double latitude_difference = abs(s-n);
    double longitude_difference = abs(e-w);

    centroid = make_shared<RealCoords>(min(s,n)+latitude_difference,min(e,w)+longitude_difference);
}

Region::Region(double s, double n, double e, double w, string capital_name, double c1, double c2):Data(data_type::COUNTRY),
  south{make_shared<RealCoords>(s,lat_or_long::LATITUDE)},north{make_shared<RealCoords>(n,lat_or_long::LATITUDE)},
  east{make_shared<RealCoords>(e,lat_or_long::LONGITUDE)},west{make_shared<RealCoords>(w,lat_or_long::LONGITUDE)},capital_name{std::move(capital_name)},
  capital{make_shared<Point>(c1,c2)}{

    double latitude_difference = abs(s-n);
    double longitude_difference = abs(e-w);

    centroid = make_shared<RealCoords>(min(s,n)+latitude_difference,min(e,w)+longitude_difference);
}


string Region::print_coords() const{
    if(type == data_type::REGION){
        return "S: "+to_string(south->get_singleton_value())+", N: "+to_string(north->get_singleton_value())+", E: "
          +to_string(east->get_singleton_value())+", W: "+to_string(west->get_singleton_value());
    }
    else{
        return "S: "+to_string(south->get_singleton_value())+", N: "+to_string(north->get_singleton_value())+", E: "
          +to_string(east->get_singleton_value())+", W: "+to_string(west->get_singleton_value())+
          "\n    Capital is "+capital_name+": "+capital->print_coords();
    }

}

Region::Region(double s, double n, double e, double w, coords_type type) : Data(data_type::REGION) {
    double latitude_difference = abs(s-n);
    double longitude_difference = abs(e-w);

    if(type == coords_type::POLAR){
        south = make_shared<PolarCoords>(s,LATITUDE);
        north = make_shared<PolarCoords>(n,LATITUDE);
        east = make_shared<PolarCoords>(e, LONGITUDE);
        west = make_shared<PolarCoords>(w, LONGITUDE);

        centroid = make_shared<PolarCoords>(min(s,n)+latitude_difference,min(e,w)+longitude_difference);
    }
    else if(type == coords_type::CARTESIAN){
        south = make_shared<CartesianCoords>(s,LATITUDE);
        north = make_shared<CartesianCoords>(n,LATITUDE);
        east = make_shared<CartesianCoords>(e, LONGITUDE);
        west = make_shared<CartesianCoords>(w, LONGITUDE);

        centroid = make_shared<CartesianCoords>(min(s,n)+latitude_difference,min(e,w)+longitude_difference);
    }
    else{
        south = make_shared<RealCoords>(s,LATITUDE);
        north = make_shared<RealCoords>(n,LATITUDE);
        east = make_shared<RealCoords>(e, LONGITUDE);
        west = make_shared<RealCoords>(w, LONGITUDE);

        centroid = make_shared<RealCoords>(min(s,n)+latitude_difference,min(e,w)+longitude_difference);
    }
}

void Database::add_data(const string &name, double c1, double c2, enum object_type obj_type){
    auto p = make_shared<Point>(c1,c2);
    data[name] = p;
    if(obj_type == CUSTOM) custom_points[name] = p;
    else if(obj_type == CITY) cities[name] = p;
    else if(obj_type == MOUNTAIN) mountains[name] = p;
}

void Database::add_data(const string &name, double s, double n, double e, double w, enum object_type obj_type){
    auto region_ptr = make_shared<Region>(s,n,e,w);
    data[name] = region_ptr;
    if(obj_type == LAKE) lakes[name] = region_ptr;
    else if(obj_type == CONTINENT) continents[name] = region_ptr;
    else if (obj_type == CUSTOM) custom_regions[name] = region_ptr;
}

void Database::add_data(const string &name, double s, double n, double e, double w,const string &capital_name, double c1, double c2){
    auto country_ptr = make_shared<Region>(s,n,e,w,capital_name,c1,c2);
    data[name] = country_ptr;
    countries[name] = country_ptr;
}

void Database::print_countries() const{
    cout << "-------------------------------------------"<<endl;
    cout << countries.size() << " countries saved:"<<endl;
    for(auto &country : countries){
        cout << " "<<country.first<<": "<<country.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

void Database::print_cities() const{
    cout << "-------------------------------------------"<<endl;
    cout << cities.size() << " cities saved: "<<endl;
    for(auto &city : cities){
        if(city.first.size() > 12) cout << " "<<city.first<<": \t"<< city.second->print_coords()<<endl;
        else if(city.first.size() < 5) cout << " "<<city.first<<": \t\t\t"<< city.second->print_coords()<<endl;
        else cout << " "<<city.first<<": \t\t"<< city.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

void Database::print_mountains() const{
    cout << "-------------------------------------------"<<endl;
    cout << mountains.size()<<" mountains saved:"<<endl;
    for(auto &mountain : mountains){
        if(mountain.first.size() > 12) cout <<" "<<mountain.first<<": \t"<< mountain.second->print_coords()<<endl;
        else if(mountain.first.size() < 5) cout <<" "<<mountain.first<<": \t\t\t"<< mountain.second->print_coords()<<endl;
        else cout <<" "<<mountain.first<<": \t\t"<< mountain.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

void Database::print_lakes() const{
    cout << "-------------------------------------------"<<endl;
    cout << lakes.size()<<" lakes saved: "<<endl;
    for(auto &lake : lakes){
        if(lake.first.size() > 12) cout << " "<<lake.first<<": \t"<< lake.second->print_coords()<<endl;
        else if(lake.first.size() < 5) cout << " "<<lake.first<<": \t\t\t"<< lake.second->print_coords()<<endl;
        else cout << " "<<lake.first<<": \t\t"<< lake.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

void Database::print_continents() const{
    cout << "-------------------------------------------"<<endl;
    cout << continents.size()<<" continents saved: "<<endl;
    for(auto &cont : continents){
        if(cont.first.size() < 5) cout << " "<<cont.first<<": \t\t"<< cont.second->print_coords()<<endl;
        else cout << " "<<cont.first<<": \t"<< cont.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

void Database::print_custom() const{
    cout << "-------------------------------------------"<<endl;
    cout << custom_points.size()+custom_regions.size()<<" custom data saved:"<<endl;
    for(auto &cust : custom_points){
        if(cust.first.size() > 12 ) cout << " "<<cust.first<<": \t"<< cust.second->print_coords()<<endl;
        else if(cust.first.size() < 5) cout << " "<<cust.first<<": \t\t\t"<< cust.second->print_coords()<<endl;
        else cout << " "<<cust.first<<": \t\t"<< cust.second->print_coords()<<endl;
    }
    for(auto &cust : custom_regions){
        if(cust.first.size() > 12 ) cout << " "<<cust.first<<": \t"<< cust.second->print_coords()<<endl;
        else if(cust.first.size() < 5) cout << " "<<cust.first<<": \t\t\t"<< cust.second->print_coords()<<endl;
        else cout << " "<<cust.first<<": \t\t"<< cust.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}



