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

item buffer[BUFFER_SIZE];
int first = -1, last = 0;
int firstTime = 1;

void produce(item *i) {
	if ((first+1)%BUFFER_SIZE==last && firstTime==0) return;
	first = (first+1) % BUFFER_SIZE;
	memcpy(&buffer[first],i,sizeof(item));
}

item* consume() {
	if (last==(first+1)%BUFFER_SIZE) return NULL;
	item* i = malloc(sizeof(item));
	memcpy(i,&buffer[last],sizeof(item));
	last = (last+1) % BUFFER_SIZE;
	return i;
}	

void print(item i) {
	printf("%d %d %d\n",i.type,i.amount,i.unit);
}

item newItem(char a,int b,char c) {
	item i;
	i.type = a;
	i.amount = b;
	i.unit = c;
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
	item* serving = consume();
	if (serving!=NULL) print(*serving);//(*serving).print();
	return 0;
}
