//
// Created by Ja on 01.03.2021.
//
#include "../include/projections.hpp"

#include<iostream>
#include<sstream>
#include<algorithm>

using namespace std;

class Controller{
public:
    map<string, pair<string,shared_ptr<Projection>>> existing_projections;

    shared_ptr<Projection> real_projection;

    pair<string,shared_ptr<Projection>> current_projection;

    map<string, map<string, shared_ptr<Projection>>> projections;

    string projection_help;
    string azimuthal_help;
    string cylindrical_help;
    string conic_help;
    string hybrid_help;
    string get_help;
    string print_help;
    string home_help;
    string add_help;
    string enter_help;
    string help;
    string real_help;

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

        projection_help = "There are 3 basic projections to choose from:\n"
                                    " -> azimuthal - The mapping of radial lines can be visualized by imagining\n"
                                    "                a plane tangent to the Earth, with the central point as \n"
                                    "                tangent point. This projection uses polar coordinates starting \n"
                                    "                at the north pole. The directions from it are preserved. \n"
                                    "                For more info type \"help azimuthal\".\n"
                                    " -> cylindrical - The mapping of meridians to vertical lines can be \n"
                                    "                  visualized by imagining a cylinder whose axis coincides\n"
                                    "                  with the Earth's axis of rotation. This cylinder is \n"
                                    "                  wrapped around the Earth, projected onto, and then unrolled.\n"
                                    "                  Cylindrical projections stretch distances east-west and \n"
                                    "                  use cartesian coordinates. These projections have a low\n"
                                    "                  distortion around the equator and a high distortion around\n"
                                    "                  the poles. For more info type \"help cylindrical\".\n"
                                    " -> conic - One or two standard parallels are defined (30 degrees in our case).\n"
                                    "            They may be visualized as secant lines where the cone intersects the globe,\n"
                                    "            or, as the tangent line where the cone is tangent to the globe. The \n"
                                    "            resulting conic map has low distortion in scale, shape, and area near those \n"
                                    "            standard parallels. For more info type \"help conic\".\n"
                                    " + hybrid - projection plane is an affine transformation of some of the\n"
                                    "            projections mentioned above. They may have both polar and\n"
                                    "            cartesian coordinate system. For more info type \"help hybrid\".\n"
                                    " + real - basic latitudinal and longitudinal geographical coordinates. Type \"help real\" for more info.\n"
                          "----------------------------------------------------------------------------------------------\n";


        azimuthal_help = "\"azimuthal\" projections:"
                                      " -> gnomonic - cannot project the equator\n"
                                      " -> stereographic - cannot project the south pole\n"
                                      " -> orthographic - can show only one hemisphere\n"
                                      " -> postel\n"
                                      " -> lambert - usually used only for one hemisphere\n"
                                      "Coordinates in this projection are \"rho\" (distance from the North pole)\n"
                                      "and \"epsilon\" (the angle analogous to longitude)\n"
                         "----------------------------------------------------------------------------------------------\n";


        cylindrical_help = "\"cylindrical\" projections (cannot project poles):\n"
                                        "  1) one standard parallel:\n"
                                        "     -> equirectangular (also called \"Marin projection\")\n"
                                        "     -> lambert\n"
                                        "     -> mercator\n"
                                        "     -> perspective\n"
                                        "  2) two standard parallels:\n"
                                        "     -> behrmann\n"
                                        "     -> trystan-edwards\n"
                                        "     -> gall\n"
                                        "Coordinates in this projection are \"y\" (vertical distance from the equator)\n"
                                        "and \"x\" (horizontal distance from the prime meridian)\n"
                           "----------------------------------------------------------------------------------------------\n";


        conic_help = "\"conic\" projections:\n"
                                  " -> ptolemy (also called Equidistant conic projection)\n"
                                  " -> lambert\n"
                     "Coordinates in this projection are \"rho\" (distance from the North pole)\n"
                     "and \"epsilon\" (the angle analogous to longitude)\n"
                     "----------------------------------------------------------------------------------------------\n";


        hybrid_help = "\"hybrid\" projections:\n"
                                   " -> sanson - the projection looks like an onion, cartesian coordinate system\n"
                                   " -> werner-stab - the projection looks like a heart, polar coordinate system\n"
                      "----------------------------------------------------------------------------------------------\n";

        real_help = "\"real\" projection - the coordinates are standard geographical coordinates in degrees,\n"
                    "                      with the equator being on latitude 0 degrees and the prime meridian\n"
                    "                      on longitude 0 degrees\n"
                    "----------------------------------------------------------------------------------------------\n";

        get_help = "COMMAND: get [(1)|(2)|(3)|(4)]\n"
                   " (1) point (real) <name> - when \"real\" is present, prints the real coordinates of a point.\n"
                   "                           It can be used to transform coordinates of the locally saved point into \n"
                   "                           real coordinates.\n"
                   "                         - when \"real\" is not present, prints the coordinates of a point\n"
                   "                           in the current projection.\n"
                   " (2) region <name> - prints the coordinates of a region in the current projection.\n"
                   "                     If the region is a country, it also prints new coordinates of the capital city.\n"
                   " (3) distance <name1> <name2> (scale <scale> (units [m|cm|mm])) - prints the distance\n"
                   "                    between two points in the current projection in kilometres. \n"
                   "                  - by adding \"scale <scale>\" the scale of the projection can be\n"
                   "                    set. \n"
                   "                  - <scale> has to be in format \"1:<int>\".\n"
                   "                  - when using scale, units can be set by adding \"units [m|cm|mm]\"."
                   " (4) area <name> - prints the area between the longitudes and the latitudes defining\n"
                   "                   the borders of the region <name> in kilometres squared.\n"
                   "\n"
                   "<name> can be in format \"<country_name>.capital\"\n"
                   "Parentheses \"()\" mean that their content is optional.\n"
                   "Brackets \"[o1|o2|o3|...]\" mean that the one of the options o1,o2,... must be chosen.\n"
                   "Angle brackets \"<name>\" mean that value with given property must be substituted in there.\n"
                   "----------------------------------------------------------------------------------------------\n";

        print_help = "COMMAND: print [countries|cities|mountains|lakes|continents|custom|local] - prints the\n"
                     "                  given data that is saved in that category.\n"
                     "                - \"local\" option prints the custom local points saved in the current projection.\n"
                     "                - the rest of the options prints data saved in the main database.\n"
                     "\n"
                     "Brackets \"[o1|o2|o3|...]\" mean that the one of the options o1,o2,... must be chosen.\n"
                     "----------------------------------------------------------------------------------------------\n";

        home_help = "COMMAND: home - switches the current projection back into the real one. This \"projection\" does\n"
                    "                not contain any local points, each data saved in real coordinates is being saved in\n"
                    "                the main database.\n"
                    "----------------------------------------------------------------------------------------------\n";


        add_help = "COMMAND: add [(1)|(2)]\n"
                   " (1) point [real|local] <name> <lat> <lon> - saves the point with name <name> and coordinates <lat> <lon>\n"
                   "                    - local <name> <lat> <lon> - The point is saved into the local projection\n"
                   "                                    database with the coordinates having the orientation\n"
                   "                                    of the latitude and longitude in real geographical projection.\n"
                   "                    - real <name> <lat> <lon> - The point is saved into the main database in section \"custom\"\n"
                   "                                  - <lat> <lon> are the latitude and longitude in a real geographical projection.\n"
                   " (2) region <name> <south> <north> <east> <west> - saves the region with name <name> and the\n"
                   "                  latitudinal and longitudinal borders in real geographical projection.\n"
                   "                  The region is saved in the main database in section \"custom\"\n"
                   "\n"
                   "<name> can be a single word or multiple words bounded with the quotation marks \" \"\n"
                   "Brackets \"[o1|o2|o3|...]\" mean that the one of the options o1,o2,... must be chosen.\n"
                   "Angle brackets \"<name>\" mean that value with given property must be substituted in there.\n"
                   "----------------------------------------------------------------------------------------------\n";

        enter_help = "COMMAND: enter <main_projection_type> <projection_subtype> - changes the current projection\n"
                     "         into the given one.\n"
                     "       - <main_projection_type> one of the listed in \"help projections\"\n"
                     "       - <projection_subtype> - one of the listed in \"help <main_projection_type>\"\n"
                     "\n"
                     "EXAMPLE: \"enter conic ptolemy\"\n"
                     "Angle brackets \"<name>\" mean that value with given property must be substituted in there.\n"
                     "----------------------------------------------------------------------------------------------\n";

        help = "COMMAND: help [projections|enter|add|home|print|get|<projection_name>] - prints a manual page for a given\n"
               "         command.\n"
               "\n"
               "Brackets \"[o1|o2|o3|...]\" mean that the one of the options o1,o2,... must be chosen.\n"
               "Angle brackets \"<name>\" mean that value with given property must be substituted in there.\n"
               "----------------------------------------------------------------------------------------------\n";

    }

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

    static void exit_cmd() {
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

    void get_cmd(stringstream &ss){
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
            if(existing_projections[current_projection.first].first.substr(0,6) == "hybrid"){
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
