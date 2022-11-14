// Author: Xi Lin
#ifndef MAP_H
#define MAP_H

#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

struct PointCloud{
    struct Point
    {
        Point(float a, float b): x(a), y(b) {};
        float x,y;
    };

    std::vector<Point> points;

    inline size_t kdtree_get_point_count() const { return points.size(); };

    inline float kdtree_get_pt(const size_t idx, const size_t dim) const
    {
        if (dim == 0)
            return points[idx].x;
        else
            return points[idx].y;
    };

    template <class BBOX>
    bool kdtree_get_bbox(BBOX& /* bb */) const
    {
        return false;
    }
};

class Map{
protected:
    // Seed for PRNG
    uint32_t seed = 0;
    
    // class that store destination and price range of a path
    class Node{
    private:
        std::mt19937 gen;
        std::uniform_real_distribution<float> dis;
        float low, high, curr;
    public:
        Node(): low(-1), high(-1), curr(-1) {};
        Node(float l, float h, uint32_t seed): gen(std::mt19937(seed)),
                                               dis(std::uniform_real_distribution<float>(l,h)), 
                                               low(l),
                                               high(h)
        {
            update_price();
        };
        void update_price(){
            curr = dis(gen);
        };
        float get_price(){return curr;};
        std::vector<float> get_price_range(){return {low,high};};
    };

    // Adjacency matrix storing links and corresponding flight price range
    std::vector<std::vector<Node*>> graph;
    
    // Locations of map points 
    PointCloud pc;
public:
    Map() {};

    // read map from file
    Map(std::string filename);

    void print_vertices();
    
    void print_edges();

    void save_map(std::string filename);

    void save_current_price(std::string filename);
    
    void update_price();
    
    void draw_map();

    friend class Map_generator;
    friend class Solver;
};

#endif