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
	char name[256];		// Student's legal name
	char id[10];		// Student's BCIT ID
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

void addStudent(StudentDatabase* studentDb) {
    //prompt and retrieve user input

    //validation

    //add to list
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