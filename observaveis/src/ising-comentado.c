// criado em					: 2019/03/15
// ultima atualizacao	: 2019/03/18
// autor							: Mariana Casaroto <mariana.fcasaroto@gmail.com>
// notas							: Modelo de Ising- Metodo de Monte Carlo- Transicao de fase Ferromagnetico -> Paramagnetico
// compilacao					: gcc -lm -lgsl -lgslcblas ising.c
// execucao						: ./a.out 'semente' > data.dat ---> semente é opcional

/*---------------------------------------------------------------MODELO DE ISING----------------------------------------------------------------*/
// 	1. O primeiro passo é criar uma rede com uma configuracao inicial. Pode ser uma configuracao homogenea ou aleatoria. Uma configuracao aleato-
// 		 ria tem mais vatagem com menos tempo de computacao para atingir o equilibrio com um banho termico associado.
//
// 	2. Se da inicio a variação de temperatura do sistema 
// 		2.1 É necessário definir um Transiente para colocar o sistema na configuracao inicial de manor energia.
// 			2.1.1 Selecionar um numero da rede utilizando um numero aleatorio uniformemente distribuido.
// 			2.1.2 Dizemos quem são os vizinhos mais proximos respeitando as condições de contorno periódicas.
// 			2.1.3 Calculamos a variação de energia DE, se ela for menor ou igual a zero, vira o spin, se a probabilidade do spin virar for maior 
// 						que um numero aleatório, o spin também vira. DE=2s(sc+sb+sd+se)
//
// 		2.2 Calculo da energia e da magnetização iniciais do sistemas - NAO ENTENDI
//
// 		2.3 Interações do sistema acontece de forma semelhante ao transiente:
// 			2.3.1 Selecionar um numero da rede utilizando um numero aleatorio uniformemente distribuido.
// 			2.3.2 Dizemos quem são os vizinhos mais proximos respeitando as condições de contorno periódicas.
// 			2.3.3 Calculamos a variação de energia DE, se ela for menor ou igual a zero, vira o spin, se a probabilidade do spin virar for maior 
// 						que um numero aleatório, o spin também vira. DE=2s(sc+sb+sd+se)
// 			2.3.4 Atualiza a magnetizaçao e a energia do sistema
// 			2.3.5 Calcula-se a magnetizaçao media, a magnetização media quadrada, a energia media e a energia media quadrada.
// 			2.3.5 É feito isso diversas vezes (MC vezes)
// 			2.3.6 Calcula-se a média dos observaveis por interação de monte carlo
// 			2.3.7 Calcula-se os outros observáveis 
// 			2.2.8 Apos terminar as MC interações de monte carlo, o sistema aumenta a temperatura voltando no estagio 2.1 e repetindo todo o processo 
// 						DT vezes
//	
//
/*-----------------------------------------------------------------Bibliotecas------------------------------------------------------------------*/
#include <stdio.h>				// biblioteca de entrada e saida
#include <stdlib.h>				// biblioteca padrão
#include <math.h>					// biblioteca matematica
#include <time.h>					// biblioteca de manipulção de data e horário de forma padrão
#include <gsl/gsl_rng.h>	// Biblioteca cientifica Gnu -> definido o numero pseudo aleatorio

/*-----------------------------------------------------------------Constantes-------------------------------------------------------------------*/
#define Nx 2	           	// Tamanho da rede em x
#define Ny 2 							// Tamanho da rede em y
#define Ti 1.0          	// Temperatura inicial
#define Tf 4.0            // Temperatura final
#define DT 0.1            // Variação de temperatura
#define NT 1e4           	// número de passos de transiente
#define MC 1e6						// número de passos de monte carlo 
#define K_B 1							// constante de boltzmann

 
/*-------------------------------------------------------------------Inicio---------------------------------------------------------------------*/
int main(int argc, char **argv){
	/*----------------------------------------------------------------Variaveis-----------------------------------------------------------------*/
	int i, j, k;            // Contadores
	int ip1, im1, jp1, jm1;	// indices dos vizinhos com condição periodica de contorno
	int *rede;							// vetor rede-> representa cada espaço da rede
	double T;								// temperatura
	double DE; 							// variacao de energia
	double E;								// Energia
	double E1;							// Média da energia <E>			
	double E2;							// Média da energia ao quadrado <E*E>
	double E1_MC;						// Energia média de todas a insterações de monte carlo E1
	double E2_MC;						// Energia média ao quadrado de todas a insterações de monte carlo ao quadrado M1*M1/MC*MC
	double M;								// Magnetização
	double M1;							// Média da magnetização <M>
	double M2;							// Média da magnetização ao quadrado <M*M>
	double M1_MC;						// Magnetização média de todas a insterações de monte carlo M1/MC
	double M2_MC;						// Magnetização média ao quadrado de todas a insterações de monte carlo ao quadrado M1*M1/MC*MC
	double Cv; 							// Calor específico
	double Xm; 							// Susceptibilidade magnética do material
	double e; 							// Energia por spin
	double e1;							// Energia media por spin
	double e2;							// Energia media ao quadrado por spin 
	double m;								// Magnetização por spin
	double m1;							// Magnetização média por spin
	double m2;							// Magnetização média ao quadrado por spin
	double cv;							// Calor específico por spin
	
	/*-----------------------------------------------------------------Semente------------------------------------------------------------------*/
	gsl_rng_default_seed= (argc == 2) ? atoi(argv[1]) : time(NULL);  // verifica se o usuário inseriu um número na posição argv[1], se sim utiliza
                                                                   // esse número como semente, se não utiliza 
                                                                   // time(NULL)-número de segundos desde 1 da janeiro de 1970 como semente
  gsl_rng *w= gsl_rng_alloc(gsl_rng_taus);                        // aloca memória para a variável aleatória

	/*-----------------------------------------------------------------1.Rede-------------------------------------------------------------------*/
	rede= (int *) calloc(Nx*Ny, sizeof(int)); 	// aloca memória do tamanho Nx*Ny do tipo inteiro para criar a rede com vetores
	for(i= 0; i<Nx; i++){												// varre a rede nas linhas
		for(j= 0; j<Ny; j++){											// varre a rede nas colunas
			if(gsl_rng_uniform(w) < 0.5){						// "se o número aleatório for menor que 0.5 o spin é 1 se não é -1"
				rede[i*Ny+j]=1;												// modo padrão para encontrar certo elemento de uma matriz (nesse caso da rede)
			}else{																
				rede[i*Ny+j]=-1;
			 }	
			}		
		}
	/*--------------------------------------------------------2. Variação de temperatura------------------------------------------------------*/
	for(T= Ti; T<Tf; T+=DT){
		/*--------------------------------------------------------------Transiente------------------------------------------------------------*/			
		for(k= 0; k<NT; k++){
			i=gsl_rng_uniform(w)*Nx;
			j=gsl_rng_uniform(w)*Ny;
			ip1= (i+1)%Nx;
			im1= (i-1+Nx)%Nx;
			jp1= (j+1)%Ny;
			jm1= (j-1+Ny)%Ny;
			DE= 2*rede[i*Ny+j]*(rede[ip1*Ny+j]+rede[im1*Ny+j]+rede[i*Ny+jp1]+rede[i*Ny+jm1]);
			if(DE<=0){
				rede[i*Ny+j]*=-1;
			}else{
				if(gsl_rng_uniform(w)< exp(-DE/(K_B*T))){
					rede[i*Ny+j]*=-1;
			  }
			}	
		}
		/*------------------------------------------------------Calculo das condições iniciais------------------------------------------------*/
			E= 0.0;
			M= 0.0;
			for(i= 0; i<Nx; i++){
				for(j= 0; j<Ny; j++){
		     	ip1= (i+1)%Nx;
    	   	jp1= (j+1)%Ny;
 					E-= rede[i*Ny+j]*(rede[ip1*Ny+j] + rede[i*Ny+jp1]);
          M+= rede[i*Ny+j];					
				}			
			}

		/*-----------------------------------------------------------Interações do sistema----------------------------------------------------*/
		E1_MC=E2_MC= M1_MC= M2_MC= 0.0;
    for(k= 0; k< MC; k++){
      i= gsl_rng_uniform(w)*Nx;
      j= gsl_rng_uniform(w)*Ny;
      ip1= (i+1)%Nx;
 	    im1= (i-1+Nx)%Nx;
      jp1= (j+1)%Ny;
      jm1= (j-1+Ny)%Ny;
      DE= 2*rede[i*Ny+j]*(rede[ip1*Ny+j] + rede[im1*Ny+j] + rede[i*Ny+jp1] + rede[i*Ny+jm1]);
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
      E1_MC+= E;
      E2_MC+= E*E;
      M1_MC+= fabs(M);
      M2_MC+= M*M;
      }
	E1= E1_MC/MC;
	E2= E2_MC/MC*MC;
	e1 = E1/(Nx*Ny);
	M1= M1_MC/MC;
	M2= M2_MC/MC*MC;
	m1= M1/(Nx*Ny);
		 Xm= (M2_MC-M1_MC*M1_MC)/(K_B*T);
		 Cv=(E2_MC-E1_MC*E1_MC)/(K_B*T*T);
		 cv= Cv/(Nx*Ny);


	printf("%e %e %e %e %e %e %e %e %e %e \n", T, E1, E2, M1, M2, Xm, Cv, e1, m1, cv);
	}
	free(rede);
	gsl_rng_free(w);
	return 0;
}
