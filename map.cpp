// Author: Xi Lin
# include "map.hpp"

Map::Map(std::string filename){
    std::ifstream file;
    file.open(filename);
    if(!file) throw std::runtime_error("Can not open the map file.");
    std::string line;
    while(std::getline(file,line)){
        std::stringstream l(line);
        int from, to;
        float low, high;
        l >> from >> to >> low >> high;
        graph[from][to] = new Node(low,high,seed++);
    }
    file.close();
};

int Map::map_points_num() const{
    return int(pc.points.size());
};

void Map::print_vertices(){
    std::cout << "Print vertices" << std::endl;
    for(int i=0; i < graph.size(); i++){
        std::cout << "Vertex: " << i << " Position: (" << pc.points[i].x << "," \
                  << pc.points[i].y << ")" << std::endl;
    }
    std::cout << std::endl;
};

void Map::print_edges(){
    std::cout << "Print edges" << std::endl;
    for(int i = 0; i < graph.size(); i++){
        for(int j = 0; j < graph.size(); j++){
            std::cout << "Edge: " << i << "->" << j << " ";
            Node* node = graph[i][j];
            if(node == nullptr) continue;
            std::vector<float> result = node->get_price_range();
            std::cout << "Price range: [" << result[0] << "," << result[1] << "] "; 
            std::cout << "Current price: " << node->get_price() << std::endl;
        }
    }
    std::cout << std::endl;
};

void Map::save_map(std::string filename){
    std::ofstream file;
    file.open(filename);
    if(!file) throw std::runtime_error("Can not open the map file.");
    
    std::string line("Vertices:\n");
    file << line;
    for(int i = 0; i < graph.size(); i++){
        line.clear();
        line += std::to_string(i) + " " + std::to_string(pc.points[i].x) \
             + " " + std::to_string(pc.points[i].y);
        file << line;
        file << "\n";
    }
    file << "\n";
    
    line = "Edges:\n";
    file << line;
    for(int i = 0; i < graph.size(); i++){
        for(int j = 0; j < graph.size(); j++){
            line.clear();
            Node* node = graph[i][j];
            if(node == nullptr) continue;
            std::vector<float> result = node->get_price_range();
            line += std::to_string(i) + " " + std::to_string(j) \
                 + " " + std::to_string(result[0]) + " " + std::to_string(result[1]);
            file << line;
            file << "\n";
        }
    }
    file.close();
};

void Map::save_current_edges(std::string filename){
    std::ofstream file;
    file.open(filename);
    if(!file) throw std::runtime_error("Can not open the map file.");

    std::string line;
    for(int i = 0; i < graph.size(); i++){
        for(int j = 0; j < graph.size(); j++){
            Node* node = graph[i][j];
            if(node == nullptr) continue;
            line += std::to_string(i) + " " + std::to_string(j);
            file << line;
            file << "\n";
            line.clear();
        }
    }
    file.close();
}

void Map::save_current_price(std::string filename){
    std::ofstream file;
    file.open(filename);
    if(!file) throw std::runtime_error("Can not open the price file.");
    
    std::string line;
    float price;
    float float_max = std::numeric_limits<float>::max();
    for(int i = 0; i < graph.size(); i++){
        for(int j = 0; j < graph.size(); j++){
            Node* node = graph[i][j];
            if(node == nullptr) price = float_max;
            else price = node->get_price();
            line += std::to_string(price);
            if(j < graph.size()-1) line += " ";
        }
        file << line;
        file << "\n";
        line.clear();
    }
    file.close();
};

void Map::save_new_edges(std::string filename){
    std::ofstream file;
    file.open(filename);
    if(!file) throw std::runtime_error("Can not open the new edges file.");

    std::string line;
    for(auto edge:most_recent_new_edges){
        int pc_num = map_points_num();
        int i = edge.first / pc_num;
        int j = edge.first % pc_num;
        line += std::to_string(i) + " " + std::to_string(j);
        file << line;
        file << "\n";
        line.clear();
    }
    file.close();
};

void Map::update_price(){
    for(int i = 0; i < graph.size(); i++){
        for(int j = 0; j < graph.size(); j++){
            Node* node = graph[i][j];
            if(node == nullptr) continue;
            node->update_price();
        }
    }
};

void Map::insert_all_new_edges(){
    for(auto edge:most_recent_new_edges){
        insert_new_edge(edge);
    }
};

void Map::insert_new_edge(std::pair<int,Node*>& edge){
    int pc_num = map_points_num();
    int i = edge.first / pc_num;
    int j = edge.first % pc_num;
    graph[i][j] = edge.second;
};

void Map::clear_all_new_edges(){
    most_recent_new_edges.clear();
}