# trapezoidal-integral-in-MPI
perform MPI parallel computing to realize trapezoidal integration
# 1, Realizing trapezoidal integration method

We can use the integral method to estimate the size of the interval between two vertical lines and the x-axis in the image of the function
![image](https://user-images.githubusercontent.com/57306138/166096691-54cd253d-4538-4afa-926e-c0f67f5159a3.png)

Basic idea: divide the interval on the x axis into N equal length sub intervals. Then, the area of the trapezoidal region between the function image and each sub interval is estimated. The bottom edge of the trapezoid is the sub interval of the x axis, and the two vertical edges are the end vertical lines passing through the sub interval.

So the area of the trapezoid：
![image](https://user-images.githubusercontent.com/57306138/166096757-8d3bb4a3-5c68-4ad0-8cbb-036e3f0ca167.png)

Because n sub intervals are equally divided, if the boundaries of the area surrounded by two vertical lines are x = a,  and x = b respectively.
Therefore, if the leftmost endpoint is called x0, the rightmost endpoint is xn, then:
![image](https://user-images.githubusercontent.com/57306138/166096785-aa6dadc5-e27d-43f4-8b87-d174d14c70a9.png)
Then the area of the whole area is:
![image](https://user-images.githubusercontent.com/57306138/166096798-b659301c-7fe4-455e-9191-d596b8feec04.png)
![image](https://user-images.githubusercontent.com/57306138/166096802-0afe8e8e-d401-4055-82f7-d52d1c2613a0.png)
Therefore, the pseudo code of a serial program is:
```
/*Input a, b, n*/
h = (b - a)/n;
approx = (f(a) + f(b))/2.0;
for(i = 1;i <= n-1; i++) {
    x_i = a + i * h;
    approx += f(x_i);
}
approx = h * approx;

```

The pseudo code of the program is as follows:
```
Get a,b,n;
h = (b-a) / n;
local_n = n / comm_sz;
local_a = a + my_rank * local_n * h;
local_b = local_a + local_n * h;
local_integral = Trap(local_a, local_b, local_n, h);
if(my_rank != 0) {
    Send local_integral to process 0;
}
else {
    total_integral = local_integral;
    for( proc = 1; proc < comm_sz; proc++) {
        Receive lcoal_integral from proc;
        total_integral += local_integral;
    }
}
if(my_rank == 0) {
    print result;
}

```
# 2,  Project Environment
gcc,g++,mpi
To run it, use CMake:
```

cd build 
cmake ..
make
mpicc -o main homework.c
mpirun -np 4 main
```
