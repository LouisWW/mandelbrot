

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <time.h>
#include <omp.h>


#define COL 1202
#define xmin_coordinates -0.13856524454488
#define xmax_coordinates -0.13956524454488
#define ymin_coordinates -0.64935990748190
#define ymax_coordinates -0.65035990748190
#define GNUPLOT "gnuplot -persist"

typedef struct 
{
   int height;
   int  width;
   int n_iteration;

   float xmin;
   float xmax;

   float ymin;
   float ymax;

   float resolutoin;
}mandelbrot;



int complex_cal(float x,float y, int iteration){

  double complex z=x+y*I;
  double complex z_1=0+0*I;
  
 
  for(int i=0;i<iteration;i++){

    if (cabs(z_1)>2){
        return i ;
       }

    z_1=z_1*z_1+z;
    
  }
   return iteration+1;
}

void linspace(float a, float b, int n, float u[])
{
    float c;
    int i;
    
   
    /* step size */
    c = (b - a)/(n - 1);
    
    /* fill vector */
    #pragma omp parallel for
    for(i = 0; i < n - 1; ++i)
        u[i] = a + i*c;
    
    /* fix last entry to b */
    u[n - 1] = b;
    
    /* done */
 
}


void init_mandelbrot(int matrix[][COL],int width, int height,float x_span[],float y_span[], int iteration){

#pragma omp parallel for
for (int i=0;i< height; i++){

    for(int k=0;k<width;k++){
    matrix[i][k]= complex_cal(x_span[i],y_span[k],iteration);

    }
}

}





 int main(){

    clock_t begin = clock();

    mandelbrot mandelbrot1={COL,COL,1024,xmin_coordinates,xmax_coordinates,ymin_coordinates,ymax_coordinates,0.65+1.1875/(COL)};
    float arr_height[mandelbrot1.height];
    float arr_width[mandelbrot1.width];
    linspace(mandelbrot1.xmin,mandelbrot1.xmax,mandelbrot1.width,arr_height);
    linspace(mandelbrot1.ymin,mandelbrot1.ymax,mandelbrot1.height,arr_width);

    // needs to be static bc takes to much space in main  
    static int matrix[COL][COL];

    init_mandelbrot(matrix,mandelbrot1.width,mandelbrot1.height,arr_height,arr_width,mandelbrot1.n_iteration);


    FILE *gp = popen(GNUPLOT, "w");

    fprintf(gp," set pm3d map \n \
            set format x \"\" \n   \
            set format x \"\" \n \
            unset colorbox \n \
            unset border \n \
            unset xtics \n \
            unset ytics \n \
            unset xlabel \n \
            set terminal png enhanced size 15500,15500 \n \
            set output 'mandelbrot.png' \n \
            splot '-' matrix \n ");
    int i, j;
    for (i = 0; i < mandelbrot1.height; i++) {
        for (j = 0; j < mandelbrot1.width; j++)
           fprintf(gp, "%d ", matrix[i][j]);
        fprintf(gp, "\n");
    }
    pclose(gp);


    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("The running time is %f \n  s", time_spent);
}
