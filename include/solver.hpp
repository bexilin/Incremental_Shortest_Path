// Author: Xi Lin
#ifndef SOLVER_H
#define SOLVER_H

#include "map.hpp"
#include <unordered_set>
#include <queue>

class Solver{
protected:
    std::vector<std::vector<float>> price_map;
    std::vector<std::vector<int>> path_map;
    const Map* m;
public:
    Solver(const Map& map);
    void update_price_map();
    void reset_path_map();
    virtual void solve() = 0;
    void save_price_map(std::string filename);
    void save_path_map(std::string filename);
};

class Floyd_Warshall: public Solver{
public:
    Floyd_Warshall(const Map& m): Solver(m) {};
    void solve();
};

class Incremental: public Solver{
private:
    std::unordered_map<int,std::vector<int>> affected_sources;
    void update_affected_sources(int new_edge);
    void incremental_APSP(int source, int new_edge);
public:
    Incremental(const Map& m): Solver(m) {};
    void solve();
};

#endif