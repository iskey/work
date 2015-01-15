#include "glib.h"
#include "stdio.h"

void print_key_value(gpointer key, gpointer value, gpointer user_data)
{
	g_printf("%s\t --- \t%s \n", (gchar*)key, (gchar*)value);
}

int main()
{
	GHashTable *hash_table= NULL;

	hash_table= g_hash_table_new(g_str_hash, g_str_equal);
	if(hash_table== NULL)
	{
		g_printf("create hash table failed.\n");
		return -1;
	}

	g_hash_table_insert(hash_table, "iskey", "xiaoxie");
	g_hash_table_insert(hash_table, "ljj", "lijunjie");
	g_hash_table_insert(hash_table, "zhg", "zhangge");
	g_hash_table_insert(hash_table, "yf", "yangfan");
	g_hash_table_foreach(hash_table, print_key_value, NULL);

	g_hash_table_replace(hash_table, "iskey", "xieyichao");
	g_printf("New hash table:\n");
	g_hash_table_foreach(hash_table, print_key_value, NULL);
	return 0;
}