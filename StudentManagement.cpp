#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <stdexcept>
using namespace std;

class Student
{
private:
    int rollNo;
    string name;
    string course;
    string studentClass;
    string contact;

    static int totalStudents;

public:

    
    Student()
    {
        rollNo = 0;
        name = "";
        course = "";
        studentClass = "";
        contact = "";
        totalStudents++;
    }

    
    Student(int r, string n, string c, string cl, string ct)
    {
        rollNo = r;
        name = n;
        course = c;
        studentClass = cl;
        contact = ct;
        totalStudents++;
    }

    
    Student(const Student &s)
    {
        rollNo = s.rollNo;
        name = s.name;
        course = s.course;
        studentClass = s.studentClass;
        contact = s.contact;
        totalStudents++;
    }

    
    virtual ~Student()
    {
        totalStudents--;
    }

    
    inline int getRollNo() const
    {
        return rollNo;
    }

    inline string getName() const
    {
        return name;
    }

    
    void setData(int r, string n, string c, string cl, string ct)
    {
        if(r <= 0)
            throw invalid_argument("Roll number must be positive!");

        rollNo = r;
        name = n;
        course = c;
        studentClass = cl;
        contact = ct;
    }

    
    virtual void display() const
    {
        cout << "\nRoll Number : " << rollNo;
        cout << "\nName        : " << name;
        cout << "\nCourse      : " << course;
        cout << "\nClass       : " << studentClass;
        cout << "\nContact     : " << contact;
        cout << "\n------------------------------";
    }

    
    void search(int r) const
    {
        if(rollNo == r)
            display();
    }

    void search(string n) const
    {
        if(name == n)
            display();
    }

    
    bool operator==(const Student &s) const
    {
        return rollNo == s.rollNo;
    }

    
    void save(ofstream &file) const
    {
        file << rollNo << '\n';
        file << name << '\n';
        file << course << '\n';
        file << studentClass << '\n';
        file << contact << '\n';
    }

    void load(ifstream &file)
    {
        file >> rollNo;
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(file, name);
        getline(file, course);
        getline(file, studentClass);
        getline(file, contact);
    }

    static int getTotalStudents()
    {
        return totalStudents;
    }

    
    friend void studentDetails(const Student &s);
};


int Student::totalStudents = 0;


void studentDetails(const Student &s)
{
    cout << "\nStudent Name: " << s.name;
    cout << "\nRoll Number: " << s.rollNo;
}



class AcademicStudent : public Student
{
private:
    float marks;

public:

    AcademicStudent() : Student()
    {
        marks = 0;
    }

    AcademicStudent(int r, string n, string c,
                   string cl, string ct, float m)
        : Student(r, n, c, cl, ct)
    {
        marks = m;
    }

    
    void display() const override
    {
        Student::display();
        cout << "\nMarks       : " << marks;
        cout << "\n------------------------------";
    }
};



class StudentManagement
{
private:
    Student *students[20];
    int total;

public:

    StudentManagement()
    {
        total = 0;

        for(int i = 0; i < 20; i++)
            students[i] = nullptr;
    }

    ~StudentManagement()
    {
        for(int i = 0; i < total; i++)
        {
            delete students[i];
            students[i] = nullptr;
        }
    }

    
    int findStudent(int rollNo) const
    {
        for(int i = 0; i < total; i++)
        {
            if(students[i]->getRollNo() == rollNo)
                return i;
        }

        return -1;
    }


    void addStudent()
    {
        if(total == 20)
        {
            cout << "\nMaximum limit reached!";
            return;
        }

        int r;
        string n, c, cl, ct;

        cout << "\nEnter Roll Number: ";
        cin >> r;

        if(r <= 0)
            throw invalid_argument("Invalid roll number!");

        if(findStudent(r) != -1)
            throw runtime_error("Roll number already exists!");

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Name: ";
        getline(cin, n);

        cout << "Enter Course: ";
        getline(cin, c);

        cout << "Enter Class: ";
        getline(cin, cl);

        cout << "Enter Contact: ";
        getline(cin, ct);

        
        students[total] = new Student(r, n, c, cl, ct);

        total++;

        cout << "\nStudent added successfully!";
    }


    void displayStudents() const
    {
        if(total == 0)
        {
            cout << "\nNo student records available!";
            return;
        }

        for(int i = 0; i < total; i++)
        {
            students[i]->display();
        }
    }


    void searchStudent()
    {
        int r;

        cout << "\nEnter Roll Number: ";
        cin >> r;

        int index = findStudent(r);

        if(index == -1)
        {
            cout << "\nStudent not found!";
        }
        else
        {
            cout << "\nStudent found!";
            students[index]->display();

            
            studentDetails(*students[index]);
        }
    }

    

    void updateStudent()
    {
        int r;

        cout << "\nEnter Roll Number to Update: ";
        cin >> r;

        int index = findStudent(r);

        if(index == -1)
        {
            cout << "\nStudent not found!";
            return;
        }

        string n, c, cl, ct;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter New Name: ";
        getline(cin, n);

        cout << "Enter New Course: ";
        getline(cin, c);

        cout << "Enter New Class: ";
        getline(cin, cl);

        cout << "Enter New Contact: ";
        getline(cin, ct);

        students[index]->setData(r, n, c, cl, ct);

        cout << "\nStudent updated successfully!";
    }

    

    void deleteStudent()
    {
        int r;

        cout << "\nEnter Roll Number to Delete: ";
        cin >> r;

        int index = findStudent(r);

        if(index == -1)
        {
            cout << "\nStudent not found!";
            return;
        }

        delete students[index];

        for(int i = index; i < total - 1; i++)
        {
            students[i] = students[i + 1];
        }

        students[total - 1] = nullptr;
        total--;

        cout << "\nStudent deleted successfully!";
    }

    

    void saveToFile()
    {
        ofstream file("students.txt");

        if(!file)
            throw runtime_error("Unable to open file!");

        file << total << '\n';

        for(int i = 0; i < total; i++)
        {
            students[i]->save(file);
        }

        file.close();

        cout << "\nRecords saved successfully!";
    }

    void loadFromFile()
    {
        ifstream file("students.txt");

        if(!file)
        {
            cout << "\nNo saved file found!";
            return;
        }

        int count;

        file >> count;

        if(!file || count < 0 || count > 20)
            throw runtime_error("Invalid file data!");

        
        for(int i = 0; i < total; i++)
        {
            delete students[i];
            students[i] = nullptr;
        }

        total = 0;

        for(int i = 0; i < count; i++)
        {
            Student *s = new Student();

            s->load(file);

            if(!file)
            {
                delete s;
                throw runtime_error("Error reading student records!");
            }

            if(findStudent(s->getRollNo()) != -1)
            {
                delete s;
                throw runtime_error("Duplicate roll number in file!");
            }

            students[total] = s;
            total++;
        }

        file.close();

        cout << "\nRecords loaded successfully!";
    }

    

    void menu()
    {
        int choice;

        do
        {
            cout << "\n\n================================";
            cout << "\n     STUDENT MANAGEMENT SYSTEM";
            cout << "\n================================";
            cout << "\n1. Add Student";
            cout << "\n2. Display Students";
            cout << "\n3. Search Student";
            cout << "\n4. Update Student";
            cout << "\n5. Delete Student";
            cout << "\n6. Save Records";
            cout << "\n7. Load Records";
            cout << "\n8. Exit";
            cout << "\n================================";
            cout << "\nEnter Choice: ";

            cin >> choice;

            try
            {
                switch(choice)
                {
                    case 1:
                        addStudent();
                        break;

                    case 2:
                        displayStudents();
                        break;

                    case 3:
                        searchStudent();
                        break;

                    case 4:
                        updateStudent();
                        break;

                    case 5:
                        deleteStudent();
                        break;

                    case 6:
                        saveToFile();
                        break;

                    case 7:
                        loadFromFile();
                        break;

                    case 8:
                        cout << "\nThank you for using the system!";
                        break;

                    default:
                        cout << "\nInvalid choice!";
                }
            }
            catch(const exception &e)
            {
                cout << "\nError: " << e.what();
            }

        } while(choice != 8);
    }
};

int main()
{
    StudentManagement sm;

    sm.menu();

    return 0;
}
