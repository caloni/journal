#include "Common.h"
#include <iostream>
#include <string>


void GetAnniversaryEmployees(std::vector<Employee>& employees)
{
	Employee emp;

	emp.name = "Joao";
	emp.age = 50;
	employees.push_back(emp);

	emp.name = "Maria";
	emp.age = 63;
	employees.push_back(emp);

	emp.name = "Roberto";
	emp.age = 45;
	employees.push_back(emp);
}


void SendMail(const Employee& employee)
{
	std::cout << "Congrats, " << employee.name << ", for " << employee.age << " year alive and working!\n";
}
