#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "operation.h"

#define HASH_POWER 16
#define MAXBYTE 64 * 1024 * 1024


typedef struct test_argu_
{
	box * b;
	int * set_num;
}test_argu;



box * box_initial()
{
	box * b = calloc(1,sizeof(box));

	if(b == NULL)
	{
		fprintf(stderr, "Box Initialization failed\n");
		return NULL;
	}


	b->hashpower = HASH_POWER;
	b->hashmask = (1<<HASH_POWER) -1;
	b->maxsize = MAXBYTE;
	b->cursize = 0;
	b->hashtable = calloc(b->hashmask+1, sizeof(item *));
	b->tablesize = b->hashmask+1;

	if(b->hashtable ==NULL)
	{
		fprintf(stderr, "Box hashtable Initialization failed\n" );
		return NULL;
	}

	printf("Box Initialization Successful\n");
	return b;


}

void random_string(char* key,int length)
{
	char charset[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for(int i=0;i<length;i++)
	{
		int index = (double)rand()/RAND_MAX*(sizeof(charset)-1);
		printf("%d\n",index);
		key[i]=charset[index];
	}
	key[length]='\0';
}



void put_test(box* b, int test_num)
{
	int rc;
	int count =0;
	for(int i =0;i<test_num;i++)
	{
		char key[20];
		char value[20];
		sprintf(key,"key%d",i);
		sprintf(value,"value%d",i);

		rc = item_put(b,key,value);

		if(rc==0)
			count +=1;

	}

	if(count <test_num)
	{
		printf("Put test not pass.\n");
	}
	else
	{
		printf("Put test pass.\n");
	}


}



void get_test(box* b, int test_num)
{
	int rc;
	int count =0;
	for(int i =0;i<test_num;i++)
	{
		char key[20];
		sprintf(key,"key%d",i);

		rc = item_get(b,key);

		if(rc!=1)
			count +=1;

	}

	if(count <test_num)
	{
		printf("Get test not pass.\n");
	}
	else
	{
		printf("Get test pass.\n");
	}

}

void set_test(box* b, int test_num)
{
	int rc;
	int count =0;
	for(int i =0;i<test_num;i++)
	{
		char key[20];
		char value[20];
		sprintf(key,"key%d",i);
		sprintf(value,"valuenew%d",i);

		rc = item_set(b,key,value);

		if(rc==0)
			count +=1;

	}

	if(count <test_num)
	{
		printf("Set test not pass.\n");
	}
	else
	{
		printf("Set test pass.\n");
	}

}

void delete_test(box* b, int test_num)
{
	int rc;
	int count =0;
	for(int i =0;i<test_num;i++)
	{
		char key[20];
		sprintf(key,"key%d",i);

		rc = item_delete(b,key);

		if(rc==0)
			count +=1;

	}

	if(count <test_num)
	{
		printf("Delete test not pass.\n");
	}
	else
	{
		printf("Delete test pass.\n");
	}

}









int main()
{
	printf("Hello, What information you want to store!\n");
	printf("Enter p(put)/s(set)/g(get)/s(status)/d(delete)/q(quit)\n");

	box * b = box_initial();

	/*
	char option[2];

	while(fgets(option,10,stdin)!=NULL)
	{
		if(strcmp(option,"p\n")==0)
		{
			printf("Enter the key (no more than 20 chars): ");
			char key[21];
			char value[21];
			
			if(fgets(key,20,stdin)==NULL)
			{

				printf("Invalid Entry\n");
				continue;
			}
			printf("Enter the value (no more than 20 chars): ");

			if(fgets(value,20,stdin)==NULL)
			{

				printf("Invalid Entry\n");
				continue;
			}

			key[strlen(key)-1]='\0';
			value[strlen(value)-1]='\0';

			item_put(b,key,value);



		}
		else if(strcmp(option,"g\n")==0)
		{
			printf("Enter the key (no more than 20 chars): ");
			char key[21];
			
			if(fgets(key,20,stdin)==NULL)
			{

				printf("Invalid Entry\n");
				continue;
			}
			key[strlen(key)-1]='\0';


			item_get(b,key);

		}
		else if(strcmp(option,"d\n")==0)
		{
			printf("Enter the key (no more than 20 chars): ");
			char key[21];
			
			if(fgets(key,20,stdin)==NULL)
			{

				printf("Invalid Entry\n");
				continue;
			}
			key[strlen(key)-1]='\0';


			item_delete(b,key);

		}
		else if(strcmp(option,"t\n")==0)
		{
			container_status(b);
		}
		else if(strcmp(option,"q\n")==0)
			break;
		else
		{
			printf("Invalid option\n");
		}

		printf("What is the next operation?\n");
	}
	*/
	
	//test

	put_test(b, 100);
	get_test(b, 100);
	set_test(b,100);
	delete_test(b,100);

	container_status(b);
	
}