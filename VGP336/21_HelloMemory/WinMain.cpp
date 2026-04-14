#include <RedSnowEngine/Inc/RedSnowEngine.h>

using namespace RedSnowEngine;
using namespace RedSnowEngine::Core;

static int sUniquueId = 0;

class Student
{
public:
    Student()
    {
        mId = ++sUniquueId;
        mMark = 60 + (rand() % 41);
        mName = "NoName_" + std::to_string(mId);
    }
    Student(const std::string& name) : mName(name)
    {
        mId = ++sUniquueId;
        mMark = 60 + (rand() % 41);
    }
    Student(const std::string& name, int mark) : mName(name), mMark(mark)
    {
        mId = ++sUniquueId;
    }
    void Log()
    {
        LOG("Name: %s - ID: %d - Mark: %d", mName.c_str(), mId, mMark);
    }

private:
    int mId = 0;
    int mMark = 0;
    std::string mName;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 100);

    std::vector<Student*> students;

    for (uint32_t i = 0; i < 50; ++i)
    {
        std::string newStudentName = "NewStudent" + std::to_string(i);
        Student* newStudent = studentPool.New(newStudentName, 75);
        newStudent->Log();
        students.push_back(newStudent);
    }

    for (uint32_t i = 0; i < 30; ++i)
    {
        Student* student = students.back();
        studentPool.Delete(student);
        students.pop_back();
    }

    for (uint32_t i = 0; i < 20; ++i)
    {
        Student* newStudent = studentPool.New();
        newStudent->Log();
        students.push_back(newStudent);
    }

    for (Student* student : students)
    {
        student->Log();
        studentPool.Delete(student);
    }
    students.clear();

    return 0;
}