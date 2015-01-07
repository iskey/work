#include "c_header.h"


extern "C" 
{	
	
int test_print(void)
{
	is_print(3);
	return 0;
}

}


int main(int argc, char** argv)
{
	test_print();
	return 0;
}
