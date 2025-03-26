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
	char name[255];		// Student's legal name
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
	Student newStudent = {"A01234567", "Example Student", 22, "CST", 3.333, DOWNTOWN};

    //prompt and retrieve user input

	//=========================
	//       VALIDATION
	//=========================

	char userInput[255];		// for all inputs
	char studentID [10];		// Student's ID
	char studentName [255];		// Student's name
	int studentAge;				// Student's age
	char studentProgram[128];	// Student's program
	float studentGpa;			// Student's grade point average
	Group studentGroup;			// Student's group

	int validID = 1;
	// validate ID
	while (validID) {
		printf("Enter Student ID by number [7 digits]: ");
		scanf("%s", &userInput);

		char* stringPtr; //String portion of the input
		strtol(userInput, &stringPtr, 10);
		if (strlen(userInput) != 7) {
			printf("Invalid student ID: Must be length 7");
			continue;
		}
		if (*stringPtr != '\0') {
			printf("Invalid student ID: ID must be numerical");
			continue;
		}
		//check uniqueness
		int flag = 0;
		for (size_t i = 0; i < studentDb->count; i++)
		{
			char tempUserID[100]; // To add A0 to the beginning of the string
			strcpy(tempUserID, "A0");
			strcpy(tempUserID, userInput);
			if (!strcmp(studentDb->database[i].id, tempUserID))
			{
				printf("Invalid student ID: student ID already exist");
				flag = 1;
				break;
			}
		}
		if (flag) { // if the number already exist
			continue;
		}
		validID = 0;
		// If everything passes
		strcpy(studentID, "A0");
		strcpy(studentID, userInput); 
	};
	// validate name
	int validName = 1;
	while (validName) {
		printf("Enter Student Name: ");
		scanf("%s", &userInput);
		char* stringPtr; //String portion of the input
		strtol(userInput, &stringPtr, 10);
		if (strlen(userInput) < 20 || strlen(userInput) == 0) { //out of bound
			printf("Invalid student Name: length must be less than 20");
			continue;
		}
		int flag = 0;
		for (int i = 0; userInput[i] != '\0'; i++) {
			if (!isalpha(userInput[i])) {
				printf("Invalid student Name: Name must only contain alphabetical characters");
				flag = 1;
				break;
			}
		}
		if (flag) { // reset question if student name contains integer
			continue;
		}
		if (*stringPtr == '\0') {
			printf("Invalid student Name: Name must only contain alphabetical characters");
		}
		validName = 0;
		strcpy(studentName, userInput);
	}
	// validate age
	int validAge = 1;
	while (validAge) {
		printf("Enter Student Age: ");
		scanf("%s", &userInput);
		char* stringPtr; //String portion of the input
		int number = strtol(userInput, &stringPtr, 10);
		if (*stringPtr != '\0') {
			printf("Invalid student Age: Age must be numeric");
			continue;
		}
		if (number < 18) {
			printf("Invalid student Age: Age must be greater than 18");
			continue;
		}
		validAge = 0;
		studentAge = userInput;
	}
	// validate program
	int validProgram = 1;
	while (validProgram) {
		printf("Enter Student Program: ");
		scanf("%s", &userInput);
		char* stringPtr; //String portion of the input
		strtol(userInput, &stringPtr, 10);
		if (*stringPtr == '\0') { // check for null and alphabetic character
			printf("Invalid student Program: Program can only contain alphabetical characters");
		}
		validProgram = 0;
		strcpy(studentProgram, userInput);
	}
	// validate GPA
	int validGPA = 1;
	while (validGPA) {
		printf("Enter Student GPA between [0.0 - 5.0]: ");
		scanf("%s", &userInput);
		char* stringPtr; //String portion of the input
		double gpaValue = strtod(userInput, &stringPtr);
		if (*stringPtr != '\0') { // has other characters than alphabet
			printf("Invalid student GPA: GPA must be a double");
			continue;
		}
		if (gpaValue <= 0.0 || gpaValue >= 5.0) {
			printf("Invalid student GPA: GPA range must be 0.0 to 5.0");
		}
		validGPA = 0;
		studentGpa = gpaValue;
	}
	// validate Group
	int validGroup = 1;
	while (validGroup) {
        printf("[D]-Downtown [B]-Burnaby\n");
		printf("Enter Student Group ['D', 'B']: ");
		scanf("%s", &userInput);
		char* stringPtr; //String portion of the input
		int number = strtol(userInput, &stringPtr, 10);
		if (stringPtr == '\0') { // empty or has other characters than alphabet
			printf("Invalid student Group: Group must be 'D' or 'B'");
		}
		if (toupper(*stringPtr) == 'D') {
			studentGroup = DOWNTOWN;
		} else if (toupper(*stringPtr) != 'B') {
			studentGroup = BURNABY;
		} else {
			printf("Invalid student Group: Group must be 'D' or 'B'");
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