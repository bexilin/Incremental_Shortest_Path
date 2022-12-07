import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
import numpy as np
import csv

def matrix_from_file(num_point,p):
    dir = "n_"+str(num_point)+"_p_"+f"{p:.1f}/"

    m = np.zeros((num_point,num_point))
    
    count = 0
    with open(root+dir+"inc_affected_pairs.csv","r") as affected_f:
        csvreader = csv.reader(affected_f,delimiter=' ')
        for row in csvreader:
            if np.abs(m[int(row[0]),int(row[1])]) < 1e-03:
                m[int(row[0]),int(row[1])] = 1.0
                count += 1

    with open(root+dir+"new_edges.csv","r") as new_edge_f:
        csvreader = csv.reader(new_edge_f,delimiter=' ')
        for row in csvreader:
            m[int(row[0]),int(row[1])] = 2.0

    return m, count

def matrix_from_file_2(num_point,p):
    dir = "n_"+str(num_point)+"_p_"+f"{p:.1f}/"

    m = np.zeros((num_point,num_point))
    
    with open(root+dir+"current_edges.csv","r") as edge_f:
        csvreader = csv.reader(edge_f,delimiter=' ')
        for row in csvreader:
            m[int(row[0]),int(row[1])] = 1.0

    return m

if __name__ == "__main__":

    root = "../build/experiment_data/"
    num_points = [10,50,100,500,1000]
    portion = [0.2,0.5,0.8]

    # incremental solver matrix plots
    cmap = ListedColormap(['w','b','r'])
    cmap_2 = ListedColormap(['w','grey'])
    fig_1 = plt.figure(figsize=(18,18))
    fig_2 = plt.figure(figsize=(18,6))
    spec = fig_1.add_gridspec(3,3)
    spec_2 = fig_2.add_gridspec(1,3)
    percentage = np.zeros((len(num_points),len(portion)))
    for i in range(5):
        for j in range(3):
            m,count = matrix_from_file(num_points[i],portion[j])
            percentage[i,j] = count/(num_points[i]**2-num_points[i]) * 100

            if i < 3:
                ax = fig_1.add_subplot(spec[i,j])
                ax.matshow(m,cmap=cmap)
                ax.set_xticks([])
                ax.set_yticks([])
                if i == 0:
                    ax.set_title("p = "+f"{portion[j]*100:.0f}"+"%",fontsize=15)
                if j == 0:
                    ax.set_ylabel("n = "+str(num_points[i]),fontsize=15,rotation=0,labelpad=35)

            if i == 1:
                m_2 = matrix_from_file_2(num_points[i],portion[j])
                ax = fig_2.add_subplot(spec_2[0,j])
                ax.matshow(m_2,cmap=cmap_2)
                ax.set_title("p = "+f"{portion[j]*100:.0f}"+"%",fontsize=15)
                ax.set_xticks([])
                ax.set_yticks([])
                if j == 0:
                    ax.set_ylabel("n = "+str(num_points[i]),fontsize=15,rotation=0,labelpad=35)

    # incremental solver proportion of affected pairs plot
    fig_3,ax = plt.subplots()
    colors = ['r','g','b']
    for i in range(3):
        ax.plot(np.array(num_points),percentage[:,i],marker='o',color=colors[i], \
                label="p = "+f"{portion[i]*100:.0f}"+"%")
    ax.legend(fontsize=15)
    ax.set_xlabel("Number of map points",fontsize=15)
    ax.set_ylabel("Percentage",fontsize=15)
    ax.xaxis.set_tick_params(labelsize=15)
    ax.yaxis.set_tick_params(labelsize=15)
    ax.set_title("Proportion of affected pairs",fontsize=15)


    # computation time comparison plots
    t_inc = np.zeros((len(num_points),len(portion)))
    t_fw = np.zeros((len(num_points),len(portion)))
    idx_pairs = [(x,y) for x in range(len(num_points)) for y in range(len(portion))]

    with open(root+"times.csv","r") as t_file:
        csvreader = csv.reader(t_file,delimiter=' ')
        for idx, row in enumerate(csvreader):
            x,y = idx_pairs[idx]
            t_inc[x,y] = float(row[-2])
            t_fw[x,y] = float(row[-1])

    fig_4,ax_2 = plt.subplots()
    ax_2.plot(np.array(num_points),t_inc[:,0],marker='o',color='b',label='Incrmental solver') 
    ax_2.plot(np.array(num_points),t_fw[:,0],marker='o',color='r',label='Floyd Warshall solver')
    ax_2.legend(fontsize=15)
    ax_2.set_xlabel("Number of map points",fontsize=15)
    ax_2.set_ylabel("Time / s",fontsize=15)
    ax_2.xaxis.set_tick_params(labelsize=15)
    ax_2.yaxis.set_tick_params(labelsize=15)
    ax_2.set_title("Computation time of two methods",fontsize=15)

    plt.show()

