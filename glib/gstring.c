#include "glib.h"
#include "stdio.h"

int main()
{
	GString *my_str;
	GString *my_str2;
	my_str= g_string_new("hello ");
	my_str2= g_string_append(my_str, "iskey.");

	g_printf("My string is %s\n", my_str2->str);

	g_string_ascii_up(my_str2);
	g_printf("My ASCII UP string is %s\n", my_str2->str);
	return 0;
}