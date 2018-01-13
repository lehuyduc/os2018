#include <unistd.h>
#include <stdio.h>

int main()
{
	printf("Main before fork\n");
	int pid = fork();
	if (pid==0) {
		int pid2 = fork();
		if (pid2==0) {
			printf("Child child is ps-ing\n");
			char *args[] = {"/bin/ps", "-ef", NULL};
			execvp("/bin/ps", args);
		}
		else {
			printf("Child is free-ing\n");
			char *args[] = {"free", "-h", NULL};
			execvp("free", args);
		}
	}
 
}

