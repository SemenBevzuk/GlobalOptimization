#include <stdio.h>
#include "vagris.h"

int main()
{
 int nf;
 double dx1, dx2;
 char filename[12];
 FILE *fp;


 for (nf=1; nf <= 100; nf++)
 {

 set_random(nf);

 sprintf(filename,"vagris%03d",nf);
 if ((fp=fopen(filename,"wt")) == NULL) return 1;

 printf("\nFunction number %d",nf);
 printf("\nIts global minimizer is (%lf, %lf)",
        rand_minimums[(nf-1)*2], rand_minimums[(nf-1)*2+1]);
 printf("\nIts global minimum value is %lf",
        random_func(rand_minimums[(nf-1)*2], rand_minimums[(nf-1)*2+1]));
 printf("\nComputing function...\n");
    for (dx1=0.0; dx1<=1.001; dx1+=0.01)
		for (dx2=0.0; dx2<=1.001; dx2+=0.01)
          {
            fprintf(fp,"%lf %lf %lf\n", dx1, dx2,
				    random_func(dx1, dx2));
			        /* evaluate function nf in (dx1, dx2) */
          }
 fclose(fp);
 } /* for nf*/

 return 0;
}
