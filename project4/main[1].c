#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define LONGEST_ENGLISH_WORD 29


//Struct for output array in encoding mode
struct encode_output{
	unsigned long long location;
	int location_rank;
};

//Struct for each phrase in dictionary
struct phrase { 
	unsigned long long location;
	unsigned long long index;
	int location_rank;
	char string[LONGEST_ENGLISH_WORD];
	int count;
};

//Struct for dictionary
struct dictionary {
	struct phrase ** array;
	unsigned long long  * array_data;
	int * collision_data;	
	unsigned long long current_largest; 

};

//Hash function 
unsigned long long hash(char* raw)
{
	unsigned long long hash = 5381;
	int c;

	while((c = tolower(*raw++)))
	{	
		hash = ((hash << 5) + hash) +c;

	}

	return hash;
}

//Function for insert data for encoding mode
int dictionary_insert_from_raw(struct dictionary* dicts, char * raw, struct encode_output* output_array, unsigned long total_words )
{

	struct phrase * exist;
	exist = (struct phrase *)calloc(1,sizeof(struct phrase));
	
	unsigned long long hashvalue = hash(raw);

	exist->location = hashvalue;
	exist->location_rank = 0;
	strcpy(exist->string, raw);

	//Determine if data appears before/hash collision occur
	for(int i=0;i<dicts->current_largest;i++)
	{
		if(dicts->array_data[i] == hashvalue)
		{
			for(int j=0;j<dicts->collision_data[i];j++)
			{
				if(strcmp(raw, dicts->array[i][j].string)==0)
				{
					dicts->array[i][j].count +=1;
					output_array[total_words-1].location_rank =0;
					return hashvalue;
				}
					
			}


			dicts->array[i]=(struct phrase *)realloc(dicts->array[i], (dicts->collision_data[i]+1)*sizeof(struct phrase));
			dicts->array[i][dicts->collision_data[i]] = *exist;
			exist->location_rank = dicts->collision_data[i];
			dicts->collision_data[i] +=1;
			output_array[total_words-1].location_rank = dicts->collision_data[i]-1;
			return hashvalue;
		}
	}


	//Add a new phrase into the dictionary
	dicts->array = (struct phrase **)realloc(dicts->array, (dicts->current_largest+1)*sizeof(struct phrase *));
	dicts->array[dicts->current_largest] =(struct phrase *)calloc(1, sizeof(struct phrase));
	strcpy(dicts->array[dicts->current_largest-1][0].string, exist->string);
	dicts->array_data = (unsigned long long  * )realloc(dicts->array_data, (dicts->current_largest)*sizeof(unsigned long long));
	dicts->collision_data =(int *)realloc(dicts->collision_data, (dicts->current_largest)*sizeof(int));
	dicts->array_data[dicts->current_largest-1] = hashvalue;
	dicts->collision_data[dicts->current_largest-1] = 1;

	dicts->array[dicts->current_largest-1][0].count =1;
	dicts->array[dicts->current_largest-1][0].location = hashvalue;
	dicts->array[dicts->current_largest-1][0].location_rank = 0;
	dicts->array[dicts->current_largest-1][0].index = dicts->current_largest-1;
	dicts->current_largest += 1; 
	output_array[total_words-1].location_rank =0;
	return hashvalue;
}

//Function for insert data for decoding mode
int dictionary_insert_from_encode(struct dictionary* dicts, char * raw, int count )
{

	struct phrase * exist;
	exist = (struct phrase *)calloc(1,sizeof(struct phrase));
	
	unsigned long long hashvalue = hash(raw);

	exist->location = hashvalue;
	exist->location_rank = 0;
	strcpy(exist->string, raw);

	//Determine if data appears before/hash collision occur
	for(int i=0;i<dicts->current_largest;i++)
	{
		if(dicts->array_data[i] == hashvalue)
		{
	
			dicts->array[i]=(struct phrase *)realloc(dicts->array[i], (dicts->collision_data[i]+1)*sizeof(struct phrase));
			dicts->array[i][dicts->collision_data[i]] = *exist;
			exist->location_rank = dicts->collision_data[i];
			dicts->collision_data[i] +=1;
			return hashvalue;
		}
	}

	//Add a new phrase into the dictionary
	dicts->array = (struct phrase **)realloc(dicts->array, (dicts->current_largest+1)*sizeof(struct phrase *));
	dicts->array[dicts->current_largest] =(struct phrase *)calloc(1, sizeof(struct phrase));
	strcpy(dicts->array[dicts->current_largest-1][0].string, exist->string);
	dicts->array_data = (unsigned long long  * )realloc(dicts->array_data, (dicts->current_largest)*sizeof(unsigned long long));
	dicts->collision_data =(int *)realloc(dicts->collision_data, (dicts->current_largest)*sizeof(int));
	dicts->array_data[dicts->current_largest-1] = hashvalue;
	dicts->collision_data[dicts->current_largest-1] = 1;

	dicts->array[dicts->current_largest-1][0].count =count;
	dicts->array[dicts->current_largest-1][0].location = hashvalue;
	dicts->array[dicts->current_largest-1][0].location_rank = 0;
	dicts->array[dicts->current_largest-1][0].index = dicts->current_largest-1;
	dicts->current_largest += 1; 
	return hashvalue;
}


int main(int argc, char** argv){

	clock_t start, end;
    start = clock();

	if(argc != 3 && argc != 4){
		perror("ERROR: Invalid arguments\n");
		return EXIT_FAILURE;
	}
	//Entering the encoding mode
	if(strcmp("-e",argv[1])==0 && argc== 4)
	{
		//Initialize the dictionary
		struct dictionary* dicts = calloc(1, sizeof(struct dictionary));
		dicts->array = (struct phrase **)calloc(1, sizeof(struct phrase *));
		dicts->array[0] = (struct phrase *)calloc(1, sizeof(struct phrase));
		dicts->collision_data = (int *)calloc(1, sizeof(int));
		dicts->array_data = (unsigned long long *)calloc(1, sizeof(unsigned long long));
		dicts->current_largest = 1;



		//Open the input and output file
		FILE * input_ptr;
		FILE * output_ptr;
		char* word = calloc(1,sizeof(char));
		char* real_word = calloc(1, sizeof(char));
		input_ptr = fopen(argv[2], "r");

		struct encode_output* output_array = calloc(1,sizeof(struct encode_output ));
		unsigned long total_words=0;

		if(input_ptr ==NULL)
		{
			perror("Invalid input file name\n");
			return EXIT_FAILURE;
		}


		output_ptr = fopen(argv[3], "w");

		//Put all phrases into the dictionary
		while(fgets(word,LONGEST_ENGLISH_WORD, input_ptr)!=NULL){
			//printf("Read word %s the size is %ld\n",word,strlen(word));
			//printf("the string length is %ld\n", strlen(word));
			long int length = strlen(word) -1;
			//printf("the string length -1 is %ld\n", length);
			word[length]='\0';
			total_words +=1;

			//printf("Real word %s the size is %ld\n",word,strlen(word));
			unsigned long long hashvalue = hash(word);

			output_array = (struct encode_output *)realloc(output_array, sizeof(struct encode_output )*total_words);

			output_array[total_words-1].location = hashvalue;


			dictionary_insert_from_raw(dicts, word,output_array, total_words);


		}

		
		//Write the dictionary info to the output file
		for(int i =0; i< dicts->current_largest-1;i++){
			for(int j =0;j < dicts->collision_data[i];j++){
				char str[100];
				sprintf(str, "%llu %s %d\n", dicts->array_data[i],dicts->array[i][j].string,dicts->array[i][j].count);

				fputs(str, output_ptr);
				//printf("%s\n",str);
			}
		}

		fputs("----------The end of dictionary----------\n",output_ptr);
		
		
		printf("Total words equals to %lu\n",total_words);

		//Write the output array data to the output file
		for(int i=0;i<total_words;i++)
		{
			char * hashoutput = calloc(1, sizeof(char));
			sprintf(hashoutput,"%llu %u\n", output_array[i].location, output_array[i].location_rank);
			fputs(hashoutput,output_ptr);
		}
		
		fclose(input_ptr);
		fclose(output_ptr);

		
		


	//Entering the decoding mode 
	}else if(strcmp("-d",argv[1])==0 && argc ==4)
	{

		FILE * input_ptr;
		FILE * output_ptr;

		input_ptr = fopen(argv[2], "r");
		output_ptr = fopen(argv[3], "w");
		if(input_ptr == NULL)
		{
			perror("Invalid input file name\n");
			return EXIT_FAILURE;
		}

		char line[100];

		//Initialize the dictionary
		struct dictionary* dicts = calloc(1, sizeof(struct dictionary));

		dicts->array = (struct phrase **)calloc(1, sizeof(struct phrase *));
		dicts->array[0] = (struct phrase *)calloc(1, sizeof(struct phrase));
		dicts->collision_data = (int *)calloc(1, sizeof(int));
		dicts->array_data = (unsigned long long *)calloc(1, sizeof(unsigned long long));
		dicts->current_largest = 1;

		//Put all the phrases into the dictionary
		while(fgets(line,99, input_ptr)!=NULL)
		{
			if(strcmp("----------The end of dictionary----------\n",line)==0)
			{
				
				printf("End of dictionary\n");
				break;
			}
			char * raw = calloc(1,sizeof(char));
			char * count_string = calloc(1, sizeof(char));
			int count;
			int space_count =0;

			for(int i =0;i<strlen(line);i++)
			{
				if(isalpha(line[i])!=0)
				{
					strncat(raw, &line[i],1);
				}else if(line[i]==' ')
				{
					space_count ++;
				}

				if(space_count ==2 && line[i]!='\n')
				{
					strncat(count_string, &line[i],1);
				}			


			}

			count = atoi(count_string);

			dictionary_insert_from_encode(dicts, raw, count);

		}
		//Decode all the phrase from its encoded data
		while(fgets(line,99, input_ptr)!=NULL)
		{
			line[strlen(line)-1]='\0';
			unsigned long long hashvalue;
			char * pEnd;
			char * hashpart = calloc(1, sizeof(char));

			strncat(hashpart, line, strlen(line)-2);
			hashvalue = strtoull(hashpart, &pEnd, 0);
			int location_rank=atoi(&line[strlen(line)-1]);
			
			for(int i=0;i<dicts->current_largest;i++)
			{
				if(dicts->array_data[i]==hashvalue)
				{
					char str[100];

					sprintf(str,"%s\n",dicts->array[i][location_rank].string);
					fputs(str,output_ptr);

					break;

				}
			}
			



		}	


	//Entering the Query mode
	}else if(argc == 4 && strcmp(argv[1],"-q")==0)
	{
		printf("Welcome to Query mode\n");
		char * query_string = calloc(1,sizeof(char));
		strcpy(query_string, argv[3]);

		printf("The query data is %s\n", query_string);
		FILE * encode_ptr;

		encode_ptr = fopen(argv[2], "r");

		char line[100];
		
		//Loop through the file to check if there is matched data
		while(fgets(line,99, encode_ptr)!=NULL)
		{
			if(strcmp("----------The end of dictionary----------\n",line)==0)
			{
				printf("The query string is not exist in the file\n");
				break;
			}

			char * raw = calloc(1,sizeof(char));
			char * count_string = calloc(1, sizeof(char));
			int count;
			int space_count =0;

			for(int i =0;i<strlen(line);i++)
			{
				if(isalpha(line[i])!=0)
				{
					strncat(raw, &line[i],1);
				}else if(line[i]==' ')
				{
					space_count ++;
				}

				if(space_count ==2 && line[i]!='\n')
				{
					strncat(count_string, &line[i],1);
				}			


			}
			//If the matched data is found
			if(strcmp(raw, query_string)==0)
			{
				printf("The %s count is %s\n", raw, count_string);
				break;
			}


		}

	//Invalid mode entry
	}else
	{
		perror("Invalid mode selection\n");
		return -1;
	}

	end = clock();
    double endtime = (double)(end - start) / CLOCKS_PER_SEC;

    printf("The total time for this is %f\n",endtime);
    return EXIT_SUCCESS;
}