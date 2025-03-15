// By Titus and David
// BCIT 2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void addStudent() {
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

}