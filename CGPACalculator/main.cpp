#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

// Stores information about one course
struct Course
{
    string name;
    char grade;
    int creditHours;
};

// Converts a letter grade into grade points
// A = 5, B = 4, C = 3, D = 2, E = 1, F = 0
double gradeToPoint(char grade)
{
    grade = toupper(grade);

    switch (grade)
    {
        case 'A': return 5.0;
        case 'B': return 4.0;
        case 'C': return 3.0;
        case 'D': return 2.0;
        case 'E': return 1.0;
        case 'F': return 0.0;
        default: return -1.0;
    }
}

int main()
{
    int numCourses;

    cout << "===== CGPA Calculator =====" << endl;
    cout << "Enter number of courses this semester: ";
    cin >> numCourses;

    if (numCourses <= 0)
    {
        cout << "Invalid number of courses." << endl;
        return 0;
    }

    vector<Course> courses(numCourses);

    double totalCreditHours = 0;
    double totalGradePoints = 0;

    // Get information for each course
    for (int i = 0; i < numCourses; i++)
    {
        cout << endl;
        cout << "Course " << i + 1 << endl;

        cout << "Course name: ";
        cin >> courses[i].name;

        // Keep asking until a valid grade is entered
        while (true)
        {
            cout << "Grade (A/B/C/D/E/F): ";
            cin >> courses[i].grade;

            if (gradeToPoint(courses[i].grade) != -1.0)
            {
                break;
            }

            cout << "Invalid grade. Please try again." << endl;
        }

        cout << "Credit hours: ";
        cin >> courses[i].creditHours;

        // Convert the grade into grade points
        double gradePoint = gradeToPoint(courses[i].grade);

        // Grade points are multiplied by the course credit hours
        totalGradePoints += gradePoint * courses[i].creditHours;

        // Add the course credit hours to the total
        totalCreditHours += courses[i].creditHours;
    }

    // Calculate GPA
    double gpa = totalGradePoints / totalCreditHours;

    cout << endl;
    cout << "===== Course Summary =====" << endl;

    // Display all courses
    for (int i = 0; i < numCourses; i++)
    {
        cout << courses[i].name
             << " | Grade: " << courses[i].grade
             << " | Credit Hours: " << courses[i].creditHours
             << endl;
    }

    cout << endl;
    cout << "Total Credit Hours: " << totalCreditHours << endl;
    cout << "Semester GPA: " << gpa << endl;

    // Since this program handles one semester,
    // the GPA is also the CGPA for now.
    cout << "Overall CGPA: " << gpa << endl;

    return 0;
}