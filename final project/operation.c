#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <pthread.h>
#include "operation.h"


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



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



int hash_put(box * b, item* it )
{
	unsigned long long hashvalue = hash(it->key);


	pthread_mutex_lock(&mutex);
	it->hash_next = b->hashtable[hashvalue & b->hashmask];
	b->hashtable[hashvalue & b->hashmask] = it;
	pthread_mutex_unlock(&mutex);
	return 0;

}


int hash_delete(box * b, char * key)
{

	unsigned long long hashvalue = hash(key);

	item * temp = b->hashtable[hashvalue & b->hashmask];

	if(strcmp(temp->key,key)==0)
	{

		pthread_mutex_lock(&mutex);
		b->hashtable[hashvalue & b->hashmask] = temp->hash_next;
		pthread_mutex_unlock(&mutex);
		return 0;
	}
	else
	{
		while(temp)
		{
			if(strcmp((temp->hash_next)->key,key)==0)
			{
				pthread_mutex_lock(&mutex);
				temp->hash_next = (temp->hash_next)->hash_next;
				pthread_mutex_unlock(&mutex);
				return 0;
			}

			temp = temp->hash_next;
		}
		return 1;
	}

}

int box_rehash(box * b)
{


	item** newtable = calloc(1<<(b->hashpower+1),sizeof(item *));

	pthread_mutex_lock(&mutex);
	b->hashpower +=1;
	b->hashmask = (1<<(b->hashpower))-1;
	b->tablesize = 2*b->tablesize;

	unsigned long long old_table_size = 1<<(b->hashpower-1);
	for(int i =0;i<old_table_size;i++)
	{
		item* temp = b->hashtable[i];
		while(temp)
		{
		
			item* temp_next = temp->hash_next; 
			unsigned long long hashvalue = hash(temp->key);

			temp->hash_next = newtable[hashvalue & b->hashmask];
			newtable[hashvalue & b->hashmask] = temp;

			temp = temp_next;

		}

	}


	free(b->hashtable);
	b->hashtable = newtable;
	b->rehash_time +=1;
	pthread_mutex_unlock(&mutex);

	if(b->hashtable == NULL)
	{
		printf("rehash failed\n");
		return 1;
	}


	return 0;
}




int item_put(box * b, char * key, char * value)
{

	int new_item_size = sizeof(item) + strlen(key) + strlen(value);
	while(new_item_size + b->cursize >= b->maxsize)
	{
		assert(b->tail!=NULL);
		item* last = b->tail;
		b-> tail = last->prev;
		last->prev->next = last->next;

		b->cursize -=last->itemsize;
		b->item_num -=1;
		//printf("Evict last item\n");
		free(last->key);
		free(last->value);
		free(last);

	}


	item * it = calloc(1,sizeof(item));
	it->key = calloc(strlen(key),sizeof(char));
	it->value = calloc(strlen(value),sizeof(char));
	it->itemsize = new_item_size;

	strcpy(it->key,key);
	strcpy(it->value,value);

	hash_put(b,it);
	it->next = b->head;
	if(it->next!=NULL)
		it->next->prev = it;

	it->prev = NULL;
	b->head = it;
	if(b->tail ==NULL)
		b->tail = it;

	//printf("Item put finished\n");

	pthread_mutex_lock(&mutex);
	b->put_hit++;
	b->item_num++;
	b->cursize +=new_item_size;
	pthread_mutex_unlock(&mutex);


	double ratio = (100*b->item_num)/b->tablesize;
	if(ratio>150)
	{

		int rc=box_rehash(b);
		if(rc ==1)
			return 1;
	}

	return 0;

	
}



item * do_item_get(box * b, char * key)
{

	unsigned long long hashvalue = hash(key);
	item * result = NULL;
	item * temp = b->hashtable[hashvalue & b->hashmask];
	unsigned long long tableindex = hashvalue & b->hashmask;
	while(temp)
	{
		if(strcmp(temp->key,key)==0)
		{
			result = temp;
			printf("Find the item\n");

			break;
		}

		temp = temp->hash_next;

	}

	if(result ==NULL)
	{
		printf("No item found with the key %s\n",key);
	}
		
	printf("The table index is %llu",tableindex);
	return result;


}

int item_get(box*b, char * key)
{
	item * it = do_item_get(b,key);
	if(it == NULL)
	{
		pthread_mutex_lock(&mutex);
		b->get_miss++;
		pthread_mutex_unlock(&mutex);
		return 1;
	}
	
	else
	{
		pthread_mutex_lock(&mutex);
		b->get_hit++;
		pthread_mutex_unlock(&mutex);
		return 0;
	}

}

int item_delete(box * b, char * key)
{
	item * it = do_item_get(b, key);
	if(it !=NULL)
	{
		hash_delete(b,key);
		pthread_mutex_lock(&mutex);
		if(b->head == it)
		{
			b-> head = it->next;
		}
		else if(b->tail == it)
		{
			b-> tail = it->prev;
		}

		if(it->next!=NULL)
			it->next->prev = it->prev;
		if(it->prev!=NULL)
			it->prev->next = it->next;

		b->cursize -=it->itemsize;
		b->item_num -=1;
		printf("Delete Successful for item with key %s\n",key);
		b->delete_hit++;

		free(it->key);
		free(it->value);
		free(it);


		pthread_mutex_unlock(&mutex);
		return 0;
	}

	printf("Nothing to delete for key %s\n",key);
	pthread_mutex_lock(&mutex);
	b->delete_miss++;
	pthread_mutex_unlock(&mutex);
	return 1;

}


int item_set(box * b, char * key, char * value)
{

	item * old = do_item_get(b,key);
	if(old != NULL)
	{

		printf("THe old value is %s\n",old->value);

		pthread_mutex_lock(&mutex);
		strcpy(old->value, value);

		printf("The new value is %s\n",old->value);

		
		int new_item_size = sizeof(item) + strlen(old->key) + strlen(old->value);
		while(new_item_size + b->cursize >= b->maxsize)
		{
			assert(b->tail!=NULL);
			item* last = b->tail;
			b-> tail = last->prev;
			last->prev->next = last->next;

			b->cursize -=last->itemsize;
			b->item_num -=1;
			//printf("Evict last item\n");
			free(last->key);
			free(last->value);
			free(last);

		}

		b->cursize +=new_item_size;


		printf("Set value for old item success\n");
		old->next = b->head;
		if(old->next!=NULL)
			old->next->prev = old;

		old->prev = NULL;
		b->head = old;
		if(b->tail ==NULL)
			b->tail = old;

		b->set_hit++;





		pthread_mutex_unlock(&mutex);
		return 0;
	}
	else
	{

		printf("The item with key %s is not found, try to put a new item\n", key);
		int rc=item_put(b,key,value);

		pthread_mutex_lock(&mutex);
		if(rc ==1)
			b->set_miss++;
		else
			b->set_hit++;
		pthread_mutex_unlock(&mutex);
		return rc;

	}
}



void container_status(box * b)
{
	printf("----------The BOX----------\n");
	printf("Maxsize    is: %lluMB\n",b->maxsize/(1024*1024));
	printf("Cursize    is: %lluMB\n",b->cursize/(1024*1024));
	printf("Tablesize  is: %llu\n",b->tablesize);
	printf("Rehash num is: %d\n",b->rehash_time);
	printf("Item num   is: %d\n",b->item_num);
	printf("\n");
	printf("Get hit     : %d\n",b->get_hit);
	printf("Get miss    : %d\n",b->get_miss);
	printf("\n");
	printf("Put hit     : %d\n",b->put_hit);
	printf("Put miss    : %d\n",b->put_miss);
	printf("\n");
	printf("Set hit     : %d\n",b->set_hit);
	printf("Set miss    : %d\n",b->set_miss);
	printf("\n");
	printf("Delete hit  : %d\n",b->delete_hit);
	printf("Delete miss : %d\n",b->delete_miss);
	printf("----------The BOX----------\n");
	

}




//void container_status();

