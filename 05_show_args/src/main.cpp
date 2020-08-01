#include <iostream>

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "No program arguments found." << std::endl;
		return 1;
	}
	for (size_t index = 1; index < argc; index++)
	{
		std::cout << argv[index] << std::endl;
	}
	return 0;
}