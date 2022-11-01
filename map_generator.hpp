// Author: Xi Lin
#include "map.hpp"
#include <fstream>
#include <random>

class Map_generator{
private:
    // PRNG seed
    uint32_t seed = 0;

    // map dimensions
    float width = 1000;
    float height = 1000;

    // number of points
    uint32_t num_points = 10;

    // minimum distance between two points 
    float min_dis = 100;

    // base price factor
    float p = 1;

    // price range percentage factor
    float r = 0.5;

    // probability of a link between two points
    float p_l = 0.3; 

    // PRNG for point locations and links
    std::mt19937 gen;
    std::uniform_real_distribution<float> uniform_dis_x, uniform_dis_y;
    std::binomial_distribution<int> binomial_dis;

public:
    Map m;
    
    Map_generator();

    void create_map();
};