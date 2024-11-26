# System Programming Lab 11 Multiprocessing
## Edited by: Alex Toma
### Changes:

- Switched this program to run with multiprocessing using forks to divide image generation tasks among multiple children
- Made the program generate 50 images at once
- added a command line argument `-c` to specify the number of processes
- Made the images generated have different scales using `pow` from math.h
- Used `sprintf` for file generation with sequential numbering for frame outputs
- Provided an image of a plot of different processes and their runtimes found [here](https://github.com/MSOE-CPE2600/multiprocessing-turney-groovyalto/blob/main/children_vs_runtime_chart_lab11_systems_programming.png)
- Created a mpg movie that includes all the images into one video found [here](https://github.com/MSOE-CPE2600/multiprocessing-turney-groovyalto/blob/main/mandel.mpg)
