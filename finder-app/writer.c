/* Assignment 2 - AESD       
 * writer.c
 * Author: Saloni Shah */
#define _GNU_SOURCE

#include<stdio.h>
#include<string.h>
#include<syslog.h>
#include<stdlib.h>
#include<dirent.h>
#include<libgen.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>

int main(int argc, char *argv[] ) {

	int fd, wr, cs, statdir, mkdr;
	struct stat st = {0};
	char *dir;
	char path[200]="\0";
	char command[210]="\0";

	strcpy(path, argv[1]);

//	openlog(NULL, 0, LOG_USER);

	if(argc != 3) {
		syslog(LOG_ERR, "Incorrect number of arguments\n"
				"USAGE:\n" "1 Path of file to be written to\n"
				"2 String to be written\n");
		printf("Error in Command line arguments\n");
		exit(1);
	}

	dir=dirname(argv[1]);

	statdir = stat(dir, &st);
	if(statdir == -1) {
		sprintf(command,"mkdir -p %s", dir);
		mkdr = system(command);
	
		if(mkdr == -1) {
			printf("error in creating directory\n");
			syslog(LOG_ERR, "Could not make new directory");
		}
	}

	fd = open(path, O_WRONLY|O_CREAT, 0777);
	if(fd == -1) {
		syslog(LOG_ERR, "Could not open specified file");
		perror("File open:");
		exit(1);
	}

	wr = write(fd, argv[2], strlen(argv[2]));
	if(wr == -1) {
		syslog(LOG_ERR, "Could not write in specified file");
		perror("File write:");
		exit(1);
	}

	cs = close(fd);
	if(cs == -1) {
		syslog(LOG_ERR, "Could not close specified file");
		perror("File close:");
		exit(1);
	}

	syslog(LOG_DEBUG, "Writing %s to %s", argv[2], path);
	return 0;
}



