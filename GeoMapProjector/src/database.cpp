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

    Data(enum data_type type):type{type}{}

    virtual string print_coords() const {}
};

class Point : public Data{
public:
    shared_ptr<Coords> coords;

    Point(double c1, double c2, enum coords_type type):Data(data_type::POINT){
        if(type == coords_type::POLAR){
            coords = make_shared<PolarCoords>(c1,c2);
        }
        else if(type == coords_type::CARTESIAN){
            coords = make_shared<CartesianCoords>(c1,c2);
        }
        else{
            coords = make_shared<RealCoords>(c1,c2);
        }
    }

    string print_coords() const{
        return to_string(coords->coords_pair.first)+" "+to_string(coords->coords_pair.second);
    }
};

class Region : public Data{
public:
    double south;
    double north;
    double east;
    double west;

    shared_ptr<Coords> centroid;

    string capital_name;
    shared_ptr<Point> capital;

    Region(double s, double n, double e, double w):Data(data_type::REGION),
      south{s},north{n},east{e},west{w}{}

    Region(double s, double n, double e, double w, string &capital_name, double c1, double c2):Data(data_type::COUNTRY),
      south{s},north{n},east{e},west{w},capital_name{capital_name},
      capital{make_shared<Point>(c1,c2,coords_type::REAL)}{

        double latitude_difference = fdim(s,n);
        double longitude_difference = fdim(e,w);

        centroid = make_shared<RealCoords>(min(s,n)+latitude_difference,min(e,w)+longitude_difference);
    }

    string print_coords() const{
        if(type == data_type::REGION){
            return "S: "+to_string(south)+"°, N: "+to_string(north)+"°, E: "+to_string(east)+"°, W: "+to_string(west)+"°";
        }
        else{
            return "S: "+to_string(south)+"°, N: "+to_string(north)+"°, E: "+to_string(east)+"°, W: "+to_string(west)+"°"+
              "\n    Capital is "+capital_name+": "+capital->print_coords();
        }

    }
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
    map<string, shared_ptr<Data>> custom;
    map<string, shared_ptr<Data>> continents;

    Database(){}

    void add_data(string &name, double c1, double c2, coords_type type, enum object_type obj_type){
        auto p = make_shared<Point>(c1,c2,type);
        data[name] = p;
        if(obj_type == CUSTOM) custom[name] = p;
        else if(obj_type == CITY) cities[name] = p;
        else if(obj_type == MOUNTAIN) mountains[name] = p;
    }

    void add_data(string &name, double s, double n, double e, double w, enum object_type obj_type){
        auto region_ptr = make_shared<Region>(s,n,e,w);
        data[name] = region_ptr;
        if(obj_type == LAKE) lakes[name] = region_ptr;
        else if(obj_type == CONTINENT) continents[name] = region_ptr;
        else if (obj_type == CUSTOM) custom[name] = region_ptr;
    }

    void add_data(string &name, double s, double n, double e, double w,string &capital_name, double c1, double c2){
        auto country_ptr = make_shared<Region>(s,n,e,w,capital_name,c1,c2);
        data[name] = country_ptr;
        countries[name] = country_ptr;
    }

    shared_ptr<Data> get_data(string &name){
        if(data.find(name) == data.end()){
            cout << "\""<<name<<"\" is not in the database."<<endl;
            return nullptr;
        }
        return data[name];
    }

    void print_countries() const{
        cout << countries.size() << " countries saved:"<<endl;
        for(auto &country : countries){
            cout << " "<<country.first<<": "<<country.second->print_coords()<<endl;
        }
    }

    void print_cities() const{
        cout << cities.size() << " cities saved: "<<endl;
        for(auto &city : cities){
            cout << " "<<city.first<<": "<< city.second->print_coords()<<endl;
        }
    }

    void print_mountains() const{
        cout << mountains.size()<<" mountains saved:"<<endl;
        for(auto &mountain : mountains){
            cout << " "<<mountain.first<<": "<< mountain.second->print_coords()<<endl;
        }
    }

    void print_lakes() const{
        cout << lakes.size()<<" lakes saved: "<<endl;
        for(auto &lake : lakes){
            cout << " "<<lake.first<<": "<< lake.second->print_coords()<<endl;
        }
    }

    void print_continents() const{
        cout << continents.size()<<" continents saved: "<<endl;
        for(auto &cont : continents){
            cout << " "<<cont.first<<": "<< cont.second->print_coords()<<endl;
        }
    }

    void print_custom() const{
        cout << custom.size()<<" custom data saved:"<<endl;
        for(auto &cust : custom){
            cout << " "<<cust.first<<": "<< cust.second->print_coords()<<endl;
        }
    }

};
