// By Titus and David
// BCIT 2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The number of elements to increment or decrement the database by
#define INC_SIZE 50

/**
 * Enum of BCIT groups. Each group corresponds to a campus
 */
typedef enum Group {
	DOWNTOWN,
	BURNABY
} Group;

/**
 * Struct representing a student at BCIT
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
 * Struct representing a database of students
 */
typedef struct StudentDatabase
{
	Student* database;				// Array of students
	size_t size;					// Current max capacity of database
	size_t count;					// Current number of students in database
} StudentDatabase;

/**
 * Attempts to increment the size of a student database
 *
 * @param studentDb the student database to be incremented
 * @return 1 if successful, 0 otherwise
 */
char incrementDatabase(StudentDatabase* studentDb)
{
	// Create copy of database with greater size
	const size_t newSize = studentDb->size + INC_SIZE;
	Student* newDb = realloc(studentDb, sizeof(Student) * newSize);

	// Reallocation might fail, so we test to make sure newDb is valid
	if (newDb != NULL)
	{
		free(studentDb->database);
		studentDb->database = newDb;
		studentDb->size = newSize;
		return 1;
	}
	// Return false if reallocation failed
	else return 0;
}

/**
 * Creates data for padding space when printing students
 *
 * @param studentDb the database
 * @param padding array of padding data, modified in-place
 * @param pSize size of array of padding data
 */
void makePadding(const StudentDatabase* studentDb, int padding[], const size_t pSize)
{
	for (size_t i = 0; i < studentDb->size; i++)
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

void addStudent(StudentDatabase* studentDb) {
    //prompt and retrieve user input
	Student newStudent = {"A01234567", "Example Student", 22, "Computer Systems Technology", 4.0f, DOWNTOWN};
	//validation

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

void DisplayStudent() {

}

struct Student searchStudent() {

}

void deleteStudent() {

}

void ListGroupMembers() {

}
//helper
void sortbyID() {}
void sortbyName() {}
void sortbyGPA() {}
//ascending
//decending
void sortStudent() {

}

int main() {
	// Initialise the student database
	StudentDatabase studentDatabase;
	studentDatabase.database = malloc(INC_SIZE * sizeof(Student));
	studentDatabase.size = INC_SIZE;
	studentDatabase.count = 0;

	// Free memory
	free(studentDatabase.database);
	return 0;
}