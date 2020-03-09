#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gsl/gsl_rng.h>

#define Nx 16
#define Ny 16
#define Ti 1.0 
#define Tf 4.0 
#define DT 0.1 
#define MC 1e6
#define NT 1e4 
#define K_B 1 


 void output(double T, int *rede){
   int x, y;
   FILE *file;
   char name[100];
   sprintf(name, "Temperatura-%g.dat", T);
   file= fopen(name, "w");
   for(x= 0; x< Nx; x++){
     for(y= 0; y< Ny; y++){
       fprintf(file, "%d ", rede[x*Ny+y]);
     }
     fprintf(file, "\n");
   }
   fclose(file);
 }


int main(int argc, char **argv){
	int i, j, k, l, ip1, im1, jp1, jm1;
	int *rede;
	double E, M , E1, E2, M1, M2, T, DE;

	
	gsl_rng_default_seed= (argc == 2) ? atoi(argv[1]) : time(NULL);
	gsl_rng *w= gsl_rng_alloc(gsl_rng_taus);	

	rede= (int *) calloc(Nx*Ny, sizeof(int));

	for(i= 0; i< Nx; i++){
		for(j= 0; j< Ny; j++){
			rede[i*Ny+j]= (gsl_rng_uniform(w) < 0.5) ? 1 : -1;
		}
	}
	output(100,rede);

	for(T= Ti; T< Tf; T+= DT){
		for(k= 0; k< NT; k++){
			i= gsl_rng_uniform(w)*Nx;
			j= gsl_rng_uniform(w)*Ny;
			ip1= (i+1)%Nx;
			im1= (i-1+Nx)%Nx;
			jp1= (j+1)%Ny;
			jm1= (j-1+Ny)%Ny;
			DE= 2*rede[i*Ny+j]*(rede[ip1*Ny+j] + rede[im1*Ny+j] + 
                          rede[i*Ny+jp1] + rede[i*Ny+jm1]);
			if(DE<=0){
        rede[i*Ny+j]*=-1;
      }else{
        if(gsl_rng_uniform(w)< exp(-DE/(K_B*T))){
          rede[i*Ny+j]*=-1;
        }
      }

		}
		E= 0.0;
		M= 0.0;
		for(i= 0; i< Nx; i++){
			for(j= 0; j< Ny; j++){
				ip1= (i+1)%Nx;
				jp1= (j+1)%Ny;
				E-= rede[i*Ny+j]*(rede[ip1*Ny+j] + rede[i*Ny+jp1]);
				M+= rede[i*Ny+j];
			}
		}		
		output(0, rede);
		E1= E2= M1= M2= 0.0;
		for(k= 0; k< MC; k++){
			i= gsl_rng_uniform(w)*Nx;
			j= gsl_rng_uniform(w)*Ny;
			ip1= (i+1)%Nx;
			im1= (i-1+Nx)%Nx;
			jp1= (j+1)%Ny;
			jm1= (j-1+Ny)%Ny;
			DE= 2*rede[i*Ny+j]*(rede[ip1*Ny+j] + rede[im1*Ny+j] + 
                          rede[i*Ny+jp1] + rede[i*Ny+jm1]);
			if(DE<=0){
         rede[i*Ny+j]*=-1;
         M+= 2*rede[i*Ny+j];
         E+= DE;
       }else{
        if(gsl_rng_uniform(w) < exp(-DE/(K_B*T))){
          rede[i*Ny+j]*= -1;
          M+= 2*rede[i*Ny+j];
          E+= DE;
        }
     }

			E1+= E;
			E2+= E*E;
			M1+= fabs(M);
			M2+= M*M;
		}
//	printf("Oi Marina T=%lf \n", T);	
	output(T*10, rede);
	}
	free(rede);
	gsl_rng_free(w);
	return 0;
}
