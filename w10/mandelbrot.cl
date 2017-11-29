__kernel void mandelbrot(__global const double *xmin, __global const double *xmax, __global const double *ymin, __global const double *ymax, __global const int *n, __global const int *max_iter, __global int *mandelbrot)
{
    double pixelWidth = (*xmax-*xmin)/ (double)*n;
    double pixelHeight = (*ymax-*ymin)/ (double)*n;
    
    int npix = get_global_id(0);
    //double ypix = get_global_id(1);
    
    double xpix = (npix/ *n);
    double ypix = (double)(npix % *n);
    //printf("xpix  %lf\n", xpix);
    //printf("ypix  %lf\n", ypix);
    double xc = *xmin + xpix * pixelWidth;
    double yc = *ymin + ypix * pixelHeight;
    
    int it;
    double x,y;
    x =  y = (double)0;
    
    for (it = 0; it < *max_iter; it++)
    {
        double x2 = x*x;
        double y2 = y*y;
        if(x2 + y2 > (double)4)
        {
            break;
        }
                
        double twoxy = (double)2 * x * y;
        x = x2 - y2 + xc;
        y = twoxy + yc;
    }
    printf("%lf %lf %d\n",xc, yc, it);
    
    //mandelbrot[((xpix* *n) + ypix)] = it;
}

