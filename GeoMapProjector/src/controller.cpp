//
// Created by Ja on 01.03.2021.
//
#include "../include/projections.hpp"

#include<iostream>
#include<fstream>
#include<sstream>

#define AZIMUTH_PROJ_INDEX 0
#define CYLINDRICAL_PROJ_INDEX 1
#define CONIC_PROJ_INDEX 2


using namespace std;

class Controller{
public:
    map<string, Projection> projections;

    shared_ptr<Projection> current_projection;


    Controller(){}

    const string general_help = "There are 3 basic projections to choose from:\n"
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


    const string azimuthal_help = "Azimuthal projections:"
                                  " -> Gnomonic - cannot project the equator\n"
                                  " -> Stereographic - cannot project the south pole\n"
                                  " -> Ortographic - can show only one hemisphere\n"
                                  " -> Postel\n"
                                  " -> Lambert - usually used only for one hemisphere";


    const string cylindrical_help = "Cylindrical projections (cannot project poles):\n"
                                    "  1) one standard parallel:\n"
                                    "     -> Equirectangular\n"
                                    "     -> Lambert\n"
                                    "     -> Mercator\n"
                                    "     -> Perspective\n"
                                    "  2) two standard parallels:\n"
                                    "     -> Behrmann\n"
                                    "     -> Trystan Edwards\n"
                                    "     -> Gall";


    const string conic_help = "Conic projections:\n"
                              " -> Ptolemy\n"
                              " -> Lambert";


    const string hybrid_help = "Hybrid projections:\n"
                               " -> Sanson - the projection looks like an onion\n"
                               " -> Werner-Stab - the projection look like a heart";


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

    void process_input(istream &is){
        string line;
        while(getline(is,line)){
            if(line == "exit") {
                cout <<"sea you next time!"<<endl;
                return;
            }

            stringstream ss(line);
            string word;
            getline(ss, word, ' ');

            if(word == "help"){
                getline(ss,word,' ');

                if(word == "projections") print_projections_help();
                else if(word.compare("azimuthal") == 0) print_help_azimuth();
                else if(word == "cylindrical") print_help_cylindrical();
                else if(word == "conic") print_help_conic();
                else if(word == "hybrid") print_help_hybrid();
            }
            if(word == "add"){

            }
            if(word == "print"){

            }
            if(word == "get"){

            }
            if(word == "rescale"){

            }
            if(word == "reset"){

            }



        }
    }



};
