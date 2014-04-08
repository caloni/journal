#include "Common.h"
#include <iostream>
#include <string>


std::string GetCurrentDate()
{
	return "08/04/2014";
}


void GetEmployees(std::vector<Employee>& employees)
{
	Employee emp;

	emp.name = "Joao";
	emp.age = 50;
	emp.birthDate = "08/05/2014";
	employees.push_back(emp);

	emp.name = "Maria";
	emp.age = 63;
	emp.birthDate = "08/04/2014";
	employees.push_back(emp);

	emp.name = "Roberto";
	emp.age = 45;
	emp.birthDate = "08/06/2014";
	employees.push_back(emp);
}


void GetAnniversaryEmployees(std::vector<Employee>& employees)
{
	return GetEmployees(employees);
}


void SendMail(const Employee& employee)
{
	std::cout << "Congrats, " << employee.name << ", for " << employee.age << " year alive and working!\n";
}
