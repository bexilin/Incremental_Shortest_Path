// Author: Xi Lin
#include "map_generator.hpp"

int main(){
    // Map m("../paths_and_prices.txt");
    Map_generator g;
    g.create_map();

    g.m.print_edges();
    g.m.update_price();
    g.m.print_edges();

    g.m.save_map("map.csv");
};