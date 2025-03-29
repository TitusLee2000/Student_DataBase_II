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
 * Converts a string to lowercase.
 *
 * @param s the string to convert
 */
void strToLower(char* s) {
	for (int i = 0; i < strlen(s); i++) {
		s[i] = (char) tolower(s[i]);
	}
}

/**
 * Converts a string to uppercase.
 *
 * @param s the string to convert
 */
void strToUpper(char* s) {
	for (int i = 0; i < strlen(s); i++) {
		s[i] = (char) toupper(s[i]);
	}
}

/**
 * Saves a database to file.
 *
 * @param studentDb the database to save
 * @return 1 if saving was successful, 0 otherwise
 */
char saveDatabase(const StudentDatabase* studentDb) {
	char input[256] = "";
	printf("Please enter the name of the file you wish to save to (excluding extension): ");

	if (scanf(" %255[^\n]s ", input) != 1) {
		printf("Error reading input\n");
		return 0;
	}
	char partialPath[256] = "";
	strcpy(partialPath, input);

	char validInput = 0;
	while (!validInput) {
		printf("Would you like to save as a 'bin' or 'txt'?:");
		scanf(" %255[^\n]s ", input);
		if (strcmp("bin", input) == 0) {
			strcat(partialPath, ".bin");
			validInput = 1;
		} else if (strcmp("txt", input) == 0) {
			strcat(partialPath, ".txt");
			validInput = 1;
		} else {
			printf("Invalid input!\n");
		}
	}
	char filePath[512] = "../";
	strcat(filePath, partialPath);

	FILE* checkFile = fopen(filePath, "r");
	if (checkFile != NULL) {
		fclose(checkFile);
		validInput = 0;
		while (!validInput) {
			printf("File already exists. Would you like to overwrite file? ('y' or 'n'): ");
			scanf(" %255[^\n]s ", input);
			if (strcmp("y", input) == 0) {
				validInput = 1;
			} else if (strcmp("n", input) == 0) {
				printf("Save aborted.\n");
				return 0;
			} else {
				printf("Invalid input!\n");
			}
		}
	} else {
		while (1) {
			printf("Will save data to %s. Is this correct? ('y' or 'n'): ", filePath);

			scanf(" %255[^\n]s ", input);

			if (tolower(input[0]) == 'n') {
				printf("Save aborted.\n");
				return 0;
			} else if (tolower(input[0]) == 'y') {
				break;
			} else {
				printf("Invalid input!\n");
			}
		}
	}



	FILE* fp = fopen(filePath, "wb");
	if (fp == NULL) return 0;

	fwrite(&studentDb->size, sizeof(size_t), 1, fp);
	fwrite(&studentDb->count, sizeof(size_t), 1, fp);
	fwrite(studentDb->database, sizeof(Student), studentDb->size, fp);

	fclose(fp);
	return 1;
}

/**
 * Prints table column labels.
 *
 * @param padding padding to use when printing
 */
void printLabels(int padding[10]) {
	printf("\n| %-*s | %-*s | %-*s | %-*s | %-*s | %-*s |\n", padding[0], "ID", padding[1], "NAME", padding[2], "AGE", padding[3], "PROGRAM", padding[4], "GPA", padding[5], "GROUP");
	printf("| %-*s | %-*s | %-*s | %-*s | %-*s | %-*s |\n", padding[0], "", padding[1], "", padding[2], "", padding[3], "", padding[4], "", padding[5], "");
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
		fclose(checkFile);
		printf("No such file exists. Aborting load...\n");
		return 0;
	} else {
		fclose(checkFile);
		while (1) {
			printf("Loading data from %s. This will overwrite any existing data. Confirm? ('y' or 'n'): ", filePath);
			char input[8] = "";
			scanf(" %7[^\n]s ", input);
			if (tolower(input[0]) == 'y') {
				printf("Loading database...\n");
				break;
			} else if (tolower(input[0]) == 'n') {
				printf("Load aborted.\n");
				return 0;
			} else {
				printf("Invalid input!\n");
			}
		}
	}

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
	padding[0] = strlen("ID");
	padding[1] = strlen("NAME");
	padding[2] = strlen("AGE");
	padding[3] = strlen("PROGRAM");
	padding[4] = strlen("0.00");
	padding[5] = strlen("GROUP");

	for (size_t i = 0; i < studentDb->count; i++)
	{
		const Student* student = &studentDb->database[i];
		if (strlen(student->id) > padding[0]) {padding[0] = (int) strlen(student->id);}
		if (strlen(student->name) > padding[1]) {padding[1] = (int) strlen(student->name);}
		if (strlen(student->program) > padding[3]) {padding[3] = (int) strlen(student->program);}
		if (student->age > 999) {
			int temp = student->age;
			int newAgePad = 0;
			while (temp > 0) {
				temp /= 10;
				newAgePad++;
			}
			if (newAgePad > padding[2]) {padding[2] = newAgePad;}
		}
	}
	padding[5] = strlen("DOWNTOWN");
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

	char input[256] = "";		// for all inputs
	char studentId [10] = "A0";	// Student's ID
	char studentName [256] = "";		// Student's name
	int studentAge = 0;				// Student's age
	char studentProgram[128] = "";	// Student's program
	float studentGpa = 0;			// Student's grade point average
	Group studentGroup;			// Student's group

	// Get ID
	while (1) {
		// Prompt user for ID
		printf("Enter Student ID (7 digits): A0");
		scanf(" %255[^\n]s ", input);

		// Validate ID formatting
		char* stringPtr; 							//String portion of the input
		strtol(input, &stringPtr, 10);
		if (*stringPtr != '\0') {					// Check whether ID is fully numeric
			printf("Invalid ID: must be numeric\n");
			continue;
		} else if (strlen(input) != 7) {		// Check whether ID is correct language
			printf("Invalid ID: Input must be exactly 7 digits\n");
			continue;
		}

		char tempUserId[10] = "A0"; 				// To add A0 to the beginning of the string
		strcat(tempUserId, input);

		// Check uniqueness
		char duplicate = 0;							// Set to 1 if a duplicate is found
		for (size_t i = 0; i < studentDb->count; i++) {
			if (!strcmp(studentDb->database[i].id, tempUserId)) {
				duplicate = 1;
				break;
			}
		}
		if (duplicate) { // If the number already exist
			printf("Invalid ID: ID '%s' already in use\n", tempUserId);
			continue;
		}

		// If everything passes
		strcpy(studentId, tempUserId);
		break;
	}

	// Get name
	while (1) {
		// Prompt user for name
		printf("Enter Student Name: ");
		scanf(" %255[^\n]s ", input);

		// Strip trailing whitespace
		size_t trail = 0;
		for (size_t i = 0; input[i] != '\0'; i++) {
			if (isblank(input[i])) {
				trail++;
			}
			else trail = 0;
		}
		if (trail != 0) {
			input[strlen(input) - trail] = '\0';
		}

		// Validate name length
		if (strlen(input) > 255 || strlen(input) == 0) { //out of bound
			printf("Invalid name: should be between 1 and 255 characters (inclusive)\n");
			continue;
		}

		strcpy(studentName, input);
		break;
	}

	// Get age
	while (1) {
		// Prompt user for age
		printf("Enter Student Age:");
		scanf(" %255[^\n]s ", &input);

		char* stringPtr; //String portion of the input
		int age = strtol(input, &stringPtr, 10);

		// Validate input
		if (*stringPtr != '\0') {
			printf("Invalid age: must be numeric\n");
			continue;
		} else if (age < 0) {
			printf("Invalid age: must be positive\n");
			continue;
		}

		// Warn if age is unusual
		if (age < 16 || age > 85) {
			char* warning = (age < 16) ? "low" : "high";
			printf("Age (%d) is unusually %s. Is this correct? ('y' or 'n'):", age, warning);
			scanf(" %255[^\n]s ", input);

			if (tolower(input[0]) != 'y') {
				continue;
			}
		}

		studentAge = age;
		break;
	}

	// Get program
	while (1) {
		// Prompt user for program
		printf("Enter Student Program:");
		scanf(" %255[^\n]s ", input);

		// Strip trailing whitespace
		size_t trail = 0;
		for (size_t i = 0; input[i] != '\0'; i++) {
			if (isblank(input[i])) {
				trail++;
			}
			else trail = 0;
		}
		if (trail != 0) {
			input[strlen(input) - trail] = '\0';
		}

		strcpy(studentProgram, input);
		break;
	}

	// Get GPA
	while (1) {
		printf("Enter Student GPA (0.0 to 5.0):");
		scanf(" %255[^\n]s ", input);

		// Validate input
		char* stringPtr; //String portion of the input
		double gpaValue = strtod(input, &stringPtr);
		if (*stringPtr != '\0') { // has other characters than alphabet
			printf("Invalid GPA: must be numeric\n");
			continue;
		}
		if (gpaValue < 0.0 || gpaValue > 5.0) {
			printf("Invalid GPA: must be between 0.0 and 5.0 (inclusive)\n");
			continue;
		}

		studentGpa = (float) gpaValue;
		break;
	}

	// Get group
	while (1) {
		printf("Select student group ('D'owntown or 'B'urnaby):");
		scanf(" %255[^\n]s ", input);

		if (toupper(input[0]) == 'D') {
			studentGroup = DOWNTOWN;
			break;
		} else if (toupper(input[0]) == 'B') {
			studentGroup = BURNABY;
			break;
		} else {
			printf("Invalid group: Must be 'D'owntown or 'B'urnaby\n");
			continue;
		}
	}

	// Create and fill new student struct
	Student newStudent;
	strcpy(newStudent.id, studentId);
	strcpy(newStudent.name, studentName);
	newStudent.age = studentAge;
	strcpy(newStudent.program, studentProgram);
	newStudent.gpa = studentGpa;
	newStudent.group = studentGroup;

	// Get confirmation from user
	printf("New student:\n");
	int padding[10];
	printStudent(&newStudent, padding);
	while (1) {
		printf("\nAdd this student to database? ('y' or 'n'):");
		scanf(" %255[^\n]s ", input);

		if (tolower(input[0]) == 'y') {
			break;
		} else if (tolower(input[0]) == 'n') {
			printf("Aborting add student...\n\n");
			return;
		} else {
			printf("Invalid input!\n");
		}
	}

    // Check capacity of database and resize if needed
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
	// Increment counter
	const size_t index = studentDb->count++;

	// Add student
	studentDb->database[index] = newStudent;
	printf("Student added\n\n");
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
	printLabels(padding);

	// Print Downtown students
	for (size_t i = 0; i < studentDb->count; i++)
	{
		if (studentDb->database[i].group == DOWNTOWN) {
			printStudent(&studentDb->database[i], padding);
		}
	}

	// Print Burnaby students
	for (size_t i = 0; i < studentDb->count; i++)
	{
		if (studentDb->database[i].group == BURNABY) {
			printStudent(&studentDb->database[i], padding);
		}
	}
	printf("\n");
}

/**
 * Prints a student's records if they exist in the database.
 *
 * @param studentDb the database to search
 */
void searchStudent(const StudentDatabase* studentDb) {
	// Get id from user input
	char input[256] = "";
	printf("Please enter the ID of the student you wish to find: ");
	scanf(" %255[^\n]s ", input);

	input[9] = '\0';
	char targetId[10];
	strcpy(targetId, input);
	targetId[0] = (char) toupper(targetId[0]);

	for (size_t i = 0; i < studentDb->count; i++)
	{
		if (!strcmp(studentDb->database[i].id, targetId))
		{
			int padding[10] = {0};
			makePadding(studentDb, padding, 10);
			printLabels(padding);
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
	char input[256] = "";
	printf("Please enter the ID of the student you wish to delete: ");
	scanf(" %255[^\n]s ", input);

	input[9] = '\0';
	char targetId[10];
	strcpy(targetId, input);
	targetId[0] = (char) toupper(targetId[0]);

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

/**
 * Sorts entries in a database.
 *
 * @param studentDb the database to sort
 */
void sortStudent(const StudentDatabase* studentDb) {
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
void listStudents(const StudentDatabase* studentDb) {
	Group studentGroup = {};

	char validInput = 0;
	while (!validInput) {
		char input[256];
		printf("Please enter the group to select ('B' or 'D'): ");
		scanf("%s", input);
		if (strcmp(input, "B") == 0 || strcmp(input, "b") == 0) {
			studentGroup = BURNABY;
			printf("Burnaby:");
			validInput = 1;
		} else if (strcmp(input, "D") == 0 || strcmp(input, "d") == 0) {
			studentGroup = DOWNTOWN;
			printf("Downtown:");
			validInput = 1;
		} else {
			printf("\nInvalid input.\n");
		}
	}

	// Create padding info
	int padding[10] = {0};
	makePadding(studentDb, padding, 10);

	// Print table labels
	printLabels(padding);

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
		printf("Enter a command or 'help':");
		scanf(" %255[^\n]s ", input);
		strToLower(input);

		if (strcmp(input, "exit") == 0) {
			char loop = 1;
			while (loop) {
				loop = 0;
				printf("Save database before exiting? ('y'|'n'|'cancel')\n");
				scanf("%s", input);
				strToLower(input);
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
			printf("Add student\n\n");
			addStudent(&studentDatabase);
		} else if (strcmp(input, "display") == 0) {
			printf("Display students\n\n");
			displayStudents(&studentDatabase);
		} else if (strcmp(input, "delete") == 0) {
			printf("Delete students\n\n");
			deleteStudent(&studentDatabase);
		} else if (strcmp(input, "search") == 0) {
			printf("Search students\n\n");
			searchStudent(&studentDatabase);
		} else if (strcmp(input, "save") == 0) {
			printf("Saving database...\n\n");
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
			printf("Database details\n");
			printf("Number of students: %llu\n", studentDatabase.count);
			printf("Allocated memory: %llu bytes (%llu slots)\n", studentDatabase.size * sizeof(studentDatabase.database), studentDatabase.size);

		} else if (strcmp(input, "help") == 0) {
			printf ("Valid commands:\n");
			printf ("add, display, search, delete, list, sort, save, load, details, exit\n");
		} else if (strcmp(input, "list") == 0) {
			printf("List students\n");
			listStudents(&studentDatabase);
		} else if (strcmp(input, "sort") == 0) {
			printf("Sort students\n");
			sortStudent(&studentDatabase);
		}
		else {
			printf("Unrecognised command. Enter 'help' to see a list of commands.\n");
		}
	}

	// Free memory
	free(studentDatabase.database);
	return 0;
}