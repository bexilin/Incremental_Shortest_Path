// Author: Xi Lin
#include "solver.hpp"

Solver::Solver(const Map& map){
    m = &map;
    int num = m->pc.points.size();
    float float_max = std::numeric_limits<float>::max();
    std::vector<float> row(num,float_max);
    price_map = std::vector<std::vector<float>>(num,row);
    std::vector<int> row_2(num,-1);
    path_map = std::vector<std::vector<int>>(num,row_2);
};

void Solver::update_price_map(){
    float float_max = std::numeric_limits<float>::max();
    for(int i = 0; i < price_map.size(); i++){
        for(int j = 0; j < price_map.size(); j++){
            Map::Node* node = m->graph[i][j];
            if(node == nullptr){
                price_map[i][j] = float_max;
            }
            else price_map[i][j] = node->get_price();
        }
    }
};

void Solver::reset_path_map(){
    for(int i = 0; i < path_map.size(); i++){
        for(int j = 0; j < path_map.size(); j++){
            Map::Node* node = m->graph[i][j];
            if(node == nullptr){
                path_map[i][j] = -1;
            }
            else path_map[i][j] = j;
        }
    }
};

void Solver::save_price_map(std::string filename){
    std::ofstream file;
    file.open(filename);
    if(!file) throw std::runtime_error("Can not open the price map file.");
    
    std::string line;
    for(int i = 0; i < price_map.size(); i++){
        for(int j = 0; j < price_map.size(); j++){
            line += std::to_string(price_map[i][j]);
            if(j < price_map.size()-1) line += " ";
        }
        file << line;
        file << "\n";
        line.clear();
    }
    file.close();
};

void Solver::save_path_map(std::string filename){
    std::ofstream file;
    file.open(filename);
    if(!file) throw std::runtime_error("Can not open the path map file.");
    
    std::string line;
    for(int i = 0; i < path_map.size(); i++){
        for(int j = 0; j < path_map.size(); j++){
            line += std::to_string(path_map[i][j]);
            if(j < path_map.size()-1) line += " ";
        }
        file << line;
        file << "\n";
        line.clear();
    }
    file.close();
};

void Floyd_Warshall::solve(){
    std::cout << "Solving with Floyd Warshall algorithm" << std::endl; 
    
    update_price_map();
    reset_path_map();
    
    float float_max = std::numeric_limits<float>::max();
    for(int k = 0; k < price_map.size(); k++){
        for(int i = 0; i < price_map.size(); i++){
            for(int j = 0; j < price_map.size(); j++){
                if(price_map[i][k] == float_max || price_map[k][j] == float_max) continue;
                if(price_map[i][j] > price_map[i][k] + price_map[k][j]){
                    price_map[i][j] = price_map[i][k] + price_map[k][j];
                    path_map[i][j] = path_map[i][k];
                }
            }
        }
    }

    std::cout << "Finished!" << std::endl << std::endl; 
};