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
    uint32_t seed = 0;

    // map dimensions
    float width = 10000;
    float height = 10000;

    // number of points
    int num_points = 100;

    // minimum distance between two points 
    float min_dis = 100;

    // Max number of newly created edges each time after initial map generation 
    int num_new_edge = 3; 

    // base price factor
    float p = 1;

    // price range percentage factor
    float r = 0.5;

    // probability of a link between two points
    float p_l = 0.1; 

    // PRNG for point locations and links
    std::mt19937 gen;
    std::uniform_real_distribution<float> uniform_dis_x, uniform_dis_y;
    std::binomial_distribution<int> binomial_dis;

public:
    Map m;
    
    Map_generator();

    void create_map();

    void insert_new_edges();
};

#endif