#include <iostream>
#include "Server.h"

int main(int argc, char *argv[])
{
	Server server = Server(2137);

	std::cout << "Success" << std::endl;
}