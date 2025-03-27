// By Titus and David
// BCIT 2025

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
	FILE* fp = fopen("../records.bin", "wb");
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
	FILE* fp = fopen("../records.bin", "rb");
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
 * Create a unique student number
 *
 * @param currentNumber an int pointer of the newest made student number without leading zero's
 * @return a char pointer to a new student number or NULL if over 99999999
 */
char* generateStudentNumber(const int* currentNumber) {
	// Over the limit
	if (*currentNumber > 99999999) {
		return NULL;
	}
	const int studentIDLength = 9;				// change if you want to go over 9 digit
	const int width = studentIDLength - 1;
	char* newNumber = (char*)malloc(studentIDLength);
	int count = 0;
	char strStudentNumber[studentIDLength];
	sprintf(strStudentNumber, "%d", *currentNumber+1);	// ending with student number
	strcat(newNumber, "A");				// starting with 'A'
	while (width-strlen(strStudentNumber) > count) {
		strcat(newNumber, "0");			// 0 as fillers
		count++;
	}
	strcat(newNumber, strStudentNumber);
	return newNumber;
}
/**
 * Add a student to the database.
 *
 * @param studentDb the database to add to
 */
void addStudent(StudentDatabase* studentDb) {
	Student newStudent = {"A01234567", "Example Student", 22, "CST", 3.333, DOWNTOWN};

    //prompt and retrieve user input

	//=========================
	//       VALIDATION
	//=========================

	// validate ID
	char userInput[20]; // for all inputs

	int validID = 1;
	while (validID) {
		printf("Student ID must be in the format of A12345678");
		printf("Enter Student ID: ");
		scanf("%s", &userInput);

		char strNumber[8];
		strncpy(strNumber, userInput + 1, 8); // substring [1-8]
		char* end;
		int number = strtol(strNumber, &end, 10); // convert str to int | Needed to enter to DB
		if (userInput[0] != 'A') {
			printf("Invalid student ID: Must begin with A");
			continue;
		}
		if (strlen(userInput) != 9) {
			printf("Invalid student ID: Must be length 9");
			continue;
		}
		if (end != NULL) {
			printf("Invalid student ID: Student ID must only be numerical after A");
			continue;
		}
		validID = 0;
	};
	// validate name
	int validName = 1;
	while (validName) {
		printf("Enter Student Name: ");
		scanf("%s", &userInput);
		//Validation code {...}
		validName = 0;
	}
	// validate age
	int validAge = 1;
	while (validAge) {
		printf("Enter Student Age: ");
		scanf("%s", &userInput);
		//Validation code {...}
		validAge = 0;
	}
	// validate program
	int validProgram = 1;
	while (validProgram) {
		printf("Enter Student Program: ");
		scanf("%s", &userInput);
		//Validation code {...}
		validProgram = 0;
	}
	// validate GPA
	int validGPA = 1;
	while (validGPA) {
		printf("Enter Student GPA: ");
		scanf("%s", &userInput);
		//Validation code {...}
		validGPA = 0;
	}
	// validate Group
	int validGroup = 1;
	while (validGroup) {
		printf("Enter Student Group: ");
		scanf("%s", &userInput);
		//Validation code {...}
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
	const char targetId[10] = "A01234567";

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
	const char targetId[] = "A01234567";

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

void ListGroupMembers() {

}

//Comparator functions

int compareID(void* a, void* b) {
	Student* studentA = (Student*) a;
	Student* studentB = (Student*) b;
	return strcmp(studentA->id, studentB->id);
}

int compareName(void* a, void* b)
{
	Student* studentA = (Student*) a;
	Student* studentB = (Student*) b;

	return strcmp(studentA->name, studentB->name);
}
int compareGPA(void* a, void* b)
{
	Student* studentA = (Student*) a;
	Student* studentB = (Student*) b;

	if (studentA->gpa > studentB->gpa) {return 1;}
	if (studentA->gpa < studentB->gpa) {return -1;}
	return 0;
}

void sortStudent() {

}

int main() {
	// Initialise the student database
	StudentDatabase studentDatabase;
	studentDatabase.database = malloc(INCREASE_SIZE * sizeof(Student));
	studentDatabase.size = INCREASE_SIZE;
	studentDatabase.count = 0;

	// Demo
	displayStudents(&studentDatabase);

	addStudent(&studentDatabase);
	displayStudents(&studentDatabase);
	searchStudent(&studentDatabase);

	deleteStudent(&studentDatabase);
	displayStudents(&studentDatabase);
	searchStudent(&studentDatabase);

	// Free memory
	free(studentDatabase.database);
	return 0;
}