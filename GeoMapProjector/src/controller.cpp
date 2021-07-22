//
// Created by Ja on 01.03.2021.
//
#include "../include/projections.hpp"

#include<iostream>
#include<sstream>
#include<algorithm>

using namespace std;

class Controller{
private:


    const string projection_help = R"(
There are 3 basic projections to choose from:
     -> azimuthal - The mapping of radial lines can be visualized by imagining
                    a plane tangent to the Earth, with the central point as
                    tangent point. This projection uses polar coordinates starting
                    at the north pole. The directions from it are preserved.
                    For more info type "help azimuthal".
     -> cylindrical - The mapping of meridians to vertical lines can be
                      visualized by imagining a cylinder whose axis coincides
                      with the Earth's axis of rotation. This cylinder is
                      wrapped around the Earth, projected onto, and then unrolled.
                      Cylindrical projections stretch distances east-west and
                      use cartesian coordinates. These projections have a low
                      distortion around the equator and a high distortion around
                      the poles. For more info type "help cylindrical".
     -> conic - One or two standard parallels are defined (30 degrees in our case).
                They may be visualized as secant lines where the cone intersects the globe,
                or, as the tangent line where the cone is tangent to the globe. The
                resulting conic map has low distortion in scale, shape, and area near those
                standard parallels. For more info type "help conic".
     + hybrid - projection plane is an affine transformation of some of the
                projections mentioned above. They may have both polar and
                cartesian coordinate system. For more info type "help hybrid".
     + real - basic latitudinal and longitudinal geographical coordinates. Type "help real" for more info.
    ----------------------------------------------------------------------------------------------)";
    const string azimuthal_help = R"(
"azimuthal" projections:
  -> gnomonic - cannot project the equator
  -> stereographic - cannot project the south pole
  -> orthographic - can show only one hemisphere
  -> postel
  -> lambert - usually used only for one hemisphere
Coordinates in this projection are "rho" (distance from the North pole)
and "epsilon" (the angle analogous to longitude)
----------------------------------------------------------------------------------------------)";
    const string cylindrical_help = R"(
"cylindrical" projections (cannot project poles):
  1) one standard parallel:
     -> equirectangular (also called "Marin projection")
     -> lambert
     -> mercator
     -> perspective
  2) two standard parallels:
     -> behrmann
     -> trystan-edwards
     -> gall
Coordinates in this projection are "y" (vertical distance from the equator)
and "x" (horizontal distance from the prime meridian)
----------------------------------------------------------------------------------------------)";
    const string conic_help = R"(
"conic" projections:
  -> ptolemy (also called Equidistant conic projection)
  -> lambert
Coordinates in this projection are "rho" (distance from the North pole)
and "epsilon" (the angle analogous to longitude)
----------------------------------------------------------------------------------------------)";
    const string hybrid_help = R"(
"hybrid" projections:
  -> sanson - the projection looks like an onion, cartesian coordinate system
  -> werner-stab - the projection looks like a heart, polar coordinate system
----------------------------------------------------------------------------------------------)";
    const string get_help = R"(
COMMAND: get [(1)|(2)|(3)|(4)]
 (1) point (real) <name> - when "real" is present, prints the real coordinates of a point.
                           It can be used to transform coordinates of the locally saved point into
                           real coordinates.
                         - when "real" is not present, prints the coordinates of a point
                           in the current projection.
 (2) region <name> - prints the coordinates of a region in the current projection.
                     If the region is a country, it also prints new coordinates of the capital city.
 (3) distance <name1> <name2> (scale <scale> (units [m|cm|mm])) - prints the distance
                    between two points in the current projection in kilometres.
                  - by adding "scale <scale>" the scale of the projection can be
                    set.
                  - <scale> has to be in format "1:<int>".
                  - when using scale, units can be set by adding "units [m|cm|mm]".
 (4) area <name> - prints the area between the longitudes and the latitudes defining
                   the borders of the region <name> in kilometres squared.
<name> can be in format "<country_name>.capital"
Parentheses " () " mean that their content is optional.
Brackets "[o1|o2|o3|...]" mean that the one of the options o1,o2,... must be chosen.
Angle brackets "<name>" mean that value with given property must be substituted in there.
----------------------------------------------------------------------------------------------)";
    const string print_help = R"(
COMMAND: print [countries|cities|mountains|lakes|continents|custom|local] - prints the
                  given data that is saved in that category.
                - "local" option prints the custom local points saved in the current projection.
                - the rest of the options prints data saved in the main database.
Brackets "[o1|o2|o3|...]" mean that the one of the options o1,o2,... must be chosen.
----------------------------------------------------------------------------------------------)";
    const string home_help = R"(
COMMAND: home - switches the current projection back into the real one. This "projection" does
                not contain any local points, each data saved in real coordinates is being saved in
                the main database.
----------------------------------------------------------------------------------------------)";
    const string add_help = R"(
COMMAND: add [(1)|(2)]
  (1) point [real|local] <name> <lat> <lon> - saves the point with name <name> and coordinates <lat> <lon>
                     - local <name> <lat> <lon> - The point is saved into the local projection
                                     database with the coordinates having the orientation
                                     of the latitude and longitude in real geographical projection.
                     - real <name> <lat> <lon> - The point is saved into the main database in section "custom"
                                   - <lat> <lon> are the latitude and longitude in a real geographical projection.
  (2) region <name> <south> <north> <east> <west> - saves the region with name <name> and the
                   latitudinal and longitudinal borders in real geographical projection.
                   The region is saved in the main database in section "custom"
<name> can be a single word or multiple words bounded with the quotation marks " "
Brackets "[o1|o2|o3|...]" mean that the one of the options o1,o2,... must be chosen.
Angle brackets "<name>" mean that value with given property must be substituted in there.
 ----------------------------------------------------------------------------------------------)";
    const string enter_help = R"(
COMMAND: enter <main_projection_type> <projection_subtype> - changes the current projection
         into the given one.
       - <main_projection_type> one of the listed in "help projections"
       - <projection_subtype> - one of the listed in "help <main_projection_type>"
EXAMPLE: "enter conic ptolemy"
Angle brackets "<name>" mean that value with given property must be substituted in there.
----------------------------------------------------------------------------------------------)";
    const string help = R"(
COMMAND: help [projections|enter|add|home|print|get|<projection_name>] - prints a manual page
 for a given command.

 Brackets "[o1|o2|o3|...]" mean that the one of the options o1,o2,... must be chosen.
 Angle brackets "<name>" mean that value with given property must be substituted in there.
 ----------------------------------------------------------------------------------------------)";
    const string real_help = R"(
"real" projection - the coordinates are standard geographical coordinates in degrees,
                    with the equator being on latitude 0 degrees and the prime meridian
                    on longitude 0 degrees
----------------------------------------------------------------------------------------------)";

    void help_cmd(stringstream &ss) const{
        string word;
        ss >> word;

        if(word.empty()) cout << help <<endl;
        else if(word == "projections") cout << projection_help <<endl;
        else if(word == "azimuthal") cout << azimuthal_help <<endl;
        else if(word == "cylindrical") cout << cylindrical_help<<endl;
        else if(word == "conic") cout << conic_help<<endl;
        else if(word == "hybrid") cout << hybrid_help<<endl;
        else if(word == "real") cout << real_help <<endl;
        else if(word == "enter") cout <<enter_help <<endl;
        else if(word == "add") cout <<add_help<<endl;
        else if(word == "home") cout <<home_help<<endl;
        else if(word == "print") cout <<print_help<<endl;
        else if(word == "get") cout << get_help << endl;
    }

    static void exit_cmd(){
        cout <<"sea you next time!"<<endl;
    }

    void enter_cmd(stringstream &ss){
        string type, subtype;
        ss >> type;
        transform(type.begin(), type.end(),type.begin(),::tolower);
        if(type == "real"){
            current_projection.first = type;
            current_projection.second = real_projection;
            return;
        }
        ss >> subtype;
        transform(subtype.begin(), subtype.end(),subtype.begin(),::tolower);
        auto alias_it = projections.find(type);
        if(alias_it == projections.end()){
            cout << "This projection does not exist. Type \"help projections\" for more info."<<endl;
            return;
        }
        auto subtype_it = (alias_it->second).find(subtype);
        if(subtype_it == alias_it->second.end()){
            cout << "This projection does not exist. Type \"help projections\" for more info."<<endl;
            return;
        }
        current_projection.first = type+" "+subtype;
        current_projection.second = projections[type][subtype];
    }

    static bool is_valid_name(const string &str){
        return all_of(str.begin(),str.end(),[](char c){return (isalnum(c) || c == ' ');});
    }

    static string get_multiword_name(stringstream &ss){
        string result;
        ss >> result;
        if(result[0] != '"') return result;

        string next_word;
        ss >> next_word;
        while(!ss.eof() && next_word[next_word.size()-1] != '"'){
            result += " "+next_word;
            ss >>next_word;
        }
        if(ss.eof() && next_word[next_word.size()-1] != '"'){
            return "";
        }
        else result += " "+next_word;
        return result.substr(1,result.size()-2);
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
                name = get_multiword_name(ss);
                if(!is_valid_name(name)){
                    cout <<"error: invalid name. Use only letters, spaces and digits."<<endl;
                    return;
                }
                ss >> lat;
                ss >> lon;
                if(ss.fail() || name.empty()){
                    cout <<"error: invalid input."<<endl;
                    return;
                }
                Projection::db->add_data(name,lat,lon,object_type::CUSTOM);
                cout <<"point \""<<name<<"\" was successfully added to the main database."<<endl;
                return;
            }
            else if(coord_type == "local"){
                if(current_projection.second == real_projection){
                    cout << "error: you currently are not in any projection."<<endl;
                    return;
                }
                string name;
                name = get_multiword_name(ss);
                if(!is_valid_name(name)){
                    cout <<"error: invalid name. Use only letters, spaces and digits."<<endl;
                    return;
                }
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
            name = get_multiword_name(ss);
            if(!is_valid_name(name)){
                cout <<"error: invalid name. Use only letters, spaces and digits."<<endl;
                return;
            }
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

    void print_cmd(stringstream &ss) const {
        string what;
        ss >> what;

        if(what == "local"){
            if(current_projection.second == real_projection){
                cout << "we are not in any projection."<<endl;
                return;
            }
            current_projection.second->print_local();
        }
        else if(what == "countries") Projection::db->print_countries();
        else if(what == "cities") Projection::db->print_cities();
        else if(what == "mountains") Projection::db->print_mountains();
        else if(what == "lakes") Projection::db->print_lakes();
        else if(what == "continents") Projection::db->print_continents();
        else if(what == "custom") Projection::db->print_custom();
        else if(what == "local") current_projection.second->print_local();
        else{
            cout << "invalid command."<<endl;
        }
    }

    static double get_scale(stringstream &ss){
        double scale = 1;
        string scale_start;
        auto c = ss.get();
        while(isspace(c)) c = ss.get();
        scale_start += (char)c;
        scale_start += (char)ss.get();
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

    void get_cmd(stringstream &ss) const{
        string calc_type;
        ss >> calc_type;
        string output;
        double scale = 1;
        double conversion_factor = 1;
        if(calc_type == "point"){
            shared_ptr<Coords> coords;
            string p = get_multiword_name(ss);
            if(p == "real"){
                p = get_multiword_name(ss);
                coords = current_projection.second->convert_to_real_coords(p);
            }
            else{
                coords = current_projection.second->find_point(p);
            }
            if(coords == nullptr){
                cout << "the point does not exist."<<endl;
                return;
            }
            cout << coords->get_coords() << endl;
        }
        else if(calc_type == "region"){
            shared_ptr<Region> region;
            string p = get_multiword_name(ss);
            region = current_projection.second->find_region(p);
            if(region == nullptr){
                cout << "the point does not exist."<<endl;
                return;
            }
            cout << region->print_coords() << endl;
        }
        else if(calc_type == "distance"){
            string p1, p2;
            p1 = get_multiword_name(ss);
            p2 = get_multiword_name(ss);
            string has_scale;
            ss >> has_scale;
            if(has_scale == "scale"){
                scale = get_scale(ss);
            }
            string has_unit;
            string unit = "km";
            ss >> has_unit;
            if(has_unit == "units"){
                ss >> unit;
                if(unit == "m") conversion_factor = 1000;
                else if(unit == "cm") conversion_factor = 100000;
                else if(unit == "mm") conversion_factor = 1000000;
            }
            double result = current_projection.second->calculate_distance(p1,p2);
            double scaled_converted_result = (result / scale) * conversion_factor;
            cout <<fixed <<scaled_converted_result<<unit<<endl;
        }
        else if(calc_type == "area"){
            if(existing_projections.at(current_projection.first).first.substr(0,6) == "hybrid"){
                cout << "Calculation of area between longitudes and latitudes in hybrid projections is currently not supported."<<endl;
                return;
            }
            string region;
            region = get_multiword_name(ss);
            string has_scale;
            ss >> has_scale;
            if(has_scale == "scale"){
                scale = get_scale(ss);
            }
            string has_unit;
            string unit = "km";
            ss >> has_unit;
            if(has_unit == "units"){
                ss >> unit;
                if(unit == "m") conversion_factor = 1000000;
                else if(unit == "cm") conversion_factor = 10000000000;
                else if(unit == "mm") conversion_factor = 1000000000000;
            }
            double result = current_projection.second->calculate_rectangular_area(region);
            double scaled_converted_result = (result/pow(scale,2))*conversion_factor;
            cout <<fixed << scaled_converted_result<<unit<< "^2"<<endl;
        }
        else{
            cout << "invalid command."<<endl;
            return;
        }
    }

public:
    map<string, pair<string,shared_ptr<Projection>>> existing_projections;

    shared_ptr<Projection> real_projection;

    pair<string,shared_ptr<Projection>> current_projection;

    map<string, map<string, shared_ptr<Projection>>> projections;

    Controller(){
        real_projection = make_shared<RealProjection>();
        current_projection.first = "real";
        current_projection.second = real_projection;

        projections["real"]["real"] = real_projection;
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
    }

    void process_input(istream &is){
        string line;
        cout <<"\n Welcome to GeoMapProjector!\nFor more info about supported commands type \"help\"\nType \"exit\" to exit the program.\n";
        cout <<">real: ";
        getline(is,line);
        while(getline(is,line)){

            if(line == "exit") {
                exit_cmd();
                return;
            }

            stringstream ss(line);
            string word;
            ss >> word;

            if(word == "help") help_cmd(ss);
            else if(word == "enter") enter_cmd(ss);
            else if(word == "add") add_cmd(ss);
            else if(word == "home") current_projection.second = real_projection;
            else if(word == "print") print_cmd(ss);
            else if(word == "get") get_cmd(ss);
            else cout << "invalid command. Please type \"help\" for help."<<endl;


            if(current_projection.second == real_projection) cout <<"\n>real: ";
            else cout << "\n>" << current_projection.first<<": ";
        }
    }
};
