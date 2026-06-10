#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "students.dat"
#define NAME_LEN 50

// Structure to store student details
typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    float marks;
} Student;

// Function prototypes
void addStudent();
void displayStudents();
void searchStudent();
void deleteStudent();
int isValidName(const char *name);

int main() {
    int choice;

    while (1) {
        printf("\n===== Student Management System =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Delete Student by ID\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // clear buffer
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: printf("Exiting program...\n"); exit(0);
            default: printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

// Validate name (only letters and spaces allowed)
int isValidName(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && !isspace(name[i])) {
            return 0;
        }
    }
    return 1;
}

// Add a new student
void addStudent() {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    Student s;
    printf("Enter Student ID: ");
    if (scanf("%d", &s.id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        fclose(fp);
        return;
    }

    printf("Enter Name: ");
    while (getchar() != '\n'); // clear buffer
    fgets(s.name, NAME_LEN, stdin);
    s.name[strcspn(s.name, "\n")] = '\0'; // remove newline

    if (!isValidName(s.name)) {
        printf("Invalid name. Only letters and spaces allowed.\n");
        fclose(fp);
        return;
    }

    printf("Enter Age: ");
    if (scanf("%d", &s.age) != 1 || s.age <= 0) {
        printf("Invalid age.\n");
        while (getchar() != '\n');
        fclose(fp);
        return;
    }

    printf("Enter Marks: ");
    if (scanf("%f", &s.marks) != 1 || s.marks < 0 || s.marks > 100) {
        printf("Invalid marks. Must be between 0 and 100.\n");
        while (getchar() != '\n');
        fclose(fp);
        return;
    }

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Student added successfully.\n");
}

// Display all students
void displayStudents() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    Student s;
    printf("\n%-10s %-20s %-5s %-6s\n", "ID", "Name", "Age", "Marks");
    printf("---------------------------------------------\n");
    while (fread(&s, sizeof(Student), 1, fp)) {
        printf("%-10d %-20s %-5d %-6.2f\n", s.id, s.name, s.age, s.marks);
    }
    fclose(fp);
}

// Search student by ID
void searchStudent() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int id, found = 0;
    printf("Enter Student ID to search: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        fclose(fp);
        return;
    }

    Student s;
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.id == id) {
            printf("Record Found:\n");
            printf("ID: %d\nName: %s\nAge: %d\nMarks: %.2f\n", s.id, s.name, s.age, s.marks);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }
    fclose(fp);
}

// Delete student by ID
void deleteStudent() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        perror("Error creating temp file");
        fclose(fp);
        return;
    }

    int id, found = 0;
    printf("Enter Student ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        fclose(fp);
        fclose(temp);
        return;
    }

    Student s;
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.id == id) {
            found = 1;
            continue; // skip writing this record
        }
        fwrite(&s, sizeof(Student), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found) {
        printf("Student deleted successfully.\n");
    } else {
        printf("Student with ID %d not found.\n", id);
    }
}
