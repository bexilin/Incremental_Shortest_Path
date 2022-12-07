import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
import numpy as np
import csv

def matrix_from_file(num_point,p):
    root = "../build/experiment_data/"
    dir = "n_"+str(num_point)+"_p_"+f"{p:.1f}/"

    m = np.zeros((num_point,num_point))

    # with open(root+dir+"current_edges.csv","r") as edge_f:
    #     csvreader = csv.reader(edge_f,delimiter=' ')
    #     for row in csvreader:
    #         m[int(row[0]),int(row[1])] = 1.0
    
    with open(root+dir+"inc_affected_pairs.csv","r") as affected_f:
        csvreader = csv.reader(affected_f,delimiter=' ')
        for row in csvreader:
            m[int(row[0]),int(row[1])] = 1.0

    with open(root+dir+"new_edges.csv","r") as new_edge_f:
        csvreader = csv.reader(new_edge_f,delimiter=' ')
        for row in csvreader:
            m[int(row[0]),int(row[1])] = 2.0

    return m


if __name__ == "__main__":

    num_points = [10,100,1000]
    portion = [0.2,0.5,0.8]

    ### incremental solver plots
    cmap = ListedColormap(['w','b','r'])
    fig_1 = plt.figure(figsize=(18,18))
    spec = fig_1.add_gridspec(3,3)
    for i in range(3):
        for j in range(3):
            if i == 2 and j == 2:
                continue
            ax = fig_1.add_subplot(spec[i,j])
            ax.matshow(matrix_from_file(num_points[i],portion[j]),cmap=cmap)

    plt.show()

