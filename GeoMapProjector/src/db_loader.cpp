//
// Created by Ja on 04.03.2021.
//

#include "../include/database.hpp"
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>

class db_loader{
public:

    db_loader(){};

    ~db_loader(){}

    double get_coordinate(istream &is){
        string num_string;
        string hemisphere;

        getline(is,num_string,';');
        getline(is,hemisphere,';');
        double coord = stod(num_string);
        if(hemisphere == "S" || hemisphere == "W") coord *= -1;

        return coord;
    }

    void add_countries(Database &db){
        ifstream is("../extreme_points_on_earth_countries.csv");

        if(is.fail()){
            cout<<"error: unable to add countries to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        while(getline(is,line,'\n')){
            stringstream ss(line);

            string state, capital;

            getline(ss,state,';');
            double s = get_coordinate(ss);
            double n = get_coordinate(ss);
            double e = get_coordinate(ss);
            double w = get_coordinate(ss);

            getline(ss,capital,';');

            double c_lat = get_coordinate(ss);
            double c_lon = get_coordinate(ss);

            db.add_data(state, s, n, e, w, capital, c_lat, c_lon);
        }
        is.close();
    }

    void add_lakes(Database &db){
        ifstream is("../extreme_points_on_earth_lakes.csv");

        if(is.fail()){
            cout<<"error: unable to add lakes to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        while(getline(is,line,'\n')){
            stringstream ss(line);

            string lake;

            getline(ss,lake,';');
            double s = get_coordinate(ss);
            double n = get_coordinate(ss);
            double e = get_coordinate(ss);
            double w = get_coordinate(ss);

            db.add_data(lake, s, n, e, w,object_type::LAKE);
        }
        is.close();
    }

    void add_mountains(Database &db){
        ifstream is("../extreme_points_on_earth_mountains.csv");

        if(is.fail()){
            cout<<"error: unable to add  mountains to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        while(getline(is,line,'\n')){
            stringstream ss(line);

            string mount;

            getline(ss,mount,';');
            double lat = get_coordinate(ss);
            double lon = get_coordinate(ss);

            db.add_data(mount,lat,lon,coords_type::REAL,object_type::MOUNTAIN);

        }
        is.close();
    }

    void add_cities(Database &db,int number = 100){
        ifstream is("../worldcities_mega_db.csv");

        if(is.fail()){
            cout<<"error: unable to add cities to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        int iteration = 0;
        while(iteration < number){
            getline(is,line,'\n');
            stringstream ss(line);

            string city, nothing;

            getline(ss,nothing,';');

            getline(ss,city,';');

            string lat_string, lon_string;
            getline(ss,lat_string,';');
            getline(ss,lon_string,';');

            double lat = stod(lat_string);
            double lon = stod(lon_string);

            db.add_data(city,lat,lon,coords_type::REAL,object_type::CITY);
            iteration++;
        }
        is.close();
    }

    void add_continents(Database &db){
        ifstream is("../extreme_points_on_earth_continents.csv");

        if(is.fail()){
            cout<<"error: unable to add continents to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        while(getline(is,line,'\n')){
            stringstream  ss(line);

            string cont;

            getline(ss,cont,';');
            double s = get_coordinate(ss);
            double n = get_coordinate(ss);
            double e = get_coordinate(ss);
            double w = get_coordinate(ss);

            db.add_data(cont, s, n, e, w,object_type::CONTINENT);
        }
        is.close();
    }


    void add_initial_data(Database &db){
        add_continents(db);
        add_countries(db);
        add_lakes(db);
        add_mountains(db);
        add_cities(db);
    }
};