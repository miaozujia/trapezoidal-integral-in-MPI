#include<stdio.h>
#include<math.h>
#include"mpi.h"
 
double f(double x) {
	return 1/(x*x+1);
}
 
double trap(double a, double b, int n) {
	/*梯形积分法的实现 a,b 为积分的上下限， n为细分的区间数目*/
	double h = (b - a) / n;
	double sum = (f(a) + f(b)) / 2;
	for (int i = 1; i < n; i++) {
		sum += f(a + i * h);
	}
	return sum * h;
}
 
void Get_input(int my_rank, int comm_sz, double* a, double* b, int* n) {
    /*用集合通信的方式实现*/
	//printf("Enter a, b, and n \n");
	//fflush(stdout);
	// scanf_s("%lf %lf %d", a, b, n);
    *a=0.0;
    *b=1.0;
	/*将主进程输入的参数广播到其余进程*/
	MPI_Bcast(a, 1, MPI_DOUBLE, my_rank, MPI_COMM_WORLD);
	MPI_Bcast(b, 1, MPI_DOUBLE, my_rank, MPI_COMM_WORLD);
	MPI_Bcast(n, 1, MPI_INT, my_rank, MPI_COMM_WORLD);
}
 
int main() {
	int my_rank, comm_sz;	//当前进程的编号，所有进程的数目
	double a, b;	//积分的上下限
	int n;	//区间的宽度
 
	/*并行部分开始*/
	MPI_Init(NULL,NULL);	
 
	// 获取当前进程的编号，所有进程的数目
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 
	Get_input(my_rank, comm_sz, &a, &b, &n);
	
	// 初始化当前进程的积分要素
	int local_n = n / comm_sz;
	double local_a = a + my_rank * (b - a) / comm_sz;
	double local_b = local_a + (b - a) / comm_sz;
	
	//计算当前进程的积分值
	double local_sum = trap(local_a, local_b, local_n);
	//临时变量，用来存放其余进程发送给主进程的计算结果
	double cacha_sum;
 
	/*集合通信，将各个进程的local_sum求和后送至0号进程的cacha_sum中*/
	MPI_Reduce(&local_sum, &cacha_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
 
	//主进程将计算结果输出
	if (my_rank == 0) {
		printf("With n=%d trapeziods, our estimate\n", n);
		printf("of the integral from %f to %f = %.15e\n", a, b, cacha_sum);
	}
 
	/*并行部分结束*/
	MPI_Finalize();
 
	return 0;
}
