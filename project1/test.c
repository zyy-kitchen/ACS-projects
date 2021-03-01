#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "helper.h"


int main(){

	int mode_flag, data_flag, row1, col1, row2, col2;
	printf("Hello, Which mode do you want to choose ?\n");
	printf("Enter 1 for native mode; 2 for SIMD mode.\n");
	scanf("%d", &mode_flag);
	printf("You choose mode %d\n", mode_flag);
	printf("Please Choose the data type:\n");
	printf("Enter 1 for fixed; 2 for floating.\n");
	scanf("%d", &data_flag);
	printf("You choose mode %d\n", data_flag);

	if((mode_flag != 1 && mode_flag != 2) || (data_flag != 1 && data_flag != 2) ){
		printf("Mode/Data selection failed \n");
		return EXIT_FAILURE;
	}



	printf("Please Enter the row and col number for matrix 1\n");
	scanf("%d%d",&row1, &col1);
	printf("Please Enter the row and col number for matrix 2\n");
	scanf("%d%d",&row2, &col2);


	if(row2 !=col1){
		printf("Entered row/col number do not match for 2 matrixs\n");
		return EXIT_FAILURE;
	}


	if(mode_flag ==1 && data_flag ==1){
		short ** m1;
		m1 = (short  **)malloc(row1 * sizeof(*m1));
		short ** m2;
		m2 = (short **)malloc(row2 * sizeof(*m2));
		short ** result;
		result =(short **) malloc(row1 * sizeof (*result));


		for (int i=0; i<row1;i++) m1[i] = (short *)malloc(col1 * sizeof (short));
		for (int i=0; i<row2;i++) m2[i] = (short *)malloc(col2 * sizeof (short));
		for (int i=0; i<row1;i++) result[i] = (short*)malloc(col2 * sizeof (short));


		Initialize_short(m1, 8, row1, col1);
		Initialize_short(m2, 1, row2, col2);
		//Print_short(m1, row1, col1);
		//printf("\n");
		//Print_short(m2, row2, col2);
		clock_t start, end;
		double time_used;
		start =clock();
		Multiply_short(m1, m2,result,row1, col1, row2, col2);
		end = clock();
		time_used = ((double) (end -start)) / CLOCKS_PER_SEC;

		printf("The time used for this native short  %dX%d *  %dX%d Multiplication is %f\n", row1, col1, row2, col2, time_used);




		//Print_short(result, row1, col2);
		Free_short(m1, row1);
		Free_short(m2, row2);
		Free_short(result, row1);

		//Initialize_short(m2, 7, row2, col2);
		//Multiply_short(m1,m2,result,row1,col1,row2,col2);
	}else if(mode_flag ==1 && data_flag ==2){
			float ** m1;
			m1 = (float  **)malloc(row1 * sizeof(*m1));
			float ** m2;
			m2 = (float **)malloc(row2 * sizeof(*m2));
			float ** result;
			result =(float **) malloc(row1 * sizeof (*result));


			for (int i=0; i<row1;i++) m1[i] = (float *)malloc(col1 * sizeof (float));
			for (int i=0; i<row2;i++) m2[i] = (float *)malloc(col2 * sizeof (float));
			for (int i=0; i<row1;i++) result[i] = (float*)malloc(col2 * sizeof (float));


			Initialize_float(m1, 2.5, row1, col1);

			Initialize_float(m2, 1, row2, col2);
			//Print_float(m1, row1, col1);
			//printf("\n");
			//Print_float(m2, row2, col2);
			clock_t start, end;
			double time_used;
			start =clock();
			Multiply_float(m1, m2,result,row1, col1, row2, col2);
			end = clock();
			time_used = ((double) (end -start)) / CLOCKS_PER_SEC;

			printf("The time used for this native float  %dX%d *  %dX%d Multiplication is %f\n", row1, col1, row2, col2, time_used);
			//Print_float(result, row1, col2);

			Free_float(m1, row1);
			Free_float(m2, row2);
			Free_float(result, row1);


			//Initialize_short(m2, 7, row2, col2);
			//Multiply_short(m1,m2,result,row1,col1,row2,col2);
	}else if(mode_flag ==2 && data_flag ==1){
		short ** m1;
		m1 = (short  **)malloc(row1 * sizeof(*m1));
		short ** m2;
		m2 = (short **)malloc(row2 * sizeof(*m2));
		short ** result;
		result =(short **) malloc(row1 * sizeof (*result));


		for (int i=0; i<row1;i++) m1[i] = (short *)malloc(col1 * sizeof (short));
		for (int i=0; i<row2;i++) m2[i] = (short *)malloc(col2 * sizeof (short));
		for (int i=0; i<row1;i++) result[i] = (short*)malloc((col2+ 32-col2%32) * sizeof (short));


		Initialize_short(m1, 8, row1, col1);
		Initialize_short(m2, 1, row2, col2);
		//Print_short(m1, row1, col1);
		//printf("\n");
		//Print_short(m2, row2, col2);
		clock_t start, end;
		double time_used;
		start =clock();
		Multiply_short_SIMD(m1, m2,result,row1, col1, row2, col2);
		end = clock();
		time_used = ((double) (end -start)) / CLOCKS_PER_SEC;

		printf("The time used for this SIMD short  %dX%d *  %dX%d Multiplication is %f\n", row1, col1, row2, col2, time_used);




		//Print_short(result, row1, col2);
		Free_short(m1, row1);
		Free_short(m2, row2);
		Free_short(result, row1);

		//Initialize_short(m2, 7, row2, col2);
		//Multiply_short(m1,m2,result,row1,col1,row2,col2);
	}else if(mode_flag ==2 && data_flag==2){
		float ** m1;
			m1 = (float  **)malloc(row1 * sizeof(*m1));
			float ** m2;
			m2 = (float **)malloc(row2 * sizeof(*m2));
			float ** result;
			result =(float **) malloc(row1 * sizeof (*result));


			for (int i=0; i<row1;i++) m1[i] = (float *)malloc(col1 * sizeof (float));
			for (int i=0; i<row2;i++) m2[i] = (float *)malloc(col2 * sizeof (float));
			for (int i=0; i<row1;i++) result[i] = (float*)malloc((col2+16-col2%16 ) * sizeof (float));


			Initialize_float(m1, 2.5, row1, col1);

			Initialize_float(m2, 1, row2, col2);
			//Print_float(m1, row1, col1);
			//printf("\n");
			//Print_float(m2, row2, col2);
			clock_t start, end;
			double time_used;
			start =clock();
			Multiply_float_SIMD(m1, m2,result,row1, col1, row2, col2);
			end = clock();
			time_used = ((double) (end -start)) / CLOCKS_PER_SEC;

			printf("The time used for this SIMD float  %dX%d *  %dX%d Multiplication is %f\n", row1, col1, row2, col2, time_used);
			//Print_float(result, row1, col2);

			Free_float(m1, row1);
			Free_float(m2, row2);
			Free_float(result, row1);


			//Initialize_short(m2, 7, row2, col2);
			//Multiply_short(m1,m2,result,row1,col1,row2,col2);
	}
}
