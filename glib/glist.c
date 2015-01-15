#include "glib.h"
#include "string.h"

void print_str(gpointer str, gpointer data)
{
	gchar *t_str= (gchar*) str;
	g_printf("%s\n", t_str);
}

gint str_compare(gconstpointer str1, gconstpointer str2)
{
	gchar *t_str1, *t_str2;
	t_str1= (gchar*)str1;
	t_str2= (gchar*)str2;

	return strcmp(t_str1, t_str2);
}

int main()
{
	GList *str_list= NULL, *num_list= NULL;

	str_list= g_list_append(str_list, "first");
	str_list= g_list_append(str_list, "second");
	str_list= g_list_prepend(str_list, "iskey");
	str_list= g_list_insert(str_list, "hello", 1);

	{
		/*This is wrong*/
		char find_str[10];
		GList *t_list= g_list_find(str_list, "second");
		if(t_list== NULL){
			g_printf("1 Element not found.\n");
		}
		else{
			g_printf("1 Element \"%s\" found.\n", t_list->data);
		}

		/*This is wrong too.*/
		memcpy(find_str, "second", 10);
		t_list= g_list_find(str_list, find_str);
		if(t_list== NULL){
			g_printf("2 Element not found.\n");
		}
		else{
			g_printf("2 Element \"%s\" found.\n", t_list->data);
		}

		/*This is the right*/
		memcpy(find_str, "second", 10);
		t_list= g_list_find_custom(str_list, find_str, str_compare);
		if(t_list== NULL){
			g_printf("3 Element not found.\n");
		}
		else{
			g_printf("3 Element \"%s\" found.\n", t_list->data);
		}
	}

	g_printf("Ths str_list length is %d\n", g_list_length(str_list));

	g_list_foreach(str_list, print_str, NULL);
}
