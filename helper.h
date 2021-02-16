#include <stdlib.h>
#include <stdio.h>






void Multiply_short(short **m1,short **m2, short **result, int row1, int col1, int row2, int col2 );
void Initialize_short(short ** m, short number, int row, int col);
void Print_short(short **m, int row, int col);
void Free_short(short **m, int row);
void Multiply_short_SIMD(short** m1,short **m2, short **result, int row1, int col1, int row2, int col2);

void Multiply_float(float** m1,float **m2, float **result, int row1, int col1, int row2, int col2);
void Multiply_float_SIMD(float** m1,float**m2, float **result, int row1, int col1, int row2, int col2);
void Initialize_float(float **m, float number, int row, int col);
void Print_float(float **m, int row, int col);
void Free_float(float **m, int row);
