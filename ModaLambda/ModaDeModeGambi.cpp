#include "Common.h"
#include <algorithm>
#include <vector>
#include <string>


int main()
{
	struct NewYearMail { void operator()(Employee& employee)
	{
		employee.age += 1;
		SendMail(employee);
	}}; // note o final com chaves-duplas, fechando a função e a struct

	std::vector<Employee> employees; // um bando de empregados
	GetAnniversaryEmployees(employees);
	std::for_each(employees.begin(), employees.end(), NewYearMail());
}
