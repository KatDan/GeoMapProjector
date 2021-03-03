//
// Created by Ja on 28.02.2021.
//

#include<utility>
#include<memory>
#include<cmath>
#include<vector>
#include<map>

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
};


class Database{
public:
    map<string,shared_ptr<Data>> data;
    Database(){}

    void add_data(string &name, double c1, double c2, coords_type type){
        data[name] = make_shared<Point>(c1,c2, type);
    }

    void add_data(string &name, double s, double n, double e, double w){
        data[name] = make_shared<Region>(s,n,e,w);
    }

    void add_data(string &name, double s, double n, double e, double w,string &capital_name, double c1, double c2){
        data[name] = make_shared<Region>(s,n,e,w,capital_name,c1,c2);
    }

    shared_ptr<Data> get_data(string &name){
        if(data.find(name) == data.end()){
            cout << "\""<<name<<"\" is not in the database."<<endl;
            return nullptr;
        }
        return data[name];
    }

};
