#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct foo_object
{
	char *name;
};

struct foo_object *foo_create(char* name)
{
	struct foo_object *new_foo_obj;
	new_foo_obj= malloc(sizeof(struct foo_object));

	new_foo_obj->name= strdup(name);
	return new_foo_obj;
}
int foo_do_something(struct foo_object *this, void *data)
{
	printf("foo_object name is %s\n", this->name);
}

int foo_release(struct foo_object *this)
{
	printf("foo_object will be released.\n");

	free(this->name);
	free(this);
}

/*
int main()
{
	struct foo_object *foo;
	foo= foo_create("xiaoxie");
	foo_do_something(foo, NULL);
	foo_release(foo);
}
*/