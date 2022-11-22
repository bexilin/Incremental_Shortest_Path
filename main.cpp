// Author: Xi Lin
#include "map_generator.hpp"
#include "solver.hpp"

int main(){
    Map_generator g;
    g.create_map();
    g.m.save_map("map.csv");
    g.m.save_current_price("current_price.csv");

    Incremental inc_solver(g.m);
    // Use floyd Warshall to solve the inital map
    inc_solver.fw_solve();
    inc_solver.save_path_map("inc_path_map.csv");
    inc_solver.save_price_map("inc_price_map.csv");

    Floyd_Warshall fw_solver(g.m);
    fw_solver.fw_solve();
    fw_solver.save_path_map("fw_path_map.csv");
    fw_solver.save_price_map("fw_price_map.csv");

    // create a new edge in the map
    g.create_new_edges();
    g.m.save_map("map_2.csv");
    g.m.save_current_price("current_price_2.csv");

    inc_solver.incremental_solve();
    inc_solver.save_path_map("inc_path_map_2.csv");
    inc_solver.save_price_map("inc_price_map_2.csv");

    fw_solver.fw_solve();
    fw_solver.save_path_map("fw_path_map_2.csv");
    fw_solver.save_price_map("fw_price_map_2.csv");
};