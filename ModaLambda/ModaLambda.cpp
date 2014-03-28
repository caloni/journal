#include "Common.h"
#include <algorithm>
#include <vector>
#include <string>


int main()
{
	std::vector<Employee> employees; // um bando de empregados
	GetAnniversaryEmployees(employees);

	std::for_each(employees.begin(), employees.end(), [&](Employee& employee)
	{
		employee.age += 1;
		SendMail(employee);
	}); // note o final com chave e parêntese, fechando a função e a chamada do for_each
}
