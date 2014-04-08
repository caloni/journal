#include "Common.h"
#include <algorithm>
#include <vector>
#include <string>


int main()
{
	std::vector<Employee> employees; // um bando de empregados
	std::string currentDate = GetCurrentDate();

	// definindo uma função, como quem não quer nada, dentro de uma função
	auto FindByBithDate = [&](Employee& employee)->bool // <-- tipo de retorno
	{
		return employee.birthDate == currentDate;
	};

	GetEmployees(employees);

	auto findIt = std::find_if(employees.begin(), employees.end(), FindByBithDate);

	while( findIt != employees.end() )
	{
		SendMail(*findIt);
		findIt = std::find_if(findIt + 1, employees.end(), FindByBithDate);
	}
}
