#include "glib.h"

typedef struct _STUDENT_
{
	gchar *name;
	guint age;
}STUDENT;

int main()
{
	GArray *array;

	array= g_array_new(FALSE, FALSE, sizeof(STUDENT));
	STUDENT stu1={"xieych", 28};
	STUDENT stu2={"zhangg", 27};
	STUDENT stu3={"lijj", 30};
	STUDENT stu4={"yangf",32};

	g_array_append_val(array, stu1);
	g_array_append_val(array, stu2);
	g_array_append_val(array, stu3);
	g_array_append_val(array, stu4);

	STUDENT *read;
	read= &g_array_index(array, STUDENT, 2);
	g_printf("The array index 2 is name:%s age:%d\n",read->name, read->age);

	return 0;
}