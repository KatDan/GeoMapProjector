//
// Created by Ja on 28.02.2021.
//
#include "../include/database.hpp"

Data::Data(enum data_type type):type{type}{}

Point::Point(double c1, double c2, enum coords_type type = REAL):Data(data_type::POINT){
    coords = make_shared<RealCoords>(c1,c2);
}

string Point::print_coords() const{
    return to_string(coords->coords_pair.first)+" "+to_string(coords->coords_pair.second);
}

Region::Region(double s, double n, double e, double w):Data(data_type::REGION),
  south{make_shared<RealCoords>(s,lat_or_long::LATITUDE)},north{make_shared<RealCoords>(n,lat_or_long::LATITUDE)},
    east{make_shared<RealCoords>(e,lat_or_long::LONGITUDE)},west{make_shared<RealCoords>(w,lat_or_long::LONGITUDE)}{

    double latitude_difference = abs(s-n);
    double longitude_difference = abs(e-w);

    centroid = make_shared<RealCoords>(min(s,n)+latitude_difference,min(e,w)+longitude_difference);
}

Region::Region(double s, double n, double e, double w, string &capital_name, double c1, double c2):Data(data_type::COUNTRY),
  south{make_shared<RealCoords>(s,lat_or_long::LATITUDE)},north{make_shared<RealCoords>(n,lat_or_long::LATITUDE)},
  east{make_shared<RealCoords>(e,lat_or_long::LONGITUDE)},west{make_shared<RealCoords>(w,lat_or_long::LONGITUDE)},capital_name{capital_name},
  capital{make_shared<Point>(c1,c2,coords_type::REAL)}{

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

void Database::add_data(string &name, double c1, double c2, coords_type type, enum object_type obj_type){
    auto p = make_shared<Point>(c1,c2,type);
    auto p_dup = p;
    data[name] = p_dup;
    if(obj_type == CUSTOM) custom_points[name] = p;
    else if(obj_type == CITY) cities[name] = p;
    else if(obj_type == MOUNTAIN) mountains[name] = p;
}

void Database::add_data(string &name, double s, double n, double e, double w, enum object_type obj_type){
    auto region_ptr = make_shared<Region>(s,n,e,w);
    auto another_ptr = region_ptr;
    data[name] = another_ptr;
    if(obj_type == LAKE) lakes[name] = region_ptr;
    else if(obj_type == CONTINENT) continents[name] = region_ptr;
    else if (obj_type == CUSTOM) custom_regions[name] = region_ptr;
}

void Database::add_data(string &name, double s, double n, double e, double w,string &capital_name, double c1, double c2){
    auto country_ptr = make_shared<Region>(s,n,e,w,capital_name,c1,c2);
    auto c_ptr = country_ptr;
    data[name] = c_ptr;
    countries[name] = country_ptr;
}

shared_ptr<Data> Database::get_data(string &name){
    if(data.find(name) == data.end()){
        cout << "\""<<name<<"\" is not in the database."<<endl;
        return nullptr;
    }
    return data[name];
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
        cout << " "<<city.first<<": "<< city.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

void Database::print_mountains() const{
    cout << "-------------------------------------------"<<endl;
    cout << mountains.size()<<" mountains saved:"<<endl;
    for(auto &mountain : mountains){
        cout <<" "<<mountain.first<<": "<< mountain.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

void Database::print_lakes() const{
    cout << "-------------------------------------------"<<endl;
    cout << lakes.size()<<" lakes saved: "<<endl;
    for(auto &lake : lakes){
        cout << " "<<lake.first<<": "<< lake.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

void Database::print_continents() const{
    cout << "-------------------------------------------"<<endl;
    cout << continents.size()<<" continents saved: "<<endl;
    for(auto &cont : continents){
        cout << " "<<cont.first<<": "<< cont.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}

void Database::print_custom() const{
    cout << "-------------------------------------------"<<endl;
    cout << custom_points.size()+custom_regions.size()<<" custom data saved:"<<endl;
    for(auto &cust : custom_points){
        cout << " "<<cust.first<<": "<< cust.second->print_coords()<<endl;
    }
    for(auto &cust : custom_regions){
        cout << " "<<cust.first<<": "<< cust.second->print_coords()<<endl;
    }
    cout << "-------------------------------------------"<<endl;
}



