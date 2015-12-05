#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

char q_dir[20];

int main(int argc, char** argv) {

	strcpy(q_dir, argv[1]);

	pid_t query1_2_pid,  query3_pid, query4_pid;

	// execute query1_2
	if( (query1_2_pid=fork()) < 0) {
		printf("1st fork error");
	} else if(query1_2_pid == 0) {
        execl("./query1_2", "query1_2", "data/", q_dir, NULL);
	}

	if( (query3_pid=fork()) < 0) {
		printf("2nd fork error");
	} else if(query3_pid == 0) {
        execl("./query3", "query3", q_dir, NULL);
	}

	if( (query4_pid=fork()) < 0) {
		printf("3rd fork error");
	} else if(query4_pid == 0) {
        execl("./query4", "query4", q_dir, NULL);
	}

	waitpid(query3_pid, NULL, 0);
	waitpid(query1_2_pid, NULL, 0);
	waitpid(query4_pid, NULL, 0);

	return 0;
}

