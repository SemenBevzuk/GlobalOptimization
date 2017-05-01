import numpy as np
import matplotlib.pyplot as plt
import os
import math

def f_0_1(x):
    return np.sqrt(2.25-np.power(x-2.2, 2))+1.2
def f_0_2(x):
    return -np.sqrt(2.25-np.power(x-2.2, 2))+1.2

def f_1_1(x):
    return 2*np.sqrt(1-np.power(x-2, 2)/1.44)
def f_1_2(x):
    return -2*np.sqrt(1-np.power(x-2, 2)/1.44)

def f_2(x):
    return 1.5+1.5*np.sin(6.283*(x-1.75))

eps = 0.05
count_layers = 10;
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

test_x = []
test_y = []
test_z = []
result_x = []
result_y = []
lines = []
file = open('function_lines.csv', 'r');
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
plt.scatter(result_x,result_y, c='k', marker='o', s = 0.005)

x = np.linspace(x_a, x_b, 5000)
y = f_0_1(x)
plt.plot(x,y,'r')
y = f_0_2(x)
plt.plot(x,y, 'r')

y = f_1_1(x)
plt.plot(x,y,'b')
y = f_1_2(x)
plt.plot(x,y, 'b')

y = f_2(x)
plt.plot(x,y, 'm')

plt.grid(True)
plt.axis([x_a, x_b, y_a, y_b])
os.chdir('../log/img')
cwd = os.getcwd()
print(cwd)
plt.savefig('img_func_1.png', format = 'png')
plt.show()

print('Done!')


