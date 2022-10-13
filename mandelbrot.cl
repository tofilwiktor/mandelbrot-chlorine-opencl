__kernel void solve_mandelbrot(__global float const * real,
                               __global float const * imag,
                               int iterations,
                               __global int * result)
{
    unsigned int i = get_global_id(0);

    float x = real[i]; 
    float y = imag[i]; 
    int   n = 0;      

    while ((x * x + y * y <= 2 * 2) && n < iterations)
    {
        float xtemp = x * x - y * y + real[i];
        y = 2 * x * y + imag[i];
        x = xtemp;
        n++;
    }

    result[i] = x * x + y * y <= 2 * 2 ? -1 : n;
}
