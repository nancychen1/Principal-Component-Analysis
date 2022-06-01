
double **GetComponentData(void);
double *GetTotalDow(void);

#include "Functions.h"
#include <iostream> 

// Global variable.
char **tickers;

int main () {

   int p, n, i,j,t, count =0;
   double **DJI, *TotalDow;
   double **Cov, **Cor, **X, mu[30], **Q, **E, **Y, 
   beta[30], varM, expM, covRM[30], x[253*30], y[253*30];
   FILE *fp; 
   
   // Read in the DJI data. These are daily returns for the 30 components for 2020.
   // Put it into a 253 x 30 array.
  DJI = GetComponentData();

  // Now read in the daily returns of the total Dow average for 2020. This is a list
  // of 253 returns.  This is used in problem 3 to compute the individual components' betas.
  TotalDow = GetTotalDow();

   // Get the dimensions of DJI.
   n = Rows (DJI);      // Should be 253
   p = Columns (DJI);   // Should be 30
   printf ("The data has %d rows and %d columns.\n\n", n, p);
   printf ("The %d components of the Dow are:\n", p);
   printf ("Column\n");
   printf ("Number   Ticker\n");
   printf ("======   ======\n");
   for (i = 1; i <= p; i++) {
      printf ("  %2d     ", i);
      printf (tickers[i]);
      printf ("\n");
   }
   //allocate array space 
   X = Array (n,p); 
   //computes the means of each DJI matrix column 
   //then subtracts the mean so each column is mean zero 
   //also computes variance of mu[i] (used in problem 3)
   for (j = 1; j<= p; j++){
      for (i = 1; i <= n; i++) 
         mu[j]+= DJI[i][j]/n; 

      for (i = 1; i <= n; i++)
         X[i][j] = DJI[i][j]-mu[j];
   }

  
   //computes covariance and correlation of mean 0 DJI matrix 
   Cov = Covariance(X); 
   Cor = Correlation(X); 
   //header
   printf ("Ticker   Ticker   Correlation\n");
   printf ("======   ======   ===========\n");

   //prints tickers and correlation if correlation is above 0.8
   //only prints unique pair of tickers 
   //no repeats because Cor[i][j] = Cor[j][i]
   for (i = 1; i <= 30; i++ ){
      for (j = i; j <=30; j++) {
         if (Cor[i][j] > 0.8){
            if (i != j) {
             printf("%*s    ", 5, tickers[i]);
             printf("%*s    ", 5, tickers[j]);
             printf("  %8.4f   \n",Cor[i][j]);
            }
           }
      }
   }

//compute and show eigenvalues 
E = QRalgorithm(Cov);
printf("Eigenvalues of the covariance matrix: \n");
Show(E);

//compute and show eigenvectors 
Q = Evector(Cov, E); 
printf("Corresponding eigenvectors Q:\n");
Show(Q);

//prints the first eigenvector & corresponding ticker 
printf("\nFirst eigenvector: \n");
for (i = 1; i <= 30; i++) {
   printf("%8.4f  ", Q[i][1]);
   printf("%*s   \n", 5, tickers[i]);
    }

//calculates expectation of the daily market returns
for (int i = 1; i<= 253; i++)
   expM += TotalDow[i] / 253.0;

//calculates variance of daily market returns 
for (int i = 1; i<= 253; i++)
   varM +=  ( (TotalDow[i] - expM)* (TotalDow[i] - expM)) / 253.0;


fp = fopen("Prob3.txt", "w"); 
fprintf(fp, "beta q_i\n");

//compute covariance of (R,M)
//R = particular stock return on any day 
//M = overall market return that same day 
//then computes beta & prints out beta values and corresponding tickers 
printf ("\nTicker   Beta\n");
printf ("======   ======\n");
for (j = 1; j<= p; j++){
   for (i = 1; i <= n; i++) {
      covRM[j] += ((DJI[i][j] - mu[j])*(TotalDow[i]- expM))/n; 
   }
   beta[j]= covRM[j]/varM; 
   printf("%*s   ", 5, tickers[j]);
   printf("%8.4f  \n", beta[j]);

   //writes to file so can produce scatterplot of points 
   //in the form(beta_i, q_i)
   //beta_i is stock i's beta
   //q_i is the i_th component of the first eigenvector 
   fprintf(fp, "%.6f %.6f\n", beta[j], Q[j][1]); 
}

fclose(fp);  



//problem 4
fp = fopen("Prob4.txt", "w");
fprintf(fp, "X Y\n");
//compute x = beta_i * M_t 
//compute y = Y_t1 * q_i
//Y_t1 is first principal component of t_th row of DJI
//q_i is the ith component of the first eigenvector 
Y = Multiply(DJI, Q);
for (i =1; i<=30; i++)
   for (t = 1; t<= 253; t++){
      x[count] = beta[i]*TotalDow[t]; 
      y[count] = Y[t][1]*Q[i][1]; 

      //writes to file so we can produce scatterplot of x and y 
      fprintf(fp, "%.6f %.6f\n", x[count], y[count]);
      count++; 
   }

fclose(fp); 

//problem 6

//print out eigenvector & tickers corresponding to the second largest eigenvalue
//second largest eigenvalue is at index 2 
printf("\nSecond largest eigenvalue eigenvector: \n");
for (i = 1; i <= 30; i++) {
   printf("%8.4f  ", Q[i][2]);
   printf("%*s   \n", 5, tickers[i]);
    }


// Pause so the window doesn't close.
Pause ();

}

// This function reads in the 253 daily returns for each of the 30 components of the
// Dow for the year 2020.
double **GetComponentData() {

   int i, k, t;
   double R;
   double **Data;
   char input[100];
   FILE *fp;

   Data = Array (253, 30);

   tickers = (char **) calloc (31, sizeof (char *));
   for (i = 1; i <= 30; i++) {
      tickers[i] = (char *) calloc (10, sizeof (char));
   }

   fp = fopen ("DJI.txt", "r");
   for (i = 1; i <= 30; i++) {

      // Read in stock i's return data.

      // Get the tickers for each stock.
      fgets (input, 99, fp);
      for (k = 0; k < 10; k++) {
         tickers[i][k] = input[k];
         if (tickers[i][k] == '\n') tickers[i][k] = '\0';
      }

      // Read in the 253 daily returns in percent.
      for (t = 1; t <= 253; t++) {

         // Month t return for stock i, put it into X[t][i].
         fgets (input, 99, fp);
         sscanf (input, "%lf", &R);
         Data[t][i] = R;

      }

   }
   fclose (fp);

   return Data;

}


// This function reads in the daily aggregate returns of the Dow Jones Industrial Average
// for the year 2020.
double *GetTotalDow() {

   int t;
   double R;
   double *Data;
   char input[100];
   FILE *fp;

   Data = List(253);

   fp = fopen ("TotalDow.txt", "r");
   for (t = 1; t <= 253; t++) {

      // Month t return for the DJIA (the total Dow).
      fgets (input, 99, fp);
      sscanf (input, "%lf", &R);
      Data[t] = R;

   }

   fclose (fp);

   return Data;

}




