// Author: Xi Lin
#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include "map.hpp"
#include "nanoflann.hpp"
#include <fstream>
#include <random>
#include <cmath>

class Map_generator{
private:
    // PRNG seed
    uint32_t seed = 1;

    // map dimensions
    float width = 2000;
    float height = 2000;

    // number of points
    int num_points = 100;

    // minimum distance between two points 
    float min_dis = 20;

    // Number of newly created edges each time after initial map generation 
    int num_new_edge = 5; 

    // base price factor
    float p = 1;

    // price range percentage factor
    float r = 0.5;

    // number of initial edges 
    int num_init_edge; 

    // PRNG for point locations and links
    std::mt19937 gen;
    std::uniform_real_distribution<float> uniform_dis_x, uniform_dis_y;
    std::uniform_int_distribution<> uniform_dis_edge;

public:
    Map m;
    
    Map_generator();

    Map_generator(int num, float p);

    void create_map();

    void create_new_edges();
};

#endif