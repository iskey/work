#include "stdio.h"

#ifndef _FOO_BOJECT_
#define _FOO_BOJECT_

struct foo_object;

struct foo_object* foo_create(char *name);
int foo_do_something(struct foo_object *, void *data);
int foo_release(struct foo_object *);

#endif/*End of _FOO_BOJECT_*/
