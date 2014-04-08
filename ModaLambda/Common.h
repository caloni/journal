#pragma once
#include <vector>

// um empregado que recebe e-mail de aniversário
struct Employee
{
	std::string name;
	std::string address;
	std::string birthDate;
	int age;
};


std::string GetCurrentDate();
void GetEmployees(std::vector<Employee>& employees);
void GetAnniversaryEmployees(std::vector<Employee>& employees);
void SendMail(const Employee& employee);
