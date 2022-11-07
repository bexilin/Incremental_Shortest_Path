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
        if(!graph.count(from)){
            graph[from] = std::vector<Node*>();
        }
        graph[from].push_back(new Node(to,low,high,seed++));
    }
    file.close();
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
        for(Node* n:graph[i]){
            std::cout << "Edge: " << i << "->" << n->get_goal() << " ";
            std::vector<float> result = n->get_price_range();
            std::cout << "Price range: [" << result[0] << "," << result[1] << "] "; 
            std::cout << "Current price: " << n->get_price() << std::endl;
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
        for(Node* n:graph[i]){
            line.clear();
            std::vector<float> result = n->get_price_range();
            line += std::to_string(i) + " " + std::to_string(n->get_goal()) \
                 + " " + std::to_string(result[0]) + " " + std::to_string(result[1]);
            file << line;
            file << "\n";
        }
    }
    file.close();
};

void Map::update_price(){
    for(auto edge:graph){
        for(auto node:edge.second){
            node->update_price();
        }
    }
};