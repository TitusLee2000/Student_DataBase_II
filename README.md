[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/sqF2PIyY)
# COMP-2510 Project Phase II
## Your name:
- Titus Lee
- David Anstee

## Student Information Management System - Advanced
### Features:

Add Student – Allow users to enter a student's name, ID, age, program, GPA, and group (Group BBY or Group DTC).

Display Students – Show all stored student records, categorized by group.

Search Student – Find a student by ID and display their group.

Delete Student – Remove a student record by ID.

List Group Members – Display all students in a specified group.

Sort Students – Sort all student records by user's choice of key and order.
Exit – Close the application.

Data persistence  – Command line argument for the file name to store data.
### Console Menu Items:

**Add Student** – Prompt for name, ID, age, program, GPA, and group (BBY or DTC), then store the record.

**Display All Students** – Show all student records, categorized by Group BBY and Group DTC.

**Search Student by ID** – Prompt for student ID, search the records, and display details if found.

**Delete Student by ID** – Prompt for student ID, search, and remove the record if found.

**List Students by Group** – Prompt for group (B - BBY or D - DTC) and display all students in that group.

**Sort Students** – Sort all student records by user's choice of key and order.
Provide a submenu for:

- selecting the sorting key: ID, name, or GPA.
- selecting the sorting order: ascending or descending.

*The sorting should affect both option 2 (Display) and option 5 (List).
The sorted order will NOT persist after the program is restarted.
Exit Application – Terminate the program.*

### Technical Requirements:

- Implement using the C programming language.

- Use struct and arrays to store student information for both groups.

- Provide a simple menu-driven interface using printf and scanf.

- Ensure proper input validation and error handling.

### Constraints:

- The maximum number of students stored in memory must be unlimited. [Hint: to use dynamic memory (de)allocation]

- The program should handle invalid inputs gracefully.

## Valid ID Requirement:
IDs must follow the BCIT format—starting with 'A' or 'a' and followed by exactly eight digits.
IDs of incorrect length are invalid.
Global Variable Restriction:
The use of global variables is strictly prohibited.
Any occurrence will result in a zero mark for this project (Project Phase II).
Struct must be used for student records.

File storage must be used.
NO late submissions will be accepted, and no extensions will be granted for any reason.

### Expected Outcome:

A console-based application that enables efficient management of student records, categorizing them into two groups 


