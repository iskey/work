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
	const struct MyData *cmp1, *cmp2;
	cmp1= (const struct Mydata *)data1;
	cmp2= (const struct Mydata *)data2;

	int ret;
	ret= cmp1->Integer- cmp2->Integer;
	if(0== ret)
		return (cmp1->decimal- cmp2->decimal);
	else
		return ret;
}

struct MyData g_data[]={{1,2},{3,4},{1,1},{5,2},{2,3},{1,8}};

int main()
{
	int i;

	/*Sort global array.*/
	qsort(g_data, sizeof(g_data)/sizeof(struct MyData),
		sizeof(struct MyData),
		MyCompare);

	/*Print global array.*/
	printf("My data is :");
	for(i= 0; i<sizeof(g_data)/sizeof(struct MyData); i++)
	{
		printf("%d.%d ",g_data[i].Integer, g_data[i].decimal);
	}
	printf("\n");

	struct MyData key;
	struct MyData *Searched;
	/*Get key to be searched.*/
	printf("TIP: Please input the data you want to search:");
	scanf("%d.%d",&key.Integer,&key.decimal);
	printf("TIP: You want to search %d.%d\n",key.Integer, key.decimal);
	//key.Integer= 2;
	//key.decimal= 3;

	/*Search key.*/
	Searched= bsearch(&key, g_data, sizeof(g_data)/sizeof(struct MyData),
		sizeof(struct MyData), MyCompare);
	if(Searched!= NULL)
		printf("RESULT: Searched value is:%d.%d\n",Searched->Integer, Searched->decimal);
	else
		printf("RESULT: Not Searched.\n");

	return 0;
}