/*Sanjeev Khemani

Dr. Kun Suo

Fall 2021

Kennesaw State University

CS7172 Parallel and Distributed Computing*/



#include <pthread.h>

#include <stdlib.h>

#include <stdio.h>

#include <string.h>

//#include <time.h>



#define MAX 10240

#define NUM_THREADS 10



int n1, n2, total = 0;

char *s1, *s2;

FILE *fp;

const char *delims = ",. \t\n";

struct range

{

    size_t start;	   	             // starting offset

    size_t end;	           	             // ending offset (right after or one past last char)

} range;



// function declarations

int readf (FILE * fp);

int num_substring (int t);

void* calSubStringThread (void *threadid);

size_t skip_to_delim (size_t start,const char *tag);

int distributeOnThread (int thread, struct range *seg);

void nonThreadedCheck (void);



int main(int argc, char *argv[])

{	

	//clock_t start, end;

     	//double threaded_time_used;

	pthread_t threads[NUM_THREADS];

	int t, rc;

	readf(fp);

	// getting rid of newline

	s2[n2] = 0;

	

	//start = clock();

	for (t = 0; t < NUM_THREADS; t++)

	{

		rc = pthread_create(&threads[t], NULL, calSubStringThread, (void *) (size_t) t);

		if (rc)

		{

		    printf("ERROR; return code from pthread_create() is %d\n", rc);

		    exit(-1);

		}

	}



	for (t = 0; t < NUM_THREADS; t++)

	{

		pthread_join(threads[t], NULL);

	}



	//end = clock();

	//threaded_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	

	printf("The total substrings count across all threads is: %d\n", total);		//The time elapsed is: %f\n", total, threaded_time_used);

	nonThreadedCheck();

	

	return 0;

}



//reading input file and generating strings s1/s2 and store their lengths n1/n2

int readf (FILE * fp)

{

	if ((fp = fopen("strings.txt", "r")) == NULL)

	{

		printf("ERROR: can't open strings.txt!\n");

		return 0;

	}

	s1 = (char *) malloc(sizeof(char) * MAX);

	if (s1 == NULL)

	{

		printf("ERROR: out of memory!\n");

		return -1;

	}

	s2 = (char *) malloc(sizeof(char) * MAX);

	if (s1 == NULL)

	{

		printf("ERROR: out of memory\n");

		return -1;

	}

	// reading s1 s2 from the file

	s1 = fgets(s1, MAX, fp);

	s2 = fgets(s2, MAX, fp);

	// length of s1 and s2

	n1 = strlen(s1);

	n2 = strlen(s2) - 1;

	// when error exists

	if (s1 == NULL || s2 == NULL || n1 < n2)

		return -1;



	return 0;

}



//2, how to sum up the total number of substrings from all threads

int num_substring (int t)

{

	struct range seg;

	distributeOnThread (t,&seg);



	char *str = &s1[seg.start];

	char *end = &s1[seg.end];



	char *token = str;

	size_t count = 0;



	while (1)

	{

		// looking for a substring match of s2 in s1

		token = strstr (token, s2);

		if (token == NULL)

		    break;

		// don't intrude on next thread's segment

		if (token >= end)

		    break;

		// advancing the count

		count += 1;

		// pointing to the start of next possible match point for s2

		token += n2;

		// stopping when we go beyond the end of our thread's area

		if (token >= end)

		    break;

	}

	total += count;



	return count;

}



void* calSubStringThread (void *threadid)

{

	long tid = (long) threadid;

	int num = num_substring (tid);

	printf("Thread %ld substrings count is: %d\n", (tid+1), num);

	pthread_exit(NULL);

}



size_t skip_to_delim (size_t start,const char *tag)

{

	char *str;

	str = &s1[start];

	start += strcspn(str, delims);



	return start;

}



//1, how to distribute different parts of string s1 into different threads

int distributeOnThread (int thread, struct range *seg)

{

	int threadMultiplier = n1 / NUM_THREADS;



	// get starting offset

	do

	{

		seg->start = threadMultiplier * thread;

		// first thread always starts at offset 0

		if (thread == 0)

			break;

		// skip past a word and stop on a delimiter

		seg->start = skip_to_delim(seg->start,"off");

	} while (0);



	// get ending offset/length

	do

	{

		if (thread == (NUM_THREADS - 1))

		{

		    seg->end = n1;

		    break;

		}

		// scan at least the amount we're allocated

		seg->end = seg->start + threadMultiplier;



		// skip past a word and stop on a delimiter

		seg->end = skip_to_delim(seg->end,"end");

	} while (0);



	return 0;

}



// nonThreadedCheck check with non-threaded algorithm

void nonThreadedCheck(void)

{	

	//clock_t sstart, eend;

	//double sequential_time_used;

	//sstart = clock();

	size_t count = 0;

	char *token = s1;

	while (1)

	{

		token = strstr(token,s2);

		if (token == NULL)

		    break;



		count += 1;

		token += n2;

	}

	//eend = clock();

	//sequential_time_used = ((double) (eend - sstart)) / CLOCKS_PER_SEC;

	printf("\nThe sequential solution substrings count is: %zu\n", count);				//The time elapsed is: %f\n", count, sequential_time_used);

}