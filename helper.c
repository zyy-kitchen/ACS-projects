#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>
#include "helper.h"



void Multiply_short(short** m1,short **m2, short **result, int row1, int col1, int row2, int col2){

	for(int i =0; i< row1; i++){
		for (int j=0; j<col2; j++){
			result[i][j] = 0;
			for(int k=0; k<col1;k++){
				result[i][j] += m1[i][k]*m2[k][j];
			}
		}
	}
}


void Multiply_short_SIMD(short** m1,short **m2, short **result, int row1, int col1, int row2, int col2){

	for(int i =0; i< row1; i++){
		for (int j=0; j<col2; j+=32){
			__m256i sum1 = _mm256_setzero_si256();
			__m256i sum2 = _mm256_setzero_si256();
			for(int k = 0; k< col1; k++){
				__m256i m1_value = _mm256_set1_epi16(m1[i][k]);
				__m256i m2_value1 = _mm256_loadu_si256((__m256i*)&m2[k][j]);
				__m256i m2_value2 = _mm256_loadu_si256((__m256i*)&m2[k][j+16]);
				__m256i product1 = _mm256_mullo_epi16(m1_value, m2_value1);
				__m256i product2 = _mm256_mullo_epi16(m1_value, m2_value2);
				sum1 = _mm256_add_epi16(sum1, product1);
				sum2 = _mm256_add_epi16(sum2, product2);

			}	

			_mm256_storeu_si256((__m256i*)&result[i][j], sum1);
			_mm256_storeu_si256((__m256i*)&result[i][j+16], sum2);
		}
	}
}




void Initialize_short(short **m, short number, int row, int col){
	for (int i=0; i< row; i++){
		for (int j=0; j< col; j++){
			m[i][j] = number;
		}
	}
}

void Print_short(short **m, int row, int col){
	for (int i=0; i< row; i++){
		for (int j=0; j< col; j++){
			printf(" %i ",m[i][j]);
		}
		printf("\n");
	}
}

void Free_short(short **m, int row){
	for (int i=0; i< row; i++){
		free(m[i]);
	}
	free(m);
}


void Multiply_float(float** m1,float **m2, float **result, int row1, int col1, int row2, int col2){

	for(int i =0; i< row1; i++){
		for (int j=0; j<col2; j++){
			result[i][j] = 0;
			for(int k=0; k<col1;k++){
				result[i][j] += m1[i][k]*m2[k][j];
			}
		}
	}
}

void Multiply_float_SIMD(float** m1,float** m2, float **result, int row1, int col1, int row2, int col2){

	for(int i =0; i< row1; i++){
		for (int j=0; j<col2; j+=16){
			__m256 sum1 = _mm256_setzero_ps();
			__m256 sum2 = _mm256_setzero_ps();
			for(int k = 0; k< col1; k++){
				__m256 m1_value = _mm256_set1_ps(m1[i][k]);
				__m256 m2_value1 = _mm256_loadu_ps(&m2[k][j]);
				__m256 m2_value2 = _mm256_loadu_ps(&m2[k][j+8]);
				__m256 product1 = _mm256_mul_ps(m1_value, m2_value1);
				__m256 product2 = _mm256_mul_ps(m1_value, m2_value2);
				sum1 = _mm256_add_ps(sum1, product1);
				sum2 = _mm256_add_ps(sum2, product2);


			}	

			_mm256_storeu_ps(&result[i][j], sum1);
			_mm256_storeu_ps(&result[i][j+8], sum2);
		}
	}
}



void Initialize_float(float **m, float number, int row, int col){
	for (int i=0; i< row; i++){
		for (int j=0; j< col; j++){
			m[i][j] = number;
		}
	}
}

void Print_float(float **m, int row, int col){
	for (int i=0; i< row; i++){
		for (int j=0; j< col; j++){
			printf(" %.6f ",m[i][j]);
		}
		printf("\n");
	}
}


void Free_float(float **m, int row){
	for (int i=0; i< row; i++){
		free(m[i]);
	}
	free(m);
}



