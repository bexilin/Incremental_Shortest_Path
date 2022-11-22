// Author: Xi Lin
#include "solver.hpp"

Solver::Solver(Map& map){
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
            if(i == j){
                price_map[i][j] = 0;
                continue;
            } 
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

void Floyd_Warshall::fw_solve(){
    std::cout << "Solving with Floyd Warshall algorithm" << std::endl;

    // m->insert_all_new_edges();
    
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

void Incremental::update_affected_sources(int new_edge){
    affected_sources.clear();
    int pc_num = m->map_points_num();
    int u = new_edge / pc_num, v = new_edge % pc_num;
    Map::Node* node = m->graph[u][v];
    if(node == nullptr) throw std::runtime_error("new edge not exist!");
    float w_uv = node->get_price();
    if(price_map[u][v] > w_uv){
        std::unordered_set<int> visited;
        std::queue<int> Q;
        Q.push(u);
        visited.insert(u);
        affected_sources[v] = {u};
        while(!Q.empty()){
            int x = Q.front();
            Q.pop();
            for(int z = 0; z < pc_num; z++){
                if(z == x) continue;
                if(m->graph[z][x] == nullptr) continue;
                
                float new_zv = price_map[z][u] + w_uv;
                if(!visited.count(z) && price_map[z][v] > new_zv){
                    Q.push(z);
                    visited.insert(z);
                    affected_sources[v].push_back(z);
                }
            }
        }
    }
    // for(auto s:affected_sources[v]) std::cout << s << std::endl;
    // std::cout << std::endl;
};

void Incremental::incremental_APSP(int new_edge){
    int pc_num = m->map_points_num();
    int u = new_edge / pc_num, v = new_edge % pc_num;
    Map::Node* node = m->graph[u][v];
    if(node == nullptr) throw std::runtime_error("new edge not exist!");
    float w_uv = node->get_price();
    if(price_map[u][v] > w_uv){
        std::queue<int> Q;
        Q.push(v);
        std::unordered_map<int,int> P;
        P[v]=v;
        std::unordered_set<int> visited;
        visited.insert(v);
        while(!Q.empty()){
            int y = Q.front();
            // std::cout << "y is " << y << std::endl;
            Q.pop();

            for(auto x:affected_sources[P[y]]){
                // std::cout << "x is " << x << std::endl;
                // std::cout << price_map[x][y] << std::endl;
                // std::cout << price_map[x][u] << std::endl;
                // std::cout << w_uv << std::endl;
                // std::cout << price_map[v][y] << std::endl;
                // std::cout << std::endl;
                if(price_map[x][y] > price_map[x][u] + w_uv + price_map[v][y]){
                    price_map[x][y] = price_map[x][u] + w_uv + price_map[v][y];
                    
                    // update shortest path
                    if(x != u) path_map[x][y] = path_map[x][u];
                    else path_map[x][y] = v;

                    if(y != v){
                        if(!affected_sources.count(y)) 
                            affected_sources[y] = std::vector<int>();
                        affected_sources[y].push_back(x);
                    }
                }
            }

            for(int w = 0; w < pc_num; w++){
                if(w == y) continue;
                if(m->graph[y][w] == nullptr) continue;
                if(!visited.count(w) && price_map[u][w] > w_uv + price_map[v][w]
                   && price_map[v][w] == price_map[v][y] + price_map[y][w])
                {
                    Q.push(w);
                    visited.insert(w);
                    P[w] = y;
                }
            }
        }
    }
};

void Incremental::incremental_solve(){
    std::cout << "Solving with incremental algorithm" << std::endl;

    for(auto new_edge:m->most_recent_new_edges){
        m->insert_new_edge(new_edge);
        update_affected_sources(new_edge.first);
        incremental_APSP(new_edge.first);
    }
    
    std::cout << "Finished!" << std::endl << std::endl;
};