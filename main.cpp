// Author: Xi Lin
#include <iomanip>
#include <sstream>
#include "map_generator.hpp"
#include "solver.hpp"

void experiment(int num_points, float portion){

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << portion;
    std::string p_string = ss.str();

    std::cout << "===== Experiment Setup =====" << std::endl;
    std::cout << "number of points: " << std::to_string(num_points) << std::endl;
    std::cout << "portion of existing edges: " << p_string << std::endl;
    std::cout << std::endl;

    std::string dir = "experiment_data/n_" + std::to_string(num_points) + "_p_" + p_string + "/";

    Map_generator g(num_points,portion);
    g.create_map();
    g.m.save_map(dir+"map.csv");
    g.m.save_current_price(dir+"current_price.csv");

    Incremental inc_solver(g.m);
    // Use floyd Warshall to solve the inital map
    inc_solver.fw_solve();
    inc_solver.save_path_map(dir+"inc_path_map.csv");
    inc_solver.save_price_map(dir+"inc_price_map.csv");

    Floyd_Warshall fw_solver(g.m);
    fw_solver.fw_solve();
    fw_solver.save_path_map(dir+"fw_path_map.csv");
    fw_solver.save_price_map(dir+"fw_price_map.csv");

    // create a new edge in the map
    g.create_new_edges();
    g.m.save_map(dir+"map_2.csv");
    g.m.save_current_price(dir+"current_price_2.csv");

    inc_solver.incremental_solve();
    inc_solver.save_path_map(dir+"inc_path_map_2.csv");
    inc_solver.save_price_map(dir+"inc_price_map_2.csv");
    inc_solver.save_affected_pairs(dir+"inc_affected_pairs.csv");

    fw_solver.fw_solve();
    fw_solver.save_path_map(dir+"fw_path_map_2.csv");
    fw_solver.save_price_map(dir+"fw_price_map_2.csv");
};

int main(){
    std::vector<int> num_points = {100};
    std::vector<float> portion = {0.2};
    for(auto num:num_points){
        for(auto p:portion){
            experiment(num,p);
        }
    }
};