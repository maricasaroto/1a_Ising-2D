#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gsl/gsl_rng.h>

#define Nx 2  
#define Ny 2
#define Ti 1.0 
#define Tf 4.0 
#define DT 0.1 
#define MC 1e6
#define NT 1e4 
#define K_B 1 


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
			
			if(T==Ti){
				printf("%d %g \n", k, M/(4.0));	
			}
		
		}
		//printf("%e %e %e %e %e\n", T, E1/(Nx*Ny*MC), (E2/MC-(E1*E1)/(MC*MC))/(Nx*Ny*T*T), M1/(MC*Nx*Ny), (M2/MC-(M1*M1)/(MC*MC))/(T*Nx*Ny));
	}
	free(rede);
	gsl_rng_free(w);
	return 0;
}
