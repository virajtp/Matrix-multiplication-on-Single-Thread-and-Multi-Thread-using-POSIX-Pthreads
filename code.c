#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pthread.h>

//variables for Matrix dimentions
int mat1Rows, mat1Cols, mat2Rows,mat2Cols;

//declaring Matrices
float mat1[2000][2000],mat2[2000][2000];
float matAnsSingle[2000][2000],matAnsMulti[2000][2000];

//function to calculate the time and display it
void timeCal(){
        double timeSingleThread, timeMultiThread;

        clock_t startTime = clock();    //get starting time
        matMultiply();
        clock_t termination = clock();  //get ending time

        timeSingleThread = (double)(termination - startTime) * 1000 / CLOCKS_PER_SEC; //execution time

        startTime = clock();        //get starting time
        matMultiThread();
        termination = clock();      //get starting time

        timeMultiThread = (double)(termination - startTime) * 1000 / CLOCKS_PER_SEC; //execution time

        if(mat1Rows*mat2Cols<=10000)
            showAns();
        printf("\n%s %.3lf %s\n "," Time taken for Single Thread\t: " ,timeSingleThread/1000.0,"s"  );
        printf("%s%.3lf %s\n ","Time taken for Multi Thread\t:  " ,timeMultiThread/1000.0,"s"  );

        //to get values to the output file
        /*FILE *getOut;

        getOut = fopen("output.txt","a+");
        fprintf(getOut, "\t%d", mat1Rows*mat2Cols);
        fprintf(getOut, "\t\t");
        fprintf(getOut, "%f\t\t%f", timeSingleThread/1000,timeMultiThread/1000);
        fprintf(getOut,"\n");*/
    }

//Function to Print Matrices
void showMatrix(){
   printf("\n%s\n","------1st Matrix----- ");
int i,j;
  for ( i=0; i<mat1Rows; i++){
    for ( j=0; j<mat1Cols; j++){
        if(mat1[i][j]-(int)mat1[i][j]>0)
            printf("\t%f ",mat1[i][j] );
        else
            printf("\t%0.f\t ",mat1[i][j] );
    }
    printf("\n");
  }
     printf("\n%s\n","------2nd Matrix----- ");

  for ( i=0; i<mat2Rows; i++){
    for ( j=0; j<mat2Cols; j++){
     if(mat2[i][j]-(int)mat2[i][j]>0)
            printf("\t%f ",mat2[i][j] );
        else
            printf("\t%0.f\t ",mat2[i][j] );
    }
    printf("\n");
  }
}

//Function to create two matrices randomly
void createMatrix(){
    srand(time(0));
    int i,j;
//making the matrix 1
  for ( i=0; i<mat1Rows; i++)
      for ( j=0; j<mat1Cols; j++){
        if(rand()%2==1)
            mat1[i][j] = (float)(rand() % 100)/RAND_MAX*10000;
        else
            mat1[i][j] = (rand() % 100);
      }

//making the matrix 2
  for ( i=0; i<mat2Rows; i++)
 for ( j=0; j<mat2Cols; j++){
        if(rand()%2==1)
            mat2[i][j] = (float)(rand() % 100)/RAND_MAX*10000;
        else
            mat2[i][j] = (rand() % 100);
      }
}

//Function to read the text file
void readFile(){

  FILE *infile;

  infile = fopen("input.txt", "r"); //open the file

  if(infile == NULL){
      printf("Error opening file\n");
      exit(1);
  }
    int i,j;
  for( i=0;i<mat1Rows;i++)
     for( j=0;j<mat1Cols;j++)
         fscanf(infile,"%f,",&mat1[i][j]);

  for( i=0;i<mat2Rows;i++)
     for( j=0;j<mat2Cols;j++)
         fscanf(infile,"%f,",&mat2[i][j]);

  fclose(infile);
}

//Function to Multiplying matrix
void matMultiply(){
  	int i, j, k;
	   for(i = 0; i < mat1Rows; ++i)
     	   	for(j = 0; j < mat2Cols; ++j)
		       	  for(k=0; k<mat1Cols; ++k)
			          	matAnsSingle[i][j] += mat1[i][k] * mat2[k][j];
}

void * multiThreadMultiply( void *arg ){

  int i, j, k, tid, portion_size, row_start, row_end;
  double sum;

  tid = *(int *)(arg); // get the thread ID assigned sequentially.
  portion_size = mat2Rows / mat1Cols;
  row_start = tid * portion_size;
  row_end = (tid+1) * portion_size;

  for (i = row_start; i < row_end; ++i) { // hold row index of 'matrix1'
    for (j = 0; j < mat2Cols; ++j) { // hold column index of 'matrix2'
      sum = 0; // hold value of a cell
      for (k = 0; k < mat1Cols; ++k)
        sum += mat1[ i ][ k ] * mat2[ k ][ j ];
      matAnsMulti[ i ][ j ] = sum;
    }
  }
}

void matMultiThread(){
  int i;
  pthread_t * threads;
  //int size = atoi( argv[1] );
  int num_threads = mat1Rows;
  threads = (pthread_t *) malloc( num_threads * sizeof(pthread_t) );
  for ( i = 0; i < num_threads; ++i ) {
    int *tid;
    tid = (int *) malloc( sizeof(int) );
    *tid = i;
    pthread_create( &threads[i], NULL, multiThreadMultiply, (void *)tid );
  }
  for ( i = 0; i < num_threads; ++i ) {
    pthread_join( threads[i], NULL );
  }
}

void showAns(){
    printf("\n\n---------Multiplied Matrix---------\n\n");
    int i,j;
    for( i=0;i<mat1Rows;i++){
       for( j=0;j<mat2Cols;j++){
     if(matAnsMulti[i][j]-(int)matAnsMulti[i][j]>0)
            printf("\t%f ",matAnsMulti[i][j] );
        else
            printf("\t%0.f\t ",matAnsMulti[i][j] );
    }
         printf("\n");
    }
}
int main()
{
  //getting values for Rows and Column
	printf("Enter the size of Matrix 1 (ex. 2 3) : ");
	scanf("%d", &mat1Rows);
	scanf("%d", &mat1Cols);
	printf("\nEnter the size of Matrix 2 (ex. 2 3) : ");
	scanf("%d", &mat2Rows);
	scanf("%d", &mat2Cols);

    if(mat1Cols==mat2Rows){

        printf("\n[%d][%d]  x [%d][%d]  can be Multiplied \n",mat1Rows,mat1Cols,mat2Rows,mat2Cols );
        printf("-------------------------------------------\n");
		printf("\n\t1 - Input matrix from Text File\n");
		printf("\t2 - Generate matrix with random values\n\n");
		printf("\tSelect your option : ");
        int choice;
        scanf("%d",&choice);

        if(choice==1){
            printf("\n%s\n", "Text Read Matrices" );
            readFile();
            showMatrix();
            timeCal();
        }
        else if(choice==2){
                createMatrix(); // assging Random values to mtrices
                if(mat1Rows*mat2Cols<=10000)
                    showMatrix();
                timeCal();

               //to get the output file
            /*FILE *getOut;
            getOut = fopen("output.txt","w");
            fprintf(getOut, "No. of Elements");
            fprintf(getOut, "\t\t");
            fprintf(getOut, "Single Thread");
            fprintf(getOut, "\t\t");
            fprintf(getOut, "Multi Thread");
            fprintf(getOut,"\n");
            int i;
            for(i=0;i<=100;i++) {
                mat1Rows=mat1Cols=mat2Rows=mat2Cols=i*10;
                createMatrix(); // assging Random values to mtrices
                timeCal();
            }*/
        }
        else{
            printf("%s\n"," Invalid Input ");
            return 0;
        }
  }
  else{
       printf("\n\n[%d][%d]  x [%d][%d]  can't be Multiplied !!!\n",mat1Rows,mat1Cols,mat2Rows,mat2Cols );
       return 0;
  }
  return 0;
}

