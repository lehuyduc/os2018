#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void print(item* i) {
	if (i==NULL) return;
        printf("%d %d %d\n",i->type,i->amount,i->unit);
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
	first = (first+1) % BUFFER_SIZE;
	memcpy(&buffer[first],i,sizeof(item));
	qsize++;
	printf("Produce. first = %d, last = %d\n",first,last);
}

item* consume() {
	if (qsize==0) return NULL;
	item* i = malloc(sizeof(item));
	memcpy(i,&buffer[last],sizeof(item));
	last = (last+1) % BUFFER_SIZE;
	qsize--;
	printf("Consume. first = %d, last = %d\n",first,last);
	return i;
}	

int main()
{
	item chicken, fries;
	chicken = newItem(0,10,0);
	fries = newItem(1,100,1);

	item *chickenPointer = &chicken, *friesPointer = &fries;
	produce(chickenPointer);
	produce(friesPointer);

	print((consume()));
	return 0;
}
