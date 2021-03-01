#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "zlib.h"
#include <string.h>
#include <math.h>
#include <time.h>



#define BLOCK_SIZE 4096
#define BLOCK_SIZE_COMPRESS_LIMIT 4110

struct _thread_data{
	int thread_id;
	char uncompress[BLOCK_SIZE+1] ;
	uLong ucompsize;
	uLong compsize;
	char compress[BLOCK_SIZE_COMPRESS_LIMIT];
};






void *CompressionWork(void * thread_argv){


	struct _thread_data * thread_data;
	thread_data = (struct _thread_data *) thread_argv;

	//printf("Come Here1\n" );
			
	char compressed_output[thread_data->ucompsize];

	//printf("After Here2\n");

	compress2((Bytef *)compressed_output, &thread_data->compsize, (Bytef *)thread_data->uncompress, thread_data->ucompsize, Z_BEST_COMPRESSION );
	strcpy(thread_data->compress, compressed_output);


	pthread_exit(NULL);
}



void *NocompressionWork(void * thread_argv){


	struct _thread_data * thread_data;
	thread_data = (struct _thread_data *) thread_argv;

	strcpy(thread_data->compress, thread_data->uncompress);


	pthread_exit(NULL);
}


int read_file_blocks(char blocks[][BLOCK_SIZE+1], int thread_num, FILE * ptr, int * blocks_count){
	*blocks_count = 0;
	for(int i=0; i< thread_num; i++){
		memset(blocks[i],0,sizeof(blocks[i]));

		int temp;
		temp = fread(blocks[i], sizeof(char), BLOCK_SIZE, ptr);	
		if(temp != BLOCK_SIZE){

			blocks[i][temp]='\0'; 
			*blocks_count = *blocks_count + temp;
			return 1;
		}

		*blocks_count = *blocks_count+ temp;
		
		 blocks[i][temp]='\0'; 
	}


	return 0;
}



int main(int argc, char ** argv){

	if(argc < 4){
			fprintf(stderr, "ERROR: Not enough input/output files entered");
			return EXIT_FAILURE;
	}			



	FILE * input_ptr = fopen(*(argv+1), "r");
	FILE * output_ptr = fopen(*(argv+2), "w");
	FILE * record_ptr = fopen(*(argv+3), "w");

	if(input_ptr == NULL){
		fprintf(stderr, "ERROR: Cannot input file\n");
		return EXIT_FAILURE;
	}

	

	int thread_num;
	printf("Inter the amount of thread you want to use (Enter a value from 1 to 16)\n");
	scanf("%d", &thread_num);

	if(thread_num < 1 || thread_num >16){
		fprintf(stderr, "ERROR: Not valid entry for thread number\n");
		return EXIT_FAILURE;
	}

	struct _thread_data thread_data_array[thread_num];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_t threads[thread_num];
	//char **  cycle_blocks = calloc(thread_num, sizeof(char *));

	//for (int i =0; i< thread_num;i++) {cycle_blocks[i] = calloc(4097, sizeof(char));}

	char cycle_blocks[thread_num][BLOCK_SIZE+1];

	int rc =0;
	int blocks_num = 0;
	int compressed_length = 0;
	void * status;



	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);
	
	while(!read_file_blocks( cycle_blocks, thread_num, input_ptr, &blocks_num)){

		//printf("Come Here 4\n");

		
		for(long i=0; i<thread_num; i++){
			//printf("In main: creating thread %ld\n", i);
			
			thread_data_array[i].thread_id = i;
			strcpy(thread_data_array[i].uncompress, cycle_blocks[i]);
			thread_data_array[i].ucompsize = BLOCK_SIZE+1;
			thread_data_array[i].compsize = compressBound(BLOCK_SIZE+1);


			rc = pthread_create(&threads[i], &attr, CompressionWork, (void *)&thread_data_array[i]);
			//rc = pthread_create(&threads[i], &attr, NocompressionWork, (void *)&thread_data_array[i]);
			if(rc){
				printf("return code from pthread_create is %d\n", rc);
				exit(-1);
			}
			
			compressed_length = strlen(thread_data_array[i].compress);

			//printf("Here 1\n");
			putw(compressed_length, record_ptr);

			//fwrite(thread_data_array[i].uncompress, 1, strlen(thread_data_array[i].uncompress), output_ptr);

			fwrite(thread_data_array[i].compress, 1, strlen(thread_data_array[i].compress), output_ptr);

		}

			for(int i=0; i<thread_num;i++){
			rc = pthread_join(threads[i], &status);
			if(rc){
				printf("ERROR; return code from pthread_create is %d\n", rc);
				exit(-1);
			}
		//printf("Main: completed join with thread %d having a status of %ld\n", i,(long)status);
		}

		
	}


	
	//printf("remaining blocks count is %d\n", blocks_num);
	//printf("remaining blocks is %d\n", blocks_num/BLOCK_SIZE+1);


	int remaining_block = blocks_num/BLOCK_SIZE +1;



	for(long i=0; i<remaining_block; i++){
		//printf("In main: creating final thread %ld\n", i);
		

		if(i==remaining_block-1){
			//printf("In main: The final thread is %ld\n",i);
			thread_data_array[i].thread_id = i;
			//printf("The size of the final input array is %ld\n",strlen(cycle_blocks[i]) );	


			strcpy(thread_data_array[i].uncompress, cycle_blocks[i]);
			


			thread_data_array[i].ucompsize = (strlen(thread_data_array[i].uncompress));
			thread_data_array[i].compsize = compressBound(thread_data_array[i].ucompsize);



			//printf("The final thread ucompsize is %ld and compsize is %ld\n", thread_data_array[i].ucompsize, thread_data_array[i].compsize );


			rc = pthread_create(&threads[i], NULL, CompressionWork, (void *)&thread_data_array[i]);
			//rc = pthread_create(&threads[i], &attr, NocompressionWork, (void *)&thread_data_array[i]);

			compressed_length = strlen(thread_data_array[i].compress);
			putw(compressed_length, record_ptr);

			//printf("The output is %s \n", thread_data_array[i].uncompress);
			//printf("The output size is %ld \n", strlen(thread_data_array[i].uncompress));



			fwrite(thread_data_array[i].compress, 1, blocks_num%BLOCK_SIZE, output_ptr);
		}else{

			thread_data_array[i].thread_id = i;
			strcpy(thread_data_array[i].uncompress, cycle_blocks[i]);
			thread_data_array[i].ucompsize = BLOCK_SIZE+1;
			thread_data_array[i].compsize = compressBound(BLOCK_SIZE+1);


			rc = pthread_create(&threads[i], NULL, CompressionWork, (void *)&thread_data_array[i]);
			//rc = pthread_create(&threads[i], &attr, NocompressionWork, (void *)&thread_data_array[i]);

			compressed_length = strlen(thread_data_array[i].compress);
			putw(compressed_length, record_ptr);

			//printf("The output is %s \n", thread_data_array[i].compress);
			//printf("The output size is %ld \n", strlen(thread_data_array[i].compress));



			fwrite(thread_data_array[i].compress, 1, strlen(thread_data_array[i].compress), output_ptr);

		}

	}
	

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec)/1000000000.0;

	printf("Hi, the time used for %d threads is %f\n",thread_num, elapsed);
	pthread_exit(NULL);
	return EXIT_SUCCESS;
}