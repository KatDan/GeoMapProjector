//
// Created by Ja on 04.03.2021.
//

#include "database.cpp"
#include<string>
#include<fstream>
#include<iostream>

class db_loader{
public:
    Database db;

    db_loader();

    double get_coordinate(istream &is){
        string num_string;
        string hemisphere;

        getline(is,num_string,';');
        getline(is,hemisphere,';');
        double coord = stod(num_string);
        if(hemisphere == "S" || hemisphere == "W") coord *= -1;

        return coord;
    }

    void add_countries(){
        ifstream is("extreme_points_on_earth_countries.csv");

        if(is.fail()){
            cout<<"error: unable to add countries to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        while(getline(is,line,'\n')){
            string state, capital;

            getline(is,state,';');
            double s = get_coordinate(is);
            double n = get_coordinate(is);
            double e = get_coordinate(is);
            double w = get_coordinate(is);

            getline(is,capital,';');

            double c_lat = get_coordinate(is);
            double c_lon = get_coordinate(is);

            db.add_data(state, s, n, e, w, capital, c_lat, c_lon);
        }
        is.close();
    }

    void add_lakes(){
        ifstream is("extreme_points_on_earth_lakes.csv");

        if(is.fail()){
            cout<<"error: unable to add lakes to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        while(getline(is,line,'\n')){
            string lake;

            getline(is,lake,';');
            double s = get_coordinate(is);
            double n = get_coordinate(is);
            double e = get_coordinate(is);
            double w = get_coordinate(is);

            db.add_data(lake, s, n, e, w,object_type::LAKE);
        }
        is.close();
    }

    void add_mountains(){
        ifstream is("extreme_points_on_earth_mountains.csv");

        if(is.fail()){
            cout<<"error: unable to add  mountains to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        while(getline(is,line,'\n')){
            string mount;

            getline(is,mount,';');
            double lat = get_coordinate(is);
            double lon = get_coordinate(is);

            db.add_data(mount,lat,lon,coords_type::REAL,object_type::MOUNTAIN);
        }
        is.close();
    }

    void add_cities(int number = 100){
        ifstream is("worldcities_mega_db.csv");

        if(is.fail()){
            cout<<"error: unable to add cities to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        while(getline(is,line,'\n')){
            string city, nothing;
            getline(is,city,';');

            getline(is,nothing,';');

            string lat_string, lon_string;
            getline(is,lat_string,';');
            getline(is,lon_string,';');

            double lat = stod(lat_string);
            double lon = stod(lon_string);

            db.add_data(city,lat,lon,coords_type::REAL,object_type::CITY);
        }
        is.close();
    }

    void add_continents(){
        ifstream is("extreme_points_on_earth_continents.csv");

        if(is.fail()){
            cout<<"error: unable to add continents to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        while(getline(is,line,'\n')){
            string cont;

            getline(is,cont,';');
            double s = get_coordinate(is);
            double n = get_coordinate(is);
            double e = get_coordinate(is);
            double w = get_coordinate(is);

            db.add_data(cont, s, n, e, w,object_type::CONTINENT);
        }
        is.close();
    }


    void add_initial_data(){
        add_continents();
        add_countries();
        add_lakes();
        add_mountains();
        add_cities();
    }
};