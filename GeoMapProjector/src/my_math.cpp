//
// Created by Ja on 01.03.2021.
//
#include "../include/my_math.hpp"

//#define EARTH_PERIMETER 6371.11

double deg_to_rad(double number){
    return number * M_PI / 180;
}

double rad_to_deg(double number){
    return number * 180 / M_PI;
}

double deg_sin(double number){
    double rad_number = deg_to_rad(number);
    return sin(rad_number);
}

double deg_cos(double number){
    double rad_number = deg_to_rad(number);
    return cos(rad_number);
}

double deg_tan(double number){
    double rad_number = deg_to_rad(number);
    return tan(rad_number);
}

double deg_cot(double number){
    double rad_number = deg_to_rad(number);
    return 1/tan(rad_number);
}

double deg_arccos(double number){
    double rad_number = deg_to_rad(number);
    return acos(rad_number);
}

double deg_arcsin(double number){
    double rad_number = deg_to_rad(number);
    return asin(rad_number);
}

double normalize_deg(double degree){
    return fmod(degree,360.0);
}
