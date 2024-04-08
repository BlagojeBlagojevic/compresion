#include<assert.h>
#include<stdio.h>
#include "fastlz.c"
#include<stdlib.h>
#include<string.h>
#include <omp.h>
#include <dirent.h>
#define MAX_FILE_SIZE 9000000    //90MB


#define ARRAY_TYPE uint8_t

typedef struct d {

	ARRAY_TYPE *array;
	size_t index_of_last;  //INDEX OF LAST ELEMET
	size_t _size;          //DEFINE THIS SIZE(IN INIT OF ARRAY)

	} D_ARRAY;



void init_Array(D_ARRAY *array,size_t initialSize) {

	array->array = (ARRAY_TYPE*)calloc(initialSize,sizeof(ARRAY_TYPE));
	array->_size = initialSize;
	array->index_of_last = 0;


	}
void insert_Array(D_ARRAY *array,ARRAY_TYPE elem) {

	if(array->index_of_last == array->_size) {
		array->_size *= 2;
		array->array = (ARRAY_TYPE*)realloc(array->array,sizeof(ARRAY_TYPE)*array->_size);
		if(array->array == NULL)
			assert(0 && "REALOC FAIL!!! ");
		printf("Realloc!!!\n");

		}
	array->array[array->index_of_last++] = elem;

	}

void print_Array(D_ARRAY *array) {

	for(size_t i = 0; i < array->index_of_last; i++) {
		putchar(array->array[i]);
		//printf("%d",array->array[i]);
		}

	}

void update_Index_Of_Last(D_ARRAY *data) {

	size_t cons_time = 0;
	for(size_t i = 0; i < data->_size; i++) {
		//printf("%d ",i);
		data->index_of_last = i;

		if(data->array[i] == 0) {
			cons_time++;

			}
		else {
			cons_time = 0;
			}
		if(cons_time == 100) {
			data->index_of_last-=100;
			break;
			}



		}
	}



void load_Array(D_ARRAY *data,const char *name) {
	FILE *f = fopen(name,"rb");
	if(f == NULL)
		assert(0 && "FAIL !!!");
	fread(data->array,1,data->_size,f);
	data->index_of_last = data->_size;
	update_Index_Of_Last(data);
	fclose(f);
	}

void save_Array(D_ARRAY *data,const char *name) {

	FILE *f = fopen(name,"wb");
	if(f == NULL)
		assert(0 && "FAIL !!!");
	update_Index_Of_Last(data);
	fwrite(data->array,1,data->index_of_last,f);
	//data->index_of_last = data->_size;
	fclose(f);

	}

// WE will  make compresed file into a folder with multiple files
void compression(const char *name,const char *name_saved) {
	char comand[1000] = "mkdir ";
	char *p = strcat(comand,name_saved);
	if(p != comand)
		assert(0 && "SOMTING FAIL !!!");
	system(comand);


	int NUM_OF_THREADS = omp_get_max_threads() - 1;
	if(NUM_OF_THREADS <= 0)
		NUM_OF_THREADS = 1;
	printf("NUM OF ALLOCATED CORE IS %d\n", NUM_OF_THREADS);
	//D_ARRAY INIT
	D_ARRAY decompressed, compressed[NUM_OF_THREADS];

	init_Array(&decompressed,MAX_FILE_SIZE * 2);

	for(size_t i = 0; i < NUM_OF_THREADS; i++)
		init_Array(&compressed[i],MAX_FILE_SIZE);

	//system("pause");
	load_Array(&decompressed,name);
	//print_Array(&decompressed);
	int size[NUM_OF_THREADS];
	//printf("")

	#pragma omp parallel
		{
		#pragma omp single
			{
			for(size_t i = 0; i < (size_t)NUM_OF_THREADS; i++) {
				#pragma omp task
					{
					size[i] = fastlz2_compress(&decompressed.array[i * decompressed.index_of_last / NUM_OF_THREADS]
					                           ,decompressed.index_of_last / NUM_OF_THREADS,compressed[i].array);

					}
				}
			}

		}


	#pragma omp parallel
		{
		#pragma omp single
			{
			for(size_t i = 0; i < (size_t)NUM_OF_THREADS; i++) {
				#pragma omp task
					{
					char name_for_file[1000];
					strcpy_s(name_for_file,100,name_saved);
					strcat_s(name_for_file,1000,"\\");
					char a[10];
					snprintf(a,10,"%d",(int)i);
					strcat_s(name_for_file, 1000, a);
					strcat_s(name_for_file, 1000, ".txt");
					printf("\n%s\n\n", name_for_file);
					compressed[i].index_of_last = size[i];
					//update_Index_Of_Last()
					save_Array(&compressed[i],name_for_file);
					}
				}
			}
		}

	//FREE MEMORY
	free(decompressed.array);
	for(size_t i = 0; i < (size_t)NUM_OF_THREADS; i++)
		free(compressed[i].array);
	}

#define NUM_OF_FILES 20
#define MAX_SIZE_STR 40
#define LOG 1

//

size_t load_files_from_dir(const char* name, char path[NUM_OF_FILES][MAX_SIZE_STR]) {
	DIR *d = opendir(name);
	struct dirent *dir;    //Intermidiead object
	if(d == NULL)
		assert(0 && "NON EXISTENT FILE !!!");
	size_t counter = 0;
	while((dir = readdir(d)) != NULL) {

		strcpy(path[counter++],dir->d_name);
		char pom[20];
		strcpy(pom,name);
		strcat(pom,path[counter - 1]);
		strcpy(path[counter - 1],pom);
#if LOG == 1
		printf("%s\n",path[counter - 1]);
#endif
		assert(counter <= NUM_OF_FILES);
		}
	closedir(d);
	return counter;
	}













void deccompression(const char *name,const char *name_saved) {
	char path[NUM_OF_FILES][MAX_SIZE_STR];      //ACTUAL FILES START FROM 2 SO ALL INDEX STARTS FROM 2 WHY NO IDEA
	size_t  size  = load_files_from_dir(name_saved, path);
	//printf("number of files is %d\n", (int)size - 2);


	int NUM_OF_THREADS = omp_get_max_threads() - 1;
	if(NUM_OF_THREADS <= 0)
		NUM_OF_THREADS = 1;
	printf("NUM OF ALLOCATED CORE IS %d\n", NUM_OF_THREADS);

	//init array
	D_ARRAY decompressed[NUM_OF_THREADS],compressed[NUM_OF_THREADS];

	for(size_t i = 0; i < (size_t)NUM_OF_THREADS; i++) {

		init_Array(&decompressed[i],2 * MAX_FILE_SIZE);
		init_Array(&compressed[i],2 * MAX_FILE_SIZE);

		}

	printf("path %s", path[2]);

	#pragma omp parallel
		{
		#pragma omp single
			{

			for(size_t i = 2; i < (size_t)NUM_OF_THREADS + 2; i++) {

				#pragma omp task
					{
					char name_for_file[1000];
					strcpy_s(name_for_file,1000,name_saved);
					//printf("\n%s\n",name_for_file);
					strcat_s(name_for_file,1000,"\\");
					strcat_s(name_for_file,1000,&path[i][1]);
					printf("\n %s\n\n",name_for_file);
					//system("pause");
					load_Array(&compressed[i - 2],name_for_file);

					}
				}
			}
		}

	//print_Array(&compressed[0]);
	//print_Array(&compressed[1]);
	//print_Array(&compressed[2]);
	//MAIN DECOMPRESION
	int size1[NUM_OF_THREADS];
	#pragma omp parallel
		{
		#pragma omp single
			{
			for(size_t i = 0; i < (size_t)NUM_OF_THREADS; i++) {
				#pragma omp task
					{
					size1[i] = fastlz2_decompress(compressed[i].array,compressed[i].index_of_last
					                              ,decompressed[i].array,MAX_FILE_SIZE * 2);
					printf("\nsize %d\n\n", size1[i]);
					}
				}

			}
		}

	FILE *f = fopen(name,"wb");
	for(size_t i = 0; i < (size_t)NUM_OF_THREADS; i++) {
		update_Index_Of_Last(&decompressed[i]);
		printf(" index of last %d\n",(int)decompressed[i].index_of_last);
		fwrite(decompressed[i].array,1,decompressed[i].index_of_last,f);
		}

	fclose(f);

	//print_Array(&compressed[0]);

	//print_Array(&decompressed[0]);
	//print_Array(&decompressed[1]);
	//print_Array(&decompressed[2]);


	for(size_t i = 0; i < (size_t)NUM_OF_THREADS; i++) {

		free(compressed[i].array);
		free(decompressed[i].array);

		}


	}

int main(int argc, char *argv[]) {


	while(1) {
		printf("What you want to do (1 - compression, 2 - decompression)\n> ");
		char name[100], name_saved[100];
		int choice;
		scanf("%d", &choice);
		if(choice == 1) {
			printf("Input name of file\n>");
			scanf_s("%s", name);

			printf("Input name of file you want to save as\n>");
			scanf_s("%s",name_saved);
			compression(name,name_saved);
			}
		else if(choice == 2) {
			printf("Input name of file\n>");
			scanf_s("%s", name);
			printf("Input name of file you want to save as\n>");
			scanf_s("%s",name_saved);
			deccompression(name_saved,name);
			}
		else {
				break;
			}

		}

	//system("pause");
	}