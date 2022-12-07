import matplotlib as mpl
import matplotlib.pyplot as plt
import csv
import numpy as np

class Map:

    def __init__(self,map_file,price_file):

        self.vertices = []
        self.edges = []
        self.airline_price = []
        self.plot_handle = None

        with open(map_file,'r') as csvfile:
            csvreader = csv.reader(csvfile,delimiter=' ')
            last = []
            record_vertex = False
            record_edge = False
            for row in csvreader:
                if len(row) == 0:
                    last = row
                    continue
                if len(row) == 1:
                    if row[0] == 'Vertices:':
                        record_vertex = True
                    if row[0] == 'Edges:':
                        record_vertex = False
                        record_edge = True
                    last = row
                    continue

                if record_vertex:
                    self.vertices.append([int(row[0]),float(row[1]),float(row[2])])
                if record_edge:
                    self.edges.append([int(row[0]),int(row[1]),float(row[2]),float(row[3])])
                
                last = row
            
        with open(price_file,'r') as csvfile:
            csvreader = csv.reader(csvfile,delimiter=' ')
            for row in csvreader:
                self.airline_price.append([float(x) for x in row])

    def visualize_topology(self,ax):

        label = True
        for v in self.vertices:
            if label:
                ax.scatter(v[1],v[2],s=20,c='r',zorder=3,label='city')
                label = False
            else:
                ax.scatter(v[1],v[2],s=20,c='r',zorder=3)
            # ax.text(v[1],v[2],str(v[0]),fontsize=15)

        label = True
        for e in self.edges:
            idx_1 = e[0]
            idx_2 = e[1]
            if label:
                ax.plot([self.vertices[idx_1][1],self.vertices[idx_2][1]], \
                        [self.vertices[idx_1][2],self.vertices[idx_2][2]],'k', \
                        linewidth=0.2, label='airline')
                label = False
            else:
                ax.plot([self.vertices[idx_1][1],self.vertices[idx_2][1]], \
                        [self.vertices[idx_1][2],self.vertices[idx_2][2]],'k', \
                        linewidth=0.2)

        ax.set_xticks([])
        ax.set_yticks([])
        ax.spines["left"].set_visible(False)
        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)
        ax.spines["bottom"].set_visible(False)

    def visualize_path(self,path,ax_graph,ax_route):
        if self.plot_handle != None:
            for h in self.plot_handle:
                h.remove()

        self.plot_handle = []
        label = True
        
        h = ax_route.text(0,0,"Route from city "+str(path[0])+" to city "+str(path[-1]),fontsize=20)
        ax_route.set_ylim([-1.0*(len(path)+1),1.0])
        self.plot_handle.append(h)
        total_cost = 0
        
        for i in range(len(path)-1):
            idx_1 = path[i]
            idx_2 = path[i+1]
            x = self.vertices[idx_1][1]
            y = self.vertices[idx_1][2]
            dx = self.vertices[idx_2][1] - x
            dy = self.vertices[idx_2][2] - y
            if label:
                h = ax_graph.arrow(x,y,dx,dy,color='b',label='path', \
                             head_width = 40, \
                             head_length = 40, \
                             length_includes_head=True, \
                             zorder=5)
                label = False
            else:
                h = ax_graph.arrow(x,y,dx,dy,color='b', \
                             head_width = 40, \
                             head_length = 40, \
                             length_includes_head=True, \
                             zorder=5)

            self.plot_handle.append(h)

            # route information
            step_cost = self.airline_price[idx_1][idx_2]
            total_cost += step_cost
            h = ax_route.text(0,-1.0*(i+1),"Step "+str(i+1)+": "+str(idx_1)+"->"\
                                           +str(idx_2)+"   Price: "+f"{step_cost:.2f}",\
                                           fontsize=15)

            self.plot_handle.append(h)

        h = ax_route.text(0,-1.0*(len(path)),"Total cost: "+f"{total_cost:.2f}",fontsize=15)
        self.plot_handle.append(h)
        
        ax_route.set_xticks([])
        ax_route.set_yticks([])
        ax_route.spines["left"].set_visible(False)
        ax_route.spines["top"].set_visible(False)
        ax_route.spines["right"].set_visible(False)
        ax_route.spines["bottom"].set_visible(False)

class Floyd_Warshall:

    def __init__(self,path_map_file,price_map_file):
        
        self.path_map = []
        self.price_map = []
        self.plot_handle = None
        
        with open(path_map_file,'r') as csvfile:
            csvreader = csv.reader(csvfile,delimiter=' ')
            for row in csvreader:
                self.path_map.append([int(x) for x in row])
        
        with open(price_map_file,'r') as csvfile:
            csvreader = csv.reader(csvfile,delimiter=' ')
            for row in csvreader:
                self.price_map.append([float(x) for x in row])

    def query(self,start,end):

        # get the path from start to end
        curr = start
        path = [curr]
        while curr != end:
            curr = self.path_map[curr][end]
            path.append(curr)
        
        return path

class Incrmental(Floyd_Warshall):

    def __init__(self,path_map_file,price_map_file):
        super().__init__(path_map_file,price_map_file)

    def query(self, start, end):
        return super().query(start, end)

if __name__ == "__main__":
    
    plot = plt.figure(figsize=(12,6))
    spec = plot.add_gridspec(1,2)
    ax_graph = plot.add_subplot(spec[0,0])
    ax_route = plot.add_subplot(spec[0,1])

    m = Map('../build/experiment_data/n_50_p_0.2/map.csv',
            '../build/experiment_data/n_50_p_0.2/current_price.csv')

    fw = Floyd_Warshall('../build/experiment_data/n_50_p_0.2/fw_path_map.csv',
                        '../build/experiment_data/n_50_p_0.2/fw_price_map.csv')

    m.visualize_topology(ax_graph)

    start = 0
    end = 47
    
    path = fw.query(start,end)

    m.visualize_path(path,ax_graph,ax_route)
    
    plot.savefig("query_example.png",bbox_inches='tight')




    
