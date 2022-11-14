// Author: Xi Lin
#include "map_generator.hpp"
#include "solver.hpp"

int main(){
    // Map m("../paths_and_prices.txt");
    Map_generator g;
    g.create_map();
    g.m.save_map("map.csv");
    g.m.save_current_price("current_price.csv");

    Floyd_Warshall fw_solver(g.m);
    fw_solver.solve();
    fw_solver.save_path_map("fw_path_map.csv");
    fw_solver.save_price_map("fw_price_map.csv");
};