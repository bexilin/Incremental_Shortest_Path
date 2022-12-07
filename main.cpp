// Author: Xi Lin
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include "map_generator.hpp"
#include "solver.hpp"

void experiment(int num_points, float portion, int num_new_edge, float& t_inc, float& t_fw){

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << portion;
    std::string p_string = ss.str();

    std::cout << "===== Experiment Setup =====" << std::endl;
    std::cout << "number of points: " << std::to_string(num_points) << std::endl;
    std::cout << "portion of existing edges: " << p_string << std::endl;
    std::cout << std::endl;

    std::string dir = "experiment_data/n_" + std::to_string(num_points) + "_p_" + p_string + "/";

    if (mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        if( errno == EEXIST ) {
            std::cout << "Warning: data directory already exists!" << std::endl;
        } 
        else {
            throw std::runtime_error("Could not create data directory");
        }
    }

    Map_generator g(num_points,portion,num_new_edge);
    g.create_map();
    g.m.save_map(dir+"map.csv");
    g.m.save_current_price(dir+"current_price.csv");
    g.m.save_current_edges(dir+"current_edges.csv");

    Incremental inc_solver(g.m);
    // Use floyd Warshall to solve the inital map
    inc_solver.fw_solve();
    inc_solver.save_path_map(dir+"inc_path_map.csv");
    inc_solver.save_price_map(dir+"inc_price_map.csv");

    Floyd_Warshall fw_solver(g.m);
    fw_solver.fw_solve();
    fw_solver.save_path_map(dir+"fw_path_map.csv");
    fw_solver.save_price_map(dir+"fw_price_map.csv");

    // create new edges in the map
    g.create_new_edges();
    g.m.save_new_edges(dir+"new_edges.csv");
    g.m.save_map(dir+"map_2.csv");
    g.m.save_current_price(dir+"current_price_2.csv");

    inc_solver.incremental_solve();
    inc_solver.save_path_map(dir+"inc_path_map_2.csv");
    inc_solver.save_price_map(dir+"inc_price_map_2.csv");
    inc_solver.save_affected_pairs(dir+"inc_affected_pairs.csv");

    fw_solver.fw_solve();
    fw_solver.save_path_map(dir+"fw_path_map_2.csv");
    fw_solver.save_price_map(dir+"fw_price_map_2.csv");

    std::vector<float> t1 = inc_solver.get_solution_time();
    std::vector<float> t2 = fw_solver.get_solution_time();

    t_inc = t1.back();
    t_fw = t2.back();
};

int main(){
    std::vector<int> num_points = {10,50,100,500,1000};
    std::vector<float> portion = {0.2,0.5,0.8};
    
    std::string dir = "experiment_data";

    if (mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        if( errno == EEXIST ) {
            std::cout << "Warning: data directory already exists!" << std::endl;
        } 
        else {
            throw std::runtime_error("Could not create data directory");
        }
    }

    std::ofstream time_file;
    time_file.open("experiment_data/times.csv");
    if(!time_file) throw std::runtime_error("Can not open the time file.");
    
    std::string line;
    for(auto num:num_points){
        for(auto p:portion){
            float t_inc, t_fw;
            experiment(num,p,num/2,t_inc,t_fw);
            
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1) << p;
            std::string p_string = ss.str();
            
            line += std::to_string(num) + " " + p_string + " " + \
                    std::to_string(t_inc) + " " + std::to_string(t_fw);
            
            time_file << line;
            time_file << "\n";
            line.clear();
        }
    }
    time_file.close();
};