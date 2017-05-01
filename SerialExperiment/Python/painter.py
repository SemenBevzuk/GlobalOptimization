import numpy as np
import matplotlib.pyplot as plt
import os
import math

nf = '74'
eps = 0.05
count_layers = 10
count_func = 1
#while (count_func<101):
cwd = os.getcwd()
print(cwd)
os.chdir('../log')
cwd = os.getcwd()
print(cwd)

file = open('function_points_' + nf + '.csv', 'r');
for line in file:
    array_coord = line.split(';',1)
    array_coord[1] = array_coord[1].replace("\n", '')
    plt.plot(array_coord[0],array_coord[1], 'go')

file = open('function_lines_' + nf + '.csv', 'r');

test_x = []
test_y = []
test_z = []
result_x = []
result_y = []
lines = []

for line in file:
    array_coord = line.split(';',2)
    array_coord[2] = array_coord[2].replace("\n", '')
    test_x.append(array_coord[0])
    test_y.append(array_coord[1])
    test_z.append(float(array_coord[2]))

max_array = max(test_z)
min_array = min(test_z)
line_step = (float(max_array)-float(min_array))/count_layers
i = 0

while (i<count_layers):
    lines.append(i*line_step)
    i = i + 1;

count = 0

for i in test_z:
    for k in lines:
        if math.fabs(float(i)+float(k)) <eps:
            result_x.append(test_x[count])
            result_y.append(test_y[count])
    count = count + 1;

print('nf = ', nf)
print('max = ', max_array)
print('min = ', min_array)
print('line_step = ', line_step)

os.chdir('../log/img')
cwd = os.getcwd()
print(cwd)

plt.axis([0, 1, 0, 1])
plt.scatter(result_x,result_y, c='k', marker='o', s = 0.005)
plt.xlabel(r'$x$')
plt.ylabel(r'$y$')
plt.grid(True)
plt.show()

#plt.savefig('func_' + nf + '.png', format = 'png')
#os.chdir('../../Python')
#count_func = count_func + 1
#nf = str(count_func)
#plt.clf()

print('Done!')



