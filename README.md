# Parallel_Algorithms
<p float="left">
<img src="https://img.shields.io/badge/language-C-red" alt="alt text">
<img src="https://img.shields.io/badge/license-MIT-green" alt="alt text">
</p>

Parallel Algorithms - course for 3-year students of the DCAM MIPT, where students learn the basics of parallel programming using the MPI and openMP libraries. 

## Integral with Precision, OpenMP

### Overview

In this problem, the trapezoid method is used to calculate the value of the integral with a given precision.

OpenMP with the task scheduling is used for acceleration, since the trapezoid method is recursive. 

Used [function](https://www.wolframalpha.com/input/?i=integrate+1%2Fx%5E2+sin%281%2Fx%29%5E2+from+0.0001+to+1):

<img src="https://latex.codecogs.com/gif.latex?\int_{0.0001}^{1}&space;\frac{sin^2&space;(\frac{1}{x^2})&space;}{x^2}dx&space;=&space;4999.58" />
 
Tested on Macbook Pro 13 / 2,3 GHz Quad-Core Intel Core i5 - 4 Cores.
 
### Usage
Ready-to-use for MacOS.

Clone a repository on your machine. When move to the [Integral_OpenMP_with_Precision](https://github.com/MeneTelk0/Parallel_Algorithms/tree/master/Integral_OpenMP_with_Precision) directory and run: 
````bash
chmod +x build.sh
./build.sh 0.000001 
````
It will automatically generate Acceleration.png and Efficiency.png plots and store it in the [res](https://github.com/MeneTelk0/Parallel_Algorithms/tree/master/Integral_OpenMP_with_Precision/res) folder.

You can change 0.000001 to any precision you want.

### Results
This program accelerates up to 4 times when using 4 cores of the system.  
![Acceleration](https://github.com/MeneTelk0/Parallel_Algorithms/blob/master/Integral_OpenMP_with_Precision/res/acceleration.png)
Efficiency of each process slightly reduces with increase of number of threads.
![Efficiency](https://github.com/MeneTelk0/Parallel_Algorithms/blob/master/Integral_OpenMP_with_Precision/res/efficiency.png)
