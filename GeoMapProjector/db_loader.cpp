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

    void add_countries(){
        ifstream is("extreme_points_on_earth_countries.csv");

        if(is.fail()){
            cout<<"error: unable to add countries to database"<<endl;
            return;
        }

        string line;
        getline(is,line,'\n');
        while(getline(is,line,'\n')){
            string state, s, s_hem, n, n_hem, e, e_hem, w, w_hem, capital, c_lat, c_lat_hem, c_lon, c_lon_hem;

            getline(is,state,';');
            getline(is,s,';');
            getline(is,s_hem,';');
            getline(is,n,';');
            getline(is,n_hem,';');
            getline(is,e,';');
            getline(is,e_hem,';');
            getline(is,w,';');
            getline(is,w_hem,';');
            getline(is,capital,';');
            getline(is,c_lat,';');
            getline(is,c_lat_hem,';');
            getline(is,c_lon,';');
            getline(is,c_lon_hem,';');

            double s_num = stod(s);
            if(s_hem == "S") s_num *= -1;

            double n_num = stod(n);
            if(n_hem == "S") n_num *= -1;

            double e_num = stod(e);
            if(e_hem == "W") e_num *= -1;

            double w_num = stod(w);
            if(w_hem == "W") w_num *= -1;

            double c_lat_num = stod(c_lat);
            if(c_lat_hem == "S") c_lat_num *= -1;

            double c_lon_num = stod(c_lon);
            if(c_lat_hem == "W") c_lat_num *= -1;

            db.add_data(state, s_num,n_num,e_num,w_num,capital,c_lat_num,c_lon_num);
        }
        is.close();
    }


    void add_initial_data(){

    }
};
