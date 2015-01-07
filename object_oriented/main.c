#include "foo.h"

int main()
{
	struct foo_object *foo;

	foo= foo_create("xiaoxie ni hao.");
	foo_do_something(foo, NULL);
	foo_release(foo);

	return 0;
}
