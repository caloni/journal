#include "Common.h"
#include <algorithm>
#include <vector>
#include <string>


void NewYearMail(Employee& employee)
{
	// isso só tem duas linhas, mas poderia ter... 10!
	employee.age += 1;
	SendMail(employee);
}


int main()
{
	std::vector<Employee> employees; // um bando de empregados
	GetAnniversaryEmployees(employees);
	std::for_each(employees.begin(), employees.end(), NewYearMail);
}
