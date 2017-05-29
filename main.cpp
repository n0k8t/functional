#include <iostream>

#include <string>
#include <vector>
#include <map>

#include <algorithm>
#include <numeric>
#include <math.h>

enum Grade
{
    F = 1,
    D,
    C,
    B,
    A
};

struct Student
{
    std::string FIO;
    std::string Group;
    std::map<std::string, Grade> Grades;
};

std::vector<Student> LoadStudents()
{
    return{
            {
                    "Anna", "G7",
                    {
                            { "math", Grade::A },
                            { "cs", Grade::A },
                            { "english", Grade::A },
                            { "dutch", Grade::B },
                    }
            },
            {
                    "Petr", "G7",
                    {
                            { "math", Grade::B },
                            { "cs", Grade::A },
                            { "dutch", Grade::C },
                    }
            },
            {
                    "Vova", "G1",
                    {
                            { "math", Grade::F },
                            { "english", Grade::F },
                    }
            },
            {
                    "Vasya", "G3",
                    {
                            { "math", Grade::F },
                            { "english", Grade::F },
                            { "cs", Grade::B },
                            { "dutch", Grade::C },
                    }
            },
            {
                    "Anton", "G4",
                    {
                            { "math", Grade::F },
                            { "english", Grade::F },
                            { "dutch", Grade::C },
                            { "cs", Grade::C },
                    }
            },
            {
                    "Masha", "G2",
                    {
                            { "math", Grade::A },
                    }
            },
            {
                    "Sasha", "G6",
                    {
                            { "english", Grade::F },
                            { "russian", Grade::F },
                            { "dutch", Grade::F },
                    }
            },
            {
                    "Jeka", "G2",
                    {
                            { "english", Grade::A },
                    }
            },
            {
                    "Sveta", "G3",
                    {
                    }
            }
    };
}

struct MathB_csA //#5
{
    std::vector<Student const*> students;

    void operator()(Student const& s)
    {
        auto it = s.Grades.find("math");
        if (it == s.Grades.end() || it->second != Grade::B) {
            return;
        }
        it = s.Grades.find("cs");
        if (it == s.Grades.end() || it->second != Grade::A) {
            return;
        }

        students.push_back(&s);
    }
};

struct GetStuden //#4
{
    std::string FIO[32];
    void operator()(Student& s)
    {
        static int i = 0;
        if (s.Group == "G7" )
        {
            FIO[i] = s.FIO;
            i++;
        }
    }
};

struct MaxSubjec //#3
{
    std::string name;
    int max;
    bool check = true;

    int myMax(Student& s)
    {
        int sMax = 0;
        for (auto it = s.Grades.begin(); it != s.Grades.end(); ++it)
        {
            sMax ++;
        }
        return sMax;
    }

    void operator()(Student& s)
    {
        if(check)
        {
            for (auto it = s.Grades.begin(); it != s.Grades.end(); ++it)
            {
                name = s.FIO;
                max ++;
                check = false;
            }
        }
        if(s.Grades.begin() != s.Grades.end() && !check)
        {
            if (myMax(s) > max && !check)
            {
                name = s.FIO;
                max = myMax(s);
            }
        }

    }
};

struct WorstSred //#2
{
    std::string name;
    float sred;
    bool check = true;

    float mySredn(Student& s)
    {
        int sum = 0;
        int k = 0;
        float res;
        for (auto it = s.Grades.begin(); it != s.Grades.end(); ++it)
        {
            sum += it->second;
            k++;
        }
        res = sum/k;
        return res;
    }

    void operator()( Student& s)
    {
        if(check && s.Grades.begin() != s.Grades.end())
        {
            name = s.FIO;
            check = false;
            sred = mySredn(s);
        }
        if(s.Grades.begin() != s.Grades.end())
        {
            if (mySredn(s) < sred && !check)
            {
                name = s.FIO;
                sred = mySredn(s);
            }
        }
    }
};

struct BestMathS //#1
{
    std::string name;
    int mark;
    bool check = true;

    void operator()(const Student& s)
    {
        if(check && s.Grades.find("math") != s.Grades.end())
        {
            name = s.FIO;
            mark = s.Grades.at("math");
            check = false;
        }
        auto it = s.Grades.find("math");
        if(it != s.Grades.end())
        {
            if (s.Grades.at("math") > mark && !check)
            {
                name = s.FIO;
                mark = s.Grades.at("math");
            }
        }

    }
};

struct AddLiterature
{
    void operator()(Student& s)
    {
        float avg = 0;
        if (s.Grades.size() != 0) {
            for (auto it : s.Grades) {
                avg += it.second;
            }
            avg /= s.Grades.size();
        }
        else {
            avg = Grade::F;
        }

        s.Grades["Russian literature"] = static_cast<Grade>(static_cast<size_t>(ceil(avg)));
    }
};

struct PrintDutch
{
    void operator()(const Student& s) const
    {
        auto it = s.Grades.find("dutch");
        if (it == s.Grades.end()) {
            std::cout << s.FIO << " hasn't mark!" << std::endl;
        }
        else {
            std::cout << s.FIO << " has " << it->second << std::endl;;
        }
    }
};

struct FindStudentGroup
{
    std::string neededGroup;
    bool operator()(const Student& s) const
    {
        return s.Group == neededGroup;
    }
};

struct PrinterStudent
{
    void operator()(const Student& s) const
    {
        std::cout << s.FIO << " studies in " << s.Group << std::endl;
    }
};


struct GradeEqual
{
    Grade grade = Grade::A;

    bool operator()(const std::pair<std::string, Grade> & el) const
    {
        return el.second == grade;
    }
};


int main()
{
    std::vector<Student> students = LoadStudents();

    // find students from G1
    FindStudentGroup finderObj;
    finderObj.neededGroup = "G1";
    auto it = std::find_if(students.begin(), students.end(), finderObj);
    if (it != students.end())
        std::cout << it->FIO << std::endl;


    // find students from G7
    it = std::find_if(students.begin(), students.end(), FindStudentGroup{"G7"});
    if (it != students.end())
        std::cout << it->FIO << std::endl;


    // print info about all students
    std::for_each(students.begin(), students.end(), PrinterStudent());


    // count of grades eq 'A' of the student from G7
    auto cnt = std::count_if(it->Grades.begin(), it->Grades.end(), GradeEqual {});
    std::cout << cnt << std::endl;

    // count of grades eq 'B' of the student from G7
    auto cnt2 = std::count_if(it->Grades.begin(), it->Grades.end(),
            // lambda
                              [](const std::pair<std::string, Grade> &el) {
                                  return el.second == Grade::B;
                              }
    );
    std::cout << cnt2 << std::endl << std::endl;

    // MY CODE
    std::cout << "My part" << std::endl << std::endl << std::endl;

    std::cout << "#1" << std::endl;
    auto mathBest = std::for_each(students.begin(), students.end(), BestMathS());
    std::cout << "Best math student: " << mathBest.name << std::endl << "Mark: " << mathBest.mark << std::endl;

    std::cout << std::endl << "#2" << std::endl;
    auto worstStud = std::for_each(students.begin(), students.end(), WorstSred());
    std::cout << "Worst student: " << worstStud.name << std::endl << "Sredn: " << worstStud.sred << std::endl;

    std::cout << std::endl << "#3" << std::endl;
    auto maxSubjStudent = std::for_each(students.begin(), students.end(), MaxSubjec());
    std::cout << "Student with max subj: " << maxSubjStudent.name << std::endl << "Ammount of them: "
              << maxSubjStudent.max << std::endl;

    //std::cout << std::endl << "#4" << std::endl;
    std::for_each(students.begin(), students.end(), GetStuden());

    std::cout << std::endl << "#5" << std::endl;
    auto res = std::for_each(students.begin(), students.end(), MathB_csA());
    for (auto &s : res.students) {
        std::cout << s->FIO << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl;
    std::for_each(students.begin(), students.end(), AddLiterature());
    std::for_each(students.begin(), students.end(), PrintDutch());
}