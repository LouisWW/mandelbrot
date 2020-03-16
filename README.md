Generate a mandelbrot plot in C
-----

The mandelbrot.c computes the mandebrot set given the coordinates in the defined variable. The resolution is given in line 95.
The file need to be complied first before being run

  gcc mandelbrot.c -o output_file
  ./output_file
  
which will save a plot in the same directory. For detailed picture, the omp.h library is incorporated to allow parallel computation by using
 
  gcc mandebrot.c  -fopenmp -o output_file
  ./output_file

Thus higher column size and resolution can be used.
