#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>
//#inlude "trap.c"
//#include "get_input.c"
//extern double result=0.0;

double Trap(double left_endpt,double right_endpt,int trap_count,double base_len);
int main(void){
  int my_rank , comm_sz, n =2000000 , local_n;
  /* printf("discrete point is : %d\n",n); */
  double a =0.0 , b=1.0, h, local_a,local_b;
  double local_int,total_int;
  int source;   
  double local_start,local_finish,local_elapsed,elapsed;
  MPI_Init(NULL,NULL);
  MPI_Barrier(MPI_COMM_WORLD);
  local_start=MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  printf("============== hello,world:process %d of %d ================\n",my_rank,comm_sz);
  h = (b-a)/n;
  local_n = n/comm_sz;
  
  //if(my_rank==comm_sz-1)
    //local_n=n-local_n*(comm_sz-1);
  double temp=0.0;
  local_a = a + my_rank * local_n * h;
  local_b = local_a +local_n * h;
  //double result=0.0;
  local_int=Trap(local_a , local_b , local_n, h);
  printf("With n = %d trapezoids, our estimate\n",local_n);
  printf("integral from %f to %f = %.11f\n",local_a,local_b,local_int);
    
  /* total_int+=local_int; */
  //printf("%d of %d process,result=%.15e\n",my_rank,comm_sz,result);
  if (my_rank != 0){
    printf("xxxx\n");
    MPI_Send(&local_int , 1 , MPI_DOUBLE , 0, 0 , MPI_COMM_WORLD);
    /* printf("With n = %d trapezoids, our estimate\n",local_n); */
    /* printf("integral from %f to %f = %.11f\n",local_a,local_b,local_int); */
    
  }else{
    total_int = local_int;
    for(source = 1 ; source < comm_sz ; source++){
      MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      /* total_int += local_int; */
      total_int+=local_int;
    }
  }
  local_finish=MPI_Wtime();
  local_elapsed=local_finish-local_start;
  MPI_Reduce(&local_elapsed,&elapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

  if (my_rank == 0 ){
    printf("\n=======================RESULT=========================\n");
    printf("With n = %d trapezoids, our estimate\n",n);
    printf("of the integral from %f to %f = %.11f\n",a,b,total_int);
    printf("error=%f\n",total_int-3.14159265359);
    printf("elapsed time=%e second\n",elapsed);
    printf("======================================================\n");
  }
  printf("xxxx\n");
  MPI_Finalize();
  return 0;
}               
