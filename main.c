#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

//creating movie struture

struct movie {
	char* title;
	int year;
	char* lang;
	float rate;
	struct movie* next;
};
struct movie* createMovie(char* currLine) {
	struct movie* m = malloc(sizeof(struct movie));
	char* saveptr;
	char* temp;
	//first token is title of movie
	char* token = strtok_r(currLine, ",", &saveptr);
	m->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(m->title, token);

	//second token is year of movie
	token = strtok_r(currLine, ",", &saveptr);
	temp = calloc(strlen(token) + 1, sizeof(char));
	int x = atoi(temp);
	m->year = x;
	strcpy(temp, token);

	//third token movie is languages 
	token = strtok_r(currLine, ",", &saveptr);
	m->lang = calloc(strlen(token) + 1, sizeof(char));
	strcpy(m->lang, token);

	//fouth token is rating 
	token = strtok_r(currLine, ",", &saveptr);
	temp = calloc(strlen(token) + 1, sizeof(char));
	float f = atof(temp);
	m->rate = f;
	strcpy(temp, token);

	//set the next node to null in the newly created movie entry
	m->next = NULL;

	return m;
};
struct movie* processFile(char* fPath) {
	FILE* file = fopen(fPath, "r");

	char* line = NULL;
	size_t len = 0;
	ssize_t nread;
	char* token;

	//head of linked list
	struct movie* first = NULL;
	//back of the linked list
	struct movie* last = NULL;

	//read file line by line 
	nread = getline(&line, &len, file);
	while ((nread = getline(&line, &len, file)) != -1) {

		// Get a new node for linked list of first text line from csv file
		struct movie* newNode = createMovie(line);

		// Is this the first node ?
		if (first == NULL) {
			//adding first and last nodes
			first = newNode;
			last = newNode;
		}
		//adding node to lists if not first
		else {
			//adding new node
			last->next = newNode;
			last = newNode;
		}
	}
	free(line);
	fclose(file);
	return first;
}
void printT(struct movie* s) {
	printf("%s\n", s->title);
}
void printYT(struct movie* s) {
	printf("%d %s\n", s->year, s->title);
}
void printYRT(struct movie* s) {
	printf("%d %.1f %s\n", s->year, s->rate, s->title);
}
void printTList(struct movie* list, int key) {
	bool found = false;
	while (list != NULL) {
		if (list->year == key) {
			printT(list);
			found = true;
		}
		list = list->next;
	}
	if (!found) {
		printf("none found\n");
	}
	
}
void printYTList(struct movie* list, char* key) {
	bool found = false;
		while (list != NULL) {
			if (strstr(list->lang, key) != NULL) {
				printYT(list);
				found = true;
			}
			list = list->next;
		}
	if (!found) {
		printf("none found\n");
	}
	
}
void printYRTList(struct movie* list) {
	bool found = false;
	struct movie* temp,* start;
	start = list;
	while (list != NULL) {
		struct movie* m = NULL;
		while (start != NULL) {
			if (m == NULL) {
				m = start;
			}
			else if ((m->year == start->year) && (m->rate < start->rate)) {
				m = start;
			}
			start = start->next;
		}
		if (temp == NULL) {
			temp = m;
		}
		else {
			temp->next = m;
			temp = temp->next;
		}
		start = list;
		list = list->next;
	}

	while (temp != NULL) {
		printYRT(temp);
		found = true;
		temp = temp->next;
	}
	if (!found) {
		printf("none found\n");
	}
	
}
//start main
int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Must provide name of file to process\n");
		printf("Example usage: ./main movie_info_csv_file");
		return EXIT_FAILURE;
	}
	struct movie* list = processFile(argv[1]);
	printf("Processed file %s and parsed data for (add list size) movies",argv[1]);
	
	int input = -1;
	int year = -1;
	char* lang;
	while (input != 4) {
		printf("\n1. Show movies released in the specific year\n");
		printf("2. Show highest rated movie for each year\n");
		printf("3. Show the title and year of release of all movies in a specific language\n");
		printf("4. Exit from the program\n\n");
		printf("Enter a choice 1 to 4: ");
		scanf("%d",input);
		switch (input) {
			case 1:
				printf("Enter the year for which you want to see movies: ");
				scanf("%4d",year);
				printTList(list,year);
				break;
			case 2:
				printYRTList(list);
				break;
			case 3:
				printf("Enter the language for which you want to see movies: ");
				scanf("%s",lang);
				printYTList(list,lang);
				break;
			case 4:
				return EXIT_SUCCESS;
				break;
			default:
				continue;
		}
	}

	return EXIT_SUCCESS;
}
