#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE (10)

typedef struct {
	int type; // 0 = chicken, 1 = fries
	int amount; // piece or gram
	int unit; // 0 = piece, 1 = gram
} item;

// 'first' is the index of the last element that was produced
// 'last' is the index of the next element that will be consumed
// 'qsize' is the number of elements currently in queue. 
// if qsize = 0, we can't consume. if qsize = BUFFER_SIZE, we can't produce.
item buffer[BUFFER_SIZE];
int first = -1, last = 0, qsize = 0;
sem_t mysem;

bool print(item* i) {
	if (i==NULL) return false;
        printf("%d %d %d\n",i->type,i->amount,i->unit);
	return true;
}

item newItem(char a,int b,char c) {
        item i;
        i.type = a;
        i.amount = b;
        i.unit = c;
        return i;
}


void produce(item *i) {
	if (qsize==BUFFER_SIZE) return;
	sem_wait(&mysem);
	first = (first+1) % BUFFER_SIZE;
	memcpy(&buffer[first],i,sizeof(item));
	qsize++;
	printf("Produce. first = %d, last = %d\n",first,last);
	sem_post(&mysem);
}

item* consume() {
	if (qsize==0) return NULL;
	sem_wait(&mysem);
	item* i = malloc(sizeof(item));
	memcpy(i,&buffer[last],sizeof(item));
	last = (last+1) % BUFFER_SIZE;
	qsize--;
	printf("Consume. first = %d, last = %d\n",first,last);
	sem_post(&mysem);
	return i;
}	

void *produceThread(void* param) 
{
	item chicken, fries, lessChicken;
        chicken = newItem(0,10,0);
        fries = newItem(1,100,1);
	lessChicken = newItem(0,5,0);
		
        item *chickenPointer = &chicken, 
	     *friesPointer = &fries,
	     *lessChickenPointer = &lessChicken;
				
	produce(chickenPointer);
	produce(friesPointer);
	produce(lessChickenPointer);
}

void *consumeThread(void* param)
{
	int n = 0;
	while (n<2) 
		if (print(consume())) n++;
}

int main()
{
	sem_init(&mysem, 0, 1);
	
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, produceThread, NULL);
	pthread_create(&tid2, NULL, consumeThread, NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	return 0;
}
