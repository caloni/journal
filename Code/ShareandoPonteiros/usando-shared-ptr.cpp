#include <memory>
#include <string>
#include <iostream>

struct Person
{
    Person() { std::cout << "Person created\n"; }
    ~Person() { std::cout << "Person destroyed\n"; }
    std::string name;
    std::string surname;
    int age;
    std::string phone;
};

typedef std::shared_ptr<Person> PersonRef;


PersonRef CreatePerson()
{
    return PersonRef(new Person);
}

void GetName(PersonRef person)
{
    person->name = "Carl";
}

void GetSurName(PersonRef person)
{
    person->surname = "Sagan";
}

void GetAge(PersonRef person)
{
    person->age = 79;
}

void GetPhone(PersonRef person)
{
    person->phone = "+01 042 4242-4242";
}

void PrintPerson(PersonRef person)
{
    std::cout << "Name: " << person->name << " " << person->surname
        << "\nAge: " << person->age
        << "\nPhone: " << person->phone
        << std::endl;
}

void CreateAndPrintPerson()
{
    PersonRef person = CreatePerson();
    GetName(person);
    GetSurName(person);
    GetAge(person);
    GetPhone(person);
    PrintPerson(person);
}

int main()
{
    CreateAndPrintPerson();
}

