# System Programming Lab 11: Multiprocessing
## Edited by: Alex Toma
### Changes:

- Switched this program to run with multiprocessing using forks to divide image generation tasks among multiple children
- Made the program generate 50 images at once
- added a command line argument `-c` to specify the number of processes
- Made the images generated have different scales using `pow` from math.h
- Used `sprintf` for file generation with sequential numbering for frame outputs
- Provided an image of a plot of different processes and their runtimes found [here](https://github.com/MSOE-CPE2600/multiprocessing-turney-groovyalto/blob/main/children_vs_runtime_chart_lab11_systems_programming.png)
- Created a mpg movie that includes all the images into one video found [here](https://github.com/MSOE-CPE2600/multiprocessing-turney-groovyalto/blob/main/mandel.mpg)

# Systems Programming Lab 12 Multithreading
## Edited by: Alex Toma
### Changes:

- Added support for multithreading by incorporating `pthreads` to parallelize the computation of the Mandelbrot set across multiple threads
- Introduced a new struct `ThreadData` to pass computation parameters (e.g., start row, end row, image bounds) to each thread
- Added a `-t` command line argument to allow users to specify the number of threads for parallel computation *(max of 20)*
- Updated the `compute_image` function to divide the image rows evenly among the specified number of threads, create threads with `pthread_create`, and wait for their completion using `pthread_join`
- Provided an image of a plot of different processes and thread combinations and their runtimes found [here](https://github.com/MSOE-CPE2600/multiprocessing-turney-groovyalto/blob/main/children_vs_threads_runtimes_lab12_systems_programming.png)
