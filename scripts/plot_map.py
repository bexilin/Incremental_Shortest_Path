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
                ax.scatter(v[1],v[2],s=20,c='b',zorder=3,label='city')
                label = False
            else:
                ax.scatter(v[1],v[2],s=20,c='b',zorder=3)
            ax.text(v[1],v[2],str(v[0]),fontsize=15)

        label = True
        for e in self.edges:
            idx_1 = e[0]
            idx_2 = e[1]
            if label:
                ax.plot([self.vertices[idx_1][1],self.vertices[idx_2][1]], \
                        [self.vertices[idx_1][2],self.vertices[idx_2][2]],'k',label='airline')
                label = False
            else:
                ax.plot([self.vertices[idx_1][1],self.vertices[idx_2][1]], \
                        [self.vertices[idx_1][2],self.vertices[idx_2][2]],'k')

    def visualize_path(self,path,ax):
        if self.plot_handle != None:
            for h in self.plot_handle:
                h.remove()

        self.plot_handle = []
        label = True
        for i in range(len(path)-1):
            idx_1 = path[i]
            idx_2 = path[i+1]
            x = self.vertices[idx_1][1]
            y = self.vertices[idx_1][2]
            dx = self.vertices[idx_2][1] - x
            dy = self.vertices[idx_2][2] - y
            if label:
                h = ax.arrow(x,y,dx,dy,color='r',label='path', \
                             head_width = 40, \
                             head_length = 40, \
                             length_includes_head=True, \
                             zorder=5)
                label = False
            else:
                h = ax.arrow(x,y,dx,dy,color='r', \
                             head_width = 40, \
                             head_length = 40, \
                             length_includes_head=True, \
                             zorder=5)

            self.plot_handle.append(h)


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
        
        return path, self.price_map[start][end]


if __name__ == "__main__":
    
    plot, ax = plt.subplots()

    m = Map('../build/map.csv','../build/current_price.csv')

    fw = Floyd_Warshall('../build/fw_path_map.csv','../build/fw_price_map.csv')

    m.visualize_topology(ax)

    start = 0
    end = 7
    
    path, total_cost = fw.query(start,end)

    print("Route from city ",start," to city ",end)

    for i in range(len(path)-1):
        idx_1 = path[i]
        idx_2 = path[i+1]
        step_cost = m.airline_price[idx_1][idx_2]
        print("Step ",i+1,": ",idx_1,"->",idx_2," price: ",step_cost)
    
    print("Total cost: ",total_cost)

    m.visualize_path(path,ax)
    
    ax.legend()
    ax.set_aspect("equal")
    ax.set_title("Map")
    plt.show()




    
