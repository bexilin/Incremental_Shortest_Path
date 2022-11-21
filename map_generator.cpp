// Author: Xi Lin
#include "map_generator.hpp"
#include "nanoflann.hpp"
#include <fstream>
#include <random>
#include <cmath>

float distance(PointCloud::Point& a, PointCloud::Point& b){
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx*dx + dy*dy);
};

Map_generator::Map_generator() {
    gen = std::mt19937(seed);
    uniform_dis_x = std::uniform_real_distribution<float>(0.0,width);
    uniform_dis_y = std::uniform_real_distribution<float>(0.0,height);
    binomial_dis = std::binomial_distribution<int>(1,p_l);
};

void Map_generator::create_map() {
    std::cout << "Creating a map with " << num_points << " cities" << std::endl;

    // Use KD Tree to identify the closest neighbour
    using kd_tree = nanoflann::KDTreeSingleIndexDynamicAdaptor<
            nanoflann::L2_Simple_Adaptor<float, PointCloud>,
            PointCloud, 2>;

    kd_tree tree(2,m.pc);

    // Create random map points
    while(m.pc.points.size() < num_points){
        float sample[2] = {uniform_dis_x(gen), uniform_dis_y(gen)};
        if(m.pc.points.empty()){
            m.pc.points.push_back(PointCloud::Point(sample[0],sample[1]));
            tree.addPoints(0,0);
            continue;
        }
        nanoflann::KNNResultSet<float> resultSet(1);
        size_t idx;
        float dis;
        resultSet.init(&idx,&dis);
        tree.findNeighbors(resultSet,sample,nanoflann::SearchParams());

        // std::cout << "new idx:" << m.pc.points.size() << std::endl;
        // std::cout << "idx: " << idx << " dis: " << dis << std::endl;
        // std::cout << m.pc.points[idx].x << " " << m.pc.points[idx].y << std::endl;
        // std::cout << sample[0] << " " << sample[1] << std::endl;
        // float dx = m.pc.points[idx].x - sample[0];
        // float dy = m.pc.points[idx].y - sample[1];
        // std::cout << sqrt(dx*dx + dy*dy) << std::endl << std::endl;

        // Abandon the sample if the closet neighbour is within min_dis units
        if(std::sqrt(dis) < min_dis) continue;

        size_t i = m.pc.points.size();
        m.pc.points.push_back(PointCloud::Point(sample[0],sample[1]));
        tree.addPoints(i,i);
    }

    // Create random links between map points
    std::vector<Map::Node*> row(num_points,nullptr);
    m.graph = std::vector<std::vector<Map::Node*>>(num_points,row);
    for(uint32_t i = 0; i < num_points-1; i++){
        for(uint32_t j = i+1; j < num_points; j++){
            if(binomial_dis(gen) == 0){
                m.potential_edges.push_back(i*num_points+j);
                continue;
            } 

            float base_price = distance(m.pc.points[i],m.pc.points[j]) * p;
            float low = base_price*(1-r);
            float high = base_price*(1+r);
            
            m.graph[i][j] = new Map::Node(low,high,m.seed++);
            m.graph[j][i] = new Map::Node(low,high,m.seed++);
        }
    }

    std::cout << "Finished!" << std::endl << std::endl;
};

void Map_generator::insert_new_edges(){
    if(m.potential_edges.empty()) throw std::runtime_error("Potential_edges is empty!");
    
    m.most_recent_new_edges.clear();

    // number of newly created edges
    std::uniform_int_distribution<int> num_dis(1,std::min(num_new_edge,int(m.potential_edges.size())));
    int num = num_dis(gen);

    int pc_num = m.map_points_num();
    for(int k = 0; k < num; k++){
        std::uniform_real_distribution<int> idx_dis(0,int(m.potential_edges.size()-1));
        int rand_idx = idx_dis(gen);
        int edge_idx = m.potential_edges[rand_idx];
        m.potential_edges.erase(m.potential_edges.begin()+rand_idx);
        
        int i = edge_idx / pc_num, j = edge_idx % pc_num;
        if(m.graph[i][j] != nullptr) 
            throw std::runtime_error("Edge "+std::to_string(i)+"->"+std::to_string(j)+" already exists!");
        
        float base_price = distance(m.pc.points[i],m.pc.points[j]) * p;
        float low = base_price*(1-r);
        float high = base_price*(1+r);
        
        m.graph[i][j] = new Map::Node(low,high,m.seed++);
        m.graph[j][i] = new Map::Node(low,high,m.seed++);

        m.most_recent_new_edges.push_back(i*pc_num+j);
        m.most_recent_new_edges.push_back(j*pc_num+i);
    }
};