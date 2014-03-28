#pragma once
#include <vector>

// um empregado que recebe e-mail de aniversário
struct Employee
{
	std::string name;
	std::string address;
	int age;
};


void GetAnniversaryEmployees(std::vector<Employee>& employees);
void SendMail(const Employee& employee);
