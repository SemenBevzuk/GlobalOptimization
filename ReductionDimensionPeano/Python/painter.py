import numpy as np
import matplotlib.pyplot as plt
import pandas
import os
import math

eps = 0.05
count_layers = 5;

cwd = os.getcwd()
print(cwd)
os.chdir('../log')
cwd = os.getcwd()
print(cwd)

file = open('function_points.csv', 'r');
s = file.readline()
array = s.split(';', 3)
array[3] = array[3].replace("\r\n", '')
x_a = int(array[0])
x_b = int(array[1])
y_a = int(array[2])
y_b = int(array[3])
for line in file:
    array_coord = line.split(';', 1)
    array_coord[1] = array_coord[1].replace("\n", '')
    plt.plot(array_coord[0], array_coord[1], 'go')

data = pandas.read_csv('function_lines.csv',delimiter=';',header = None)
temp = np.transpose(pandas.DataFrame(data,columns=[0,1,2]).as_matrix())
grid_x = temp[0]
grid_y = temp[1]
grid_z = temp[2]

plt.axis([x_a, x_b, y_a, y_b])

plt.tricontour(grid_x, grid_y, grid_z, 10, colors='black')

plt.xlabel(r'$x$')
plt.ylabel(r'$y$')
plt.grid(True)
plt.show()

os.chdir('../log/img')
cwd = os.getcwd()
plt.savefig('img_func.png', format = 'png')
print(cwd)

print('Done!')



