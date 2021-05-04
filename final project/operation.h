#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

extern pthread_mutex_t mutex;




typedef struct item_
{

	struct item_ *hash_next;
	struct item_ *next;
	struct item_ *prev;

	int itemsize;

	char * key;
	char * value;

}item;


typedef struct box_
{

	item** hashtable;
	item* head;
	item* tail;


	int hashpower;
	unsigned long long hashmask;
	unsigned long long maxsize;
	unsigned long long cursize;
	unsigned long long tablesize;


	int item_num;
	int rehash_time;

	int get_hit;
	int get_miss;

	int put_hit;
	int put_miss;

	int set_hit;
	int set_miss;

	int delete_hit;
	int delete_miss;



}box;


unsigned long long hash(char* raw);
int hash_put(box * b, item* it );
int hash_delete(box * b, char * key);
int box_rehash(box * b);


int item_put(box * b, char * key, char * value);
int item_set(box * b, char * key, char * value);
item * do_item_get(box * b, char * key);
int item_get(box*b, char * key);
int item_delete(box * b, char * key);

void container_status();









