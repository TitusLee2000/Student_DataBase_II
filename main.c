// By Titus and David
// BCIT 2025

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The number of elements to increment or decrement the database by
#define INCREASE_SIZE 50

/**
 * Enum of BCIT groups. Each group corresponds to a campus.
 */
typedef enum Group {
	DOWNTOWN,
	BURNABY
} Group;

/**
 * Struct representing a student at BCIT.
 */
typedef struct Student {
	char id[10];		// Student's BCIT ID
	char name[256];		// Student's legal name
	int age;			// Student's age
	char program[128];	// Student's program
	float gpa;			// Student's grade point average
	Group group;		// Student's group (campus)
} Student;

/**
 * Struct representing a database of students.
 */
typedef struct StudentDatabase
{
	Student* database;				// Array of students
	size_t size;					// Current max capacity of database
	size_t count;					// Current number of students in database
} StudentDatabase;

/**
 * Saves a database to file.
 *
 * @param studentDb the database to save
 * @return 1 if saving was successful, 0 otherwise
 */
char saveDatabase(const StudentDatabase* studentDb) {
	char path[256] = "";
	printf("Please enter the name of the file you wish to save to: ");
	scanf("%s", path);

	char validInput = 0;
	while (!validInput) {
		printf("Would you like to save as a 'bin' or 'txt'?");
		char input[8] = "";
		scanf("%s", input);
		if (strcmp("bin", input) == 0) {
			strcat(path, ".bin");
			validInput = 1;
		} else if (strcmp("txt", input) == 0) {
			strcat(path, ".txt");
			validInput = 1;
		} else {
			printf("Invalid input!\n");
		}
	}
	char filePath[512] = "../";
	strcat(filePath, path);

	FILE* checkFile = fopen(filePath, "r");
	if (checkFile != NULL) {
		validInput = 0;
		while (!validInput) {
			printf("File already exists. Would you like to overwrite file? ('y' or 'n'): ");
			char input[8] = "";
			scanf("%s", input);
			if (strcmp("y", input) == 0) {
				validInput = 1;
			} else if (strcmp("n", input) == 0) {
				printf("Aborting save...\n");
				fclose(checkFile);
				return 0;
			} else {
				printf("Invalid input!\n");
			}
		}
	}
	fclose(checkFile);

	FILE* fp = fopen(filePath, "wb");
	if (fp == NULL) return 0;

	fwrite(&studentDb->size, sizeof(size_t), 1, fp);
	fwrite(&studentDb->count, sizeof(size_t), 1, fp);
	fwrite(studentDb->database, sizeof(Student), studentDb->size, fp);

	fclose(fp);
	return 1;
}
/**
 * Load a database from file.
 *
 * @param studentDb the database to load into
 * @return 1 if loading was successful, 0 otherwise
 */
char loadDatabase(StudentDatabase* studentDb) {
	char path[256] = "";
	printf("Please enter the name of the file you wish to load: ");
	scanf("%s", path);

	char validInput = 0;
	while (!validInput) {
		printf("Is the file a 'bin' or 'txt'?");
		char input[8] = "";
		scanf("%s", input);
		if (strcmp("bin", input) == 0) {
			strcat(path, ".bin");
			validInput = 1;
		} else if (strcmp("txt", input) == 0) {
			strcat(path, ".txt");
			validInput = 1;
		} else {
			printf("Invalid input!\n");
		}
	}
	char filePath[512] = "../";
	strcat(filePath, path);

	FILE* checkFile = fopen(filePath, "r");
	if (checkFile == NULL) {
		printf("No such file exists. Aborting load...\n");
		fclose(checkFile);
		return 0;
	}
	fclose(checkFile);

	FILE* fp = fopen(filePath, "rb");
	if (fp == NULL) return 0;

	fseek(fp, 0, SEEK_SET);
	fread(&studentDb->size, sizeof(size_t), 1, fp);

	fseek(fp, sizeof(size_t), SEEK_SET);
	fread(&studentDb->count, sizeof(size_t), 1, fp);

	fseek(fp, sizeof(size_t)*2, SEEK_SET);
	fread(studentDb->database, sizeof(Student), studentDb->size, fp);

	fclose(fp);
	return 1;
}

/**
 * Attempts to increment the size of a student database.
 *
 * @param studentDb the student database to be incremented
 * @return 1 if successful, 0 otherwise
 */
char incrementDatabase(StudentDatabase* studentDb)
{
	// Create copy of database with greater size
	const size_t newSize = studentDb->size + INCREASE_SIZE;
	Student* newDb = realloc(studentDb->database, sizeof(Student) * newSize);

	// Reallocation might fail, so we test to make sure newDb is valid
	if (newDb != NULL)
	{
		studentDb->database = newDb;
		studentDb->size = newSize;
		return 1;
	}
	// Return false if reallocation failed
	else return 0;
}

/**
 * Attempts to decrement the size of a student database. Database size will never be decreased below INCREASE_SIZE.
 *
 * @param studentDb the student database to be decremented
 * @return 1 if successful, 0 otherwise
 */
char decrementDatabase(StudentDatabase* studentDb)
{
	// Don't decrement the database if it is already small
	if (studentDb->size <= INCREASE_SIZE)
	{
		return 0;
	}

	// Create copy of database with smaller size
	const size_t newSize = studentDb->size - INCREASE_SIZE;
	Student* newDb = realloc(studentDb->database, sizeof(Student) * newSize);

	// Make sure reallocation was successful
	if (newDb != NULL)
	{
		studentDb->database = newDb;
		studentDb->size = newSize;
		return 1;
	}
	// Return false if reallocation failed
	else return 0;
}

/**
 * Creates data for padding space when printing students.
 *
 * @param studentDb the database
 * @param padding array of padding data, modified in-place
 * @param pSize size of array of padding data
 */
void makePadding(const StudentDatabase* studentDb, int padding[], const size_t pSize)
{
	for (size_t i = 0; i < studentDb->count; i++)
	{
		const Student* student = &studentDb->database[i];
		if (strlen(student->id) > padding[0]) {padding[0] = strlen(student->id);}
		if (strlen(student->name) > padding[1]) {padding[1] = strlen(student->name);}
		if (strlen(student->program) > padding[3]) {padding[3] = strlen(student->program);}
	}
	padding[5] = strlen("Downtown");	// GROUP padding
	padding[2] = 2;							// Age padding
	padding[4] = 3;

	// Minimum of 1 space of padding
	for (size_t i = 0; i < pSize; i++)
	{
		padding[i]++;
	}
}

/**
 * Prints a student's details.
 *
 * @param student the student to print
 * @param padding details
 */
void printStudent(const Student* student, const int padding[])
{
	const char* groupName = (student->group == DOWNTOWN) ? "Downtown" : "Burnaby";
	printf("| %-*s | %-*s | %-*d | %-*s | %-.2f | %-*s |\n", padding[0], student->id, padding[1], student->name, padding[2], student->age, padding[3], student->program, student->gpa, padding[5], groupName);
}
/**
 * Add a student to the database.
 *
 * @param studentDb the database to add to
 */
void addStudent(StudentDatabase* studentDb) {
	//=====================================
	//        PROMPT AND VALIDATE
	//=====================================

	char userInput[255];		// for all inputs
	char studentID [10] = "A0";	// Student's ID
	char studentName [256];		// Student's name
	int studentAge;				// Student's age
	char studentProgram[128];	// Student's program
	float studentGpa;			// Student's grade point average
	Group studentGroup;			// Student's group

	int validID = 1;
	// validate ID
	while (validID) {
		printf("Enter Student ID by number [7 digits]:\n");
		scanf("%s", &userInput);

		char* stringPtr; //String portion of the input
		strtol(userInput, &stringPtr, 10);
		if (*stringPtr != '\0') {
			printf("Invalid student ID: ID must be numerical\n");
			continue;
		}
		if (strlen(userInput) != 7) {
			printf("Invalid student ID: Must be length 7\n");
			continue;
		}
		//check uniqueness
		int flag = 0;
		for (size_t i = 0; i < studentDb->count; i++)
		{
			char tempUserID[100]; // To add A0 to the beginning of the string
			strcpy(tempUserID, "A0");
			strcat(tempUserID, userInput);
			if (!strcmp(studentDb->database[i].id, tempUserID))
			{
				printf("Invalid student ID: student ID %s already exist\n", tempUserID);
				flag = 1;
				break;
			}
		}
		if (flag) { // if the number already exist
			continue;
		}
		validID = 0;
		// If everything passes
		strcat(studentID, userInput);
	};
	// validate name
	int validName = 1;
	while (validName) {
		printf("Enter Student Name:\n");
		scanf("%s", &userInput);
		char* stringPtr; //String portion of the input
		strtol(userInput, &stringPtr, 10);
		if (strlen(userInput) > 255 || strlen(userInput) == 0) { //out of bound
			printf("Invalid student Name: length must be less than 256\n");
			continue;
		}
		int flag = 0;
		for (int i = 0; userInput[i] != '\0'; i++) {
			if (!isalpha(userInput[i])) {
				printf("Invalid student Name: Name must only contain alphabetical characters\n");
				flag = 1;
				break;
			}
		}
		if (flag) { // reset question if student name contains integer
			continue;
		}
		if (*stringPtr == '\0') {
			printf("Invalid student Name: Name must only contain alphabetical characters\n");
		}
		validName = 0;
		strcpy(studentName, userInput);
	}
	// validate age
	int validAge = 1;
	while (validAge) {
		printf("Enter Student Age:\n");
		scanf("%s", &userInput);
		char* stringPtr; //String portion of the input
		int number = strtol(userInput, &stringPtr, 10);
		if (*stringPtr != '\0') {
			printf("Invalid student Age: Age must be numeric\n");
			continue;
		}
		if (number < 18) {
			printf("Invalid student Age: Age must be greater than 18\n");
			continue;
		}
		validAge = 0;
		studentAge = number;
	}
	// validate program
	int validProgram = 1;
	while (validProgram) {
		printf("Enter Student Program:\n");
		char dummy;
		scanf("%c", &dummy);
		fgets(userInput, sizeof(userInput), stdin);
		printf(userInput);
		int flag = 0;
		for (int i = 0; i < strlen(userInput); i++) {
			if (userInput[i] != ' ' && !isalpha(userInput[i]) && !userInput[i] != '\0') {
				printf("Invalid student Program: Program can only contain alphabetical characters\n");
				flag = 1;
				break;
			}
			if (userInput[i] == '\n' || userInput[i] == '\r') {
				userInput[i] = '\0';
			}
		}
		if (flag) { // the program contained anything other than alphabetical characters
			continue;
		}
		validProgram = 0;
		strcpy(studentProgram, userInput);
	}
	// validate GPA
	int validGPA = 1;
	while (validGPA) {
		printf("Enter Student GPA between [0.0 - 5.0]:\n");
		scanf("%s", &userInput);
		char* stringPtr; //String portion of the input
		double gpaValue = strtod(userInput, &stringPtr);
		if (*stringPtr != '\0') { // has other characters than alphabet
			printf("Invalid student GPA: GPA must be a double\n");
			continue;
		}
		if (gpaValue <= 0.0 || gpaValue >= 5.0) {
			printf("Invalid student GPA: GPA range must be be between 0.0 to 5.0\n");
			continue;
		}
		validGPA = 0;
		studentGpa = gpaValue;
	}
	// validate Group
	int validGroup = 1;
	while (validGroup) {
        printf("[D]-Downtown [B]-Burnaby\n");
		printf("Enter Student Group ['D', 'B']:\n");
		scanf("%s", &userInput);
		char* stringPtr; //String portion of the input
		strtol(userInput, &stringPtr, 10);
		int flag = 0;
		for (int i = 0; i < strlen(userInput); i++) {
			if (!isalpha(userInput[i])) {
				printf("Invalid student Program: Program can only contain alphabetical characters\n");
				flag = 1;
				break;
			}
		}
		if (flag) { // the program contained anything other than alphabetical characters
			continue;
		}
		if (toupper(*stringPtr) == 'D') {
			studentGroup = DOWNTOWN;
		} else if (toupper(*stringPtr) == 'B') {
			studentGroup = BURNABY;
		} else {
			printf("Invalid student Group: Group must be 'D' or 'B'\n");
			continue;
		}
		validGroup = 0;
	}

    // Check capacity of database and increase size if needed
	if (studentDb->count >= studentDb->size) {
		// Attempt to increment the size of the database
		incrementDatabase(studentDb);

		// Check whether size was actually increased enough
		if (studentDb->count >= studentDb->size)
		{
			printf("ERROR: Database operation failed\n");
			return;
		}
	}

	const size_t index = studentDb->count++;

	// Create a new student after passing all validations
	// Student newStudent; = {{studentID}, {studentName}, studentAge,
	// 	{studentProgram}, studentGpa, studentGroup};'
	Student newStudent;
	strcpy(newStudent.id, studentID);
	strcpy(newStudent.name, studentName);
	newStudent.age = studentAge;
	strcpy(newStudent.program, studentProgram);
	newStudent.gpa = studentGpa;
	newStudent.group = studentGroup;

	studentDb->database[index] = newStudent;
}

/**
 *	Prints all students in a database.
 *
 * @param studentDb the student database to print from
 */
void displayStudents(const StudentDatabase* studentDb) {
	if (studentDb->count < 1)
	{
		printf("No students to display\n");
		return;
	}

	// Create padding info
	int padding[10] = {0};
	makePadding(studentDb, padding, 10);

	// Print table labels
	printf("\n| %-*s | %-*s | %-*s | %-*s | %-*s | %-*s |\n", padding[0], "ID", padding[1], "NAME", padding[2], "AGE", padding[3], "PROGRAM", padding[4], "GPA", padding[5], "GROUP");
	printf("| %-*s | %-*s | %-*s | %-*s | %-*s | %-*s |\n", padding[0], "", padding[1], "", padding[2], "", padding[3], "", padding[4], "", padding[5], "");

	// Print each student
	for (size_t i = 0; i < studentDb->count; i++)
	{
		printStudent(&studentDb->database[i], padding);
	}
	printf("\n");
}

/**
 * Prints a student's records if they exist in the database.
 *
 * @param studentDb the database to search
 */
void searchStudent(StudentDatabase* studentDb) {
	// Get id from user input
	char targetId[10];
	printf("Please enter the ID of the student you wish to find: ");
	scanf("%s", targetId);
	targetId[0] = toupper(targetId[0]);

	for (size_t i = 0; i < studentDb->count; i++)
	{
		if (!strcmp(studentDb->database[i].id, targetId))
		{
			int padding[10] = {0};
			makePadding(studentDb, padding, 10);
			printStudent(&studentDb->database[i], padding);

			return;
		}
	}

	printf("Student with ID %s not found\n", targetId);
}

/**
 * Deletes a student's entry in the database.
 *
 * @param studentDb the database to delete from
 */
void deleteStudent(StudentDatabase* studentDb) {
	// Get id from user input
	char targetId[10];
	printf("Please enter the ID of the student you wish to remove: ");
	scanf("%s", targetId);
	targetId[0] = toupper(targetId[0]);
	Student* targetStudent = NULL;
	size_t index = 0;

	// Iterate over all students to find the target ID
	for (size_t i = 0; i < studentDb->count; i++)
	{
		if (!strcmp(studentDb->database[i].id, targetId))
		{
			targetStudent = &studentDb->database[i];
			index = i;
			break;
		}
	}

	// Return if no student found
	if (targetStudent == NULL)
	{
		printf("No student with ID '%s' in found\n", targetId);
		return;
	}

	// Shift the database down
	for (size_t i = index; i < studentDb->count - 1; i++)
	{
		studentDb->database[i] = studentDb->database[i + 1];
	}
	studentDb->count -= 1;

	// If database can be shrunk, shrink it
	if (studentDb->count < (studentDb->size - INCREASE_SIZE))
	{
		decrementDatabase(studentDb);
	}

	printf("Student with ID %s deleted from database\n", targetId);
}

//Comparator functions

int compareId(void* a, void* b) {
	Student* studentA = (Student*) a;
	Student* studentB = (Student*) b;
	return strcmp(studentA->id, studentB->id);
}

int compareIdReverse(void* a, void* b) {
	return -compareId(a, b);
}

int compareName(void* a, void* b)
{
	Student* studentA = (Student*) a;
	Student* studentB = (Student*) b;

	return strcmp(studentA->name, studentB->name);
}
int compareNameReverse(void* a, void* b) {
	return -compareName(a, b);
}

int compareGpa(void* a, void* b)
{
	Student* studentA = (Student*) a;
	Student* studentB = (Student*) b;

	if (studentA->gpa > studentB->gpa) {return 1;}
	if (studentA->gpa < studentB->gpa) {return -1;}
	return 0;
}

int compareGpaReverse(void* a, void* b) {
	return -compareGpa(a, b);
}

void sortStudent(StudentDatabase* studentDb) {
	char sortMethod = -1;
	do {
		char input[256] = "";
		printf("Please select a sorting method ('id', 'name', or 'gpa'): ");
		scanf("%s", input);
		if (strcmp(input, "id") == 0) {
			sortMethod = 1;
		} else if (strcmp(input, "name") == 0) {
			sortMethod = 2;
		} else if (strcmp(input, "gpa") == 0) {
			sortMethod = 3;
		} else {
			printf("Invalid input.\n");
		}
	} while (sortMethod == -1);

	char order = 0;
	do {
		char input[256] = "";
		printf("Sort in ascending or descending order? ('asc' or 'desc'): ");
		scanf("%s", input);
		if (strcmp(input, "asc") == 0) {
			order = 1;
		} else if (strcmp(input, "desc") == 0) {
			order = -1;
		} else {
			printf("Invalid input.\n");
		}
	} while (order == 0);

	int (*compare) (const void*, const void*);

	if (sortMethod == 1) {
		compare = (order == 1) ? compareId : compareIdReverse;
	} else if (sortMethod == 2) {
		compare = (order == 1) ? compareName : compareNameReverse;
	} else if (sortMethod == 3) {
		compare = (order == 1) ? compareGpa : compareGpaReverse;
	} else {
		printf("Sorting failed.\n");
		return;
	}

	qsort(studentDb->database, studentDb->count, sizeof(Student), compare);
	printf("Sorting finished.\n");
}

/**
 * List students by group.
 *
 * @param studentDb student database to list from
 */
void listStudents(StudentDatabase* studentDb) {
	Group studentGroup;

	char validInput = 0;
	while (!validInput) {
		char input[256];
		printf("Please enter the group to select ('B' or 'D'): ");
		scanf("%s", input);
		if (strcmp(input, "B") == 0 || strcmp(input, "b") == 0) {
			studentGroup = BURNABY;
			printf("\nBurnaby:\n");
			validInput = 1;
		} else if (strcmp(input, "D") == 0 || strcmp(input, "d") == 0) {
			studentGroup = DOWNTOWN;
			printf("\nDowntown:\n");
			validInput = 1;
		} else {
			printf("\nInvalid input.\n");
		}
	}

	// Create padding info
	int padding[10] = {0};
	makePadding(studentDb, padding, 10);

	char num = 0;
	for (int i = 0; i < studentDb->count; i++) {
		Student* student = &studentDb->database[i];
		if (student->group == studentGroup) {
			printStudent(student, padding);
			++num;
		}
	}
	if (num == 0) {
		printf("No students to display\n");
	}
}

int main() {
	// Initialise the student database
	StudentDatabase studentDatabase;
	studentDatabase.database = calloc(INCREASE_SIZE, sizeof(Student));
	studentDatabase.size = INCREASE_SIZE;
	studentDatabase.count = 0;

	// Demo
	char quit = 0;
	while (!quit) {
		char input[256] = "";
		printf("Enter a command or 'help'\n");
		scanf("%s", input);

		if (strcmp(input, "exit") == 0) {
			char loop = 1;
			while (loop) {
				loop = 0;
				printf("Save database before exiting? ('y'|'n'|'cancel')\n");
				scanf("%s", input);
				if (strcmp(input, "y") == 0) {
					printf("Saving database...\n");
					if (saveDatabase(&studentDatabase)) {
						quit = 1;
						printf("Exiting...\n");
					} else {
						printf("Save failed.\n");
						loop = 1;
					}
				} else if (strcmp(input, "n") == 0) {
					printf("Exiting without saving...\n");
					quit = 1;
				} else if (strcmp(input, "cancel") == 0) {
					printf("Exit canceled.\n");
				} else {
					printf("Unrecognised command.\n");
					loop = 1;
				}
			}
		} else if (strcmp(input, "add") == 0) {
			addStudent(&studentDatabase);
		} else if (strcmp(input, "display") == 0) {
			displayStudents(&studentDatabase);
		} else if (strcmp(input, "delete") == 0) {
			deleteStudent(&studentDatabase);
		} else if (strcmp(input, "search") == 0) {
			searchStudent(&studentDatabase);
		} else if (strcmp(input, "save") == 0) {
			printf("Saving database...\n");
			if (saveDatabase(&studentDatabase)) {
				printf("Saved successfully\n");
			} else {
				printf("Save failed!\n");
			}
		} else if (strcmp(input, "load") == 0) {
			printf("Loading database...\n");
			if (loadDatabase(&studentDatabase)) {
				printf("Loaded successfully\n");
			} else {
				printf("Load failed!\n");
			}
		} else if (strcmp(input, "details") == 0) {
			printf("Number of students: %llu\n", studentDatabase.count);
			printf("Allocated memory: %llu bytes (%llu slots)\n", studentDatabase.size * sizeof(studentDatabase.database), studentDatabase.size);

		} else if (strcmp(input, "help") == 0) {
			printf ("Valid commands:\n");
			printf ("add, display, search, delete, list, sort, save, load, details, exit\n");
		} else if (strcmp(input, "list") == 0) {
			listStudents(&studentDatabase);
		} else if (strcmp(input, "sort") == 0) {
			sortStudent(&studentDatabase);
		}
		else {
			printf("Unrecognised command. Enter 'help' to see a list of commands.\n");
		}
		printf("\n\n___________________________\n");
	}

	// Free memory
	free(studentDatabase.database);
	return 0;
}