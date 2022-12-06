// Author: Xi Lin
#ifndef SOLVER_H
#define SOLVER_H

#include "map.hpp"
#include <unordered_set>
#include <queue>
#include <chrono>

class Solver{
protected:
    std::vector<std::vector<float>> price_map;
    std::vector<std::vector<int>> path_map;
    std::vector<float> solution_time;
    Map* m;
public:
    Solver(Map& map);
    void update_price_map();
    void reset_path_map();
    void save_price_map(std::string filename);
    void save_path_map(std::string filename);
};

class Floyd_Warshall: public Solver{
public:
    Floyd_Warshall(Map& m): Solver(m) {};
    void fw_solve();
};

class Incremental: public Floyd_Warshall{
private:
    std::unordered_map<int,std::vector<int>> affected_sources;
    std::vector<std::pair<int,int>> affected_pairs;
    void update_affected_sources(int new_edge);
    void incremental_APSP(int new_edge);
public:
    Incremental(Map& m): Floyd_Warshall(m) {}; 
    void incremental_solve();
    void save_affected_pairs(std::string filename);
};

#endif