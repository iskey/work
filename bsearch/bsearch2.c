#include "stdlib.h"
#include "stdio.h"

struct MyData{
	int Integer;
	int decimal;
};

//typedef int(*compar_func)(const void*, const void*);
//compar_func MyCompare1;

int MyCompare(const void* data1, const void* data2)
{
	struct MyData *cmp1, *cmp2;
	cmp1= *(struct Mydata **)data1;
	cmp2= *(struct Mydata **)data2;

	int ret;
	ret= cmp1->Integer- cmp2->Integer;
	if(0== ret)
		return (cmp1->decimal- cmp2->decimal);
	else
		return ret;
}

struct MyData **g_data;

#define DATA_NUM 100

int main()
{
	int i;

	g_data= malloc(DATA_NUM* sizeof(struct MyData**));

	for(i= 0; i< DATA_NUM; i++)
	{
		*(g_data+ i)= malloc(sizeof(struct MyData));
		(*(g_data+ i))->Integer= i*rand()%100 + 100;
		(*(g_data+ i))->decimal= (i*i+rand())%100 + 100;
	}

	qsort(g_data, DATA_NUM, sizeof(struct MyData**), MyCompare);

	printf("My data is :");

	for(i= 0; i< DATA_NUM; i++)
	{
		struct MyData *t_data;
		t_data= *(g_data+ i);
		printf("%d.%d ",t_data->Integer, t_data->decimal);
	}
	printf("\n");

	struct MyData *key= malloc(sizeof(struct MyData));
	struct MyData **Searched;

	printf("TIP: Please input the data you want to search:");
	scanf("%d.%d",&key->Integer,&key->decimal);
	printf("TIP: You want to search %d.%d\n",key->Integer, key->decimal);

	Searched= bsearch(&key, g_data, DATA_NUM, sizeof(struct MyData*), MyCompare);

	if(Searched!= NULL)
		printf("RESULT: Searched value is:%d.%d\n",(**Searched).Integer, (*Searched)->decimal);
	else
		printf("RESULT: Not Searched.\n");

	return 0;
}