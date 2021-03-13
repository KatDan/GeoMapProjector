//
// Created by Ja on 01.03.2021.
//
#include "../include/projections.hpp"

#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>

#define AZIMUTH_PROJ_INDEX 0
#define CYLINDRICAL_PROJ_INDEX 1
#define CONIC_PROJ_INDEX 2


using namespace std;

class Controller{
public:
    map<string, pair<string,shared_ptr<Projection>>> existing_projections;

    shared_ptr<Projection> real_projection;

    pair<string,shared_ptr<Projection>> current_projection;

    map<string, map<string, shared_ptr<Projection>>> projections;

    string general_help;
    string azimuthal_help;
    string cylindrical_help;
    string conic_help;
    string hybrid_help;

    Controller(){
        real_projection = make_shared<RealProjection>();
        current_projection.first = "real";
        current_projection.second = real_projection;

        projections["azimuthal"]["gnomonic"] = make_shared<GnomonicProjection>();
        projections["azimuthal"]["stereographic"] = make_shared<StereoGraphicProjection>();
        projections["azimuthal"]["orthographic"] = make_shared<OrthographicProjection>();
        projections["azimuthal"]["postel"] = make_shared<PostelProjection>();
        projections["azimuthal"]["lambert"] = make_shared<LambertAzimuthalProjection>();

        projections["cylindrical"]["equirectangular"] = make_shared<EquirectangularProjection>();
        projections["cylindrical"]["lambert"] = make_shared<LambertCylindricalProjection>();
        projections["cylindrical"]["mercator"] = make_shared<MercatorProjection>();
        projections["cylindrical"]["perspective"] = make_shared<PerspectiveProjection>();
        projections["cylindrical"]["behrmann"] = make_shared<BehrmannProjection>();
        projections["cylindrical"]["trystan-edwards"] = make_shared<TrystanEdwardsProjection>();
        projections["cylindrical"]["gall"] = make_shared<GallProjection>();

        projections["conic"]["ptolemy"] = make_shared<PtolemyProjection>();
        projections["conic"]["lambert"] = make_shared<LambertConicProjection>();

        projections["hybrid"]["sanson"] = make_shared<SansonProjectionSpecial>();
        projections["hybrid"]["werner-stab"] = make_shared<WernerStabProjectionSpecial>();

        general_help = "There are 3 basic projections to choose from:\n"
                                    " -> Azimuthal projection - The mapping of radial lines can be visualized by imagining\n"
                                    "                           a plane tangent to the Earth, with the central point as \n"
                                    "                           tangent point. This projection uses polar coordinates starting \n"
                                    "                           at the north pole. The directions from it are preserved. \n"
                                    "                           For more info type \"help azimuthal\".\n"
                                    " -> Cylindrical projection - The mapping of meridians to vertical lines can be \n"
                                    "                             visualized by imagining a cylinder whose axis coincides\n"
                                    "                             with the Earth's axis of rotation. This cylinder is \n"
                                    "                             wrapped around the Earth, projected onto, and then unrolled.\n"
                                    "                             Cylindrical projections stretch distances east-west and \n"
                                    "                             use cartesian coordinates. These projections have a low\n"
                                    "                             distortion around the equator and a high distortion around\n"
                                    "                             the poles. For more info type\n"
                                    "                             \"help cylindrical\".\n"
                                    " -> Conic projection - One or two standard parallels are defined (30 degrees in our case).\n"
                                    "                       They may be visualized as secant lines where the cone intersects the globe,\n"
                                    "                       or, as the tangent line where the cone is tangent to the globe. The \n"
                                    "                       resulting conic map has low distortion in scale, shape, and area near those \n"
                                    "                       standard parallels. For more info type \"help conic\".\n"
                                    " + Hybrid projections - projection plane is an affine transformation of some of the\n"
                                    "                         projections mentioned above. They may have both polar and\n"
                                    "                         cartesian coordinate system. For more info type \"help hybrid\"\n"
                                    " + basic latitude and longitude coordinates of a globe.";


        azimuthal_help = "Azimuthal projections:"
                                      " -> Gnomonic - cannot project the equator\n"
                                      " -> Stereographic - cannot project the south pole\n"
                                      " -> Orthographic - can show only one hemisphere\n"
                                      " -> Postel\n"
                                      " -> Lambert - usually used only for one hemisphere";


        cylindrical_help = "Cylindrical projections (cannot project poles):\n"
                                        "  1) one standard parallel:\n"
                                        "     -> Equirectangular\n"
                                        "     -> Lambert\n"
                                        "     -> Mercator\n"
                                        "     -> Perspective\n"
                                        "  2) two standard parallels:\n"
                                        "     -> Behrmann\n"
                                        "     -> Trystan Edwards\n"
                                        "     -> Gall";


        conic_help = "Conic projections:\n"
                                  " -> Ptolemy\n"
                                  " -> Lambert";


        hybrid_help = "Hybrid projections:\n"
                                   " -> Sanson - the projection looks like an onion\n"
                                   " -> Werner-Stab - the projection look like a heart";
    }

    void print_projections_help() const{
        cout << general_help <<endl;
    }

    void print_help_azimuth() const {
        cout << azimuthal_help <<endl;
    }

    void print_help_cylindrical() const{
        cout << cylindrical_help << endl;
    }

    void print_help_conic() const{
        cout << conic_help <<endl;
    }

    void print_help_hybrid() const{
        cout << hybrid_help <<endl;
    }

    void print_saved_projections(){
        cout << "you have "<<existing_projections.size()<<" saved projections:"<<endl;
        for(auto &proj : existing_projections){
            cout << " name:"<<proj.first<<", type: "<<proj.second.first<<endl;
        }
    }

    void help_cmd(stringstream &ss){
        string word;
        ss >> word;

        if(word == "projections") print_projections_help();
        else if(word.compare("azimuthal") == 0) print_help_azimuth();
        else if(word == "cylindrical") print_help_cylindrical();
        else if(word == "conic") print_help_conic();
        else if(word == "hybrid") print_help_hybrid();
    }

    void exit_cmd() const{
        cout <<"sea you next time!"<<endl;
    }

    void make_cmd(stringstream &ss){
        string proj_type;
        string proj_subtype;
        string alias;

        ss >> proj_type;

        auto main_proj_it = projections.find(proj_type);
        if(main_proj_it == projections.end()){
            cout << "this type of projection does not exist"<<endl;
            return;
        }

        ss >> proj_subtype;
        auto subtype_it = (*main_proj_it).second.find(proj_subtype);

        if(subtype_it == (*main_proj_it).second.end()){
            cout << "this type of projection does not exist"<<endl;
            return;
        }

        ss >> alias;
        if(alias == ""){
            cout << "invalid name."<<endl;
            return;
        }
        existing_projections[alias] = make_pair(proj_type+" "+proj_subtype,(*subtype_it).second);
        cout <<proj_type<<" "<<proj_subtype <<" projection called \""<<alias<<"\" was added to your list of projections."<<endl;
    }

    void enter_cmd(stringstream &ss){
        string alias;
        ss >> alias;
        auto alias_it = existing_projections.find(alias);
        if(alias_it == existing_projections.end()){
            cout << "There is none projection saved under this name."<<endl;
            return;
        }
        current_projection.first = alias;
        current_projection.second = existing_projections[alias].second;
    }

    void add_cmd(stringstream &ss){
        string type;
        ss >>type;
        if(type == "point"){
            string coord_type;
            ss >> coord_type;
            if(coord_type == "real"){
                string name;
                double lat, lon;
                ss >> name;
                ss >> lat;
                ss >> lon;
                if(ss.fail()){
                    cout <<"invalid number."<<endl;
                    return;
                }
                Projection::db->add_data(name,lat,lon,coords_type::REAL,object_type::CUSTOM);
                cout <<"point \""<<name<<"\" was successfully added to the main database."<<endl;
                return;
            }
            else if(coord_type == "local"){
                if(current_projection.second == real_projection){
                    cout << "error: you currently are not in any projection."<<endl;
                    return;
                }
                string name;
                ss >>name;
                double relative_x, relative_y;
                ss >> relative_x;
                ss >> relative_y;
                if(ss.fail()){
                    cout <<"invalid number."<<endl;
                    return;
                }
                current_projection.second->add_point(name,relative_x,relative_y);
                cout <<"point \""<<name<<"\" was successfully added to the local database."<<endl;
                return;
            }
            else{
                cout <<"invalid command."<<endl;
                return;
            }
        }
        else if(type == "region"){
            string name;
            double s,n,e,w;
            ss >> name;
            ss >>s;
            ss >> n;
            ss >> e;
            ss >> w;
            if(ss.fail()){
                cout << "invalid number."<<endl;
                return;
            }
            Projection::db->add_data(name,s,n,e,w,object_type::CUSTOM);
            cout <<"region \""<<name<<"\" was successfully added to the main database."<<endl;
            return;
        }
        else{
            cout << "invalid command."<<endl;
        }
    }

    void print_cmd(stringstream &ss){
        string what;
        ss >> what;

        if(what == "local"){
            if(current_projection.second == real_projection){
                cout << "we are not in any projection."<<endl;
                return;
            }
            current_projection.second->print_local();
        }
        else if(what == "projections"){
            print_saved_projections();
        }
        else if(what == "countries") Projection::db->print_countries();
        else if(what == "cities") Projection::db->print_cities();
        else if(what == "mountains") Projection::db->print_mountains();
        else if(what == "lakes") Projection::db->print_lakes();
        else if(what == "continents") Projection::db->print_continents();
        else if(what == "custom") Projection::db->print_custom();
        else{
            cout << "invalid command."<<endl;
        }
    }

    double get_scale(stringstream &ss){
        double scale = 1;
        string scale_start;
        auto c = ss.get();
        while(isspace(c)) c = ss.get();
        scale_start += c;
        scale_start += ss.get();
        if(scale_start != "1:"){
            cout << "please, type a scale in format \"1:[number]\". Scale ignored."<<endl;
            return scale;
        }
        ss >> scale;
        if(ss.fail()) {
            cout << "invalid scale." << endl;
        }
        return scale;
    }

    void get_cmd(stringstream &ss){
        string calc_type;
        ss >> calc_type;
        string output;
        double scale = 1;
        if(calc_type == "distance"){
            string p1, p2;
            ss >> p1;
            ss >> p2;
            string has_scale;
            ss >> has_scale;
            if(has_scale == "scale"){
                scale = get_scale(ss);
            }
            double result = current_projection.second->calculate_distance(p1,p2);
            cout <<fixed <<result/scale <<"km"<<endl;
        }
        else if(calc_type == "area"){
            if(existing_projections[current_projection.first].first.substr(0,6) == "hybrid"){
                cout << "Calculation of area between longitudes and latitudes in hybrid projections is currently not supported."<<endl;
                return;
            }
            string region;
            ss >> region;
            string has_scale;
            ss >> has_scale;
            if(has_scale == "scale"){
                scale = get_scale(ss);
            }
            double result = current_projection.second->calculate_rectangular_area(region);
            cout <<fixed <<result/pow(scale,2) << "km^2"<<endl;
        }
        else{
            cout << "invalid command."<<endl;
            return;
        }
    }

    void process_input(istream &is){
        string line;
        cout <<">main-menu: ";
        getline(is,line);
        while(getline(is,line)){

            if(line == "exit") {
                exit_cmd();
                return;
            }
            //transform(line.begin(),line.end(),line.begin(),::tolower);

            stringstream ss(line);
            string word;
            ss >> word;

            if(word == "help") help_cmd(ss);
            else if(word == "make") make_cmd(ss);
            else if(word == "enter") enter_cmd(ss);
            else if(word == "add") add_cmd(ss);
            else if(word == "menu") current_projection.second = real_projection;
            else if(word == "print") print_cmd(ss);
            else if(word == "get") get_cmd(ss);
            else cout << "invalid command. Please type \"help\" for help."<<endl;


            if(current_projection.second == real_projection) cout <<"\n>main-menu: ";
            else cout << "\n>" << current_projection.first<<": ";
        }
    }



};
