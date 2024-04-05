#include<assert.h>
#include<stdio.h>
#include "fastlz.c"
#include<stdlib.h>
#include <pthread.h>

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
	
void update_Index_Of_Last(D_ARRAY *data){
	
	size_t cons_time = 0;
	for(size_t i = 0;i < data->_size;i++){
		//printf("%d ",i);
	
		if(data->array[i] == 0){
			cons_time++;
			
		}
		else {
			cons_time = 0;
		}
		if(cons_time == 5){
			data->index_of_last = i;
			break;
			//system("pause");
		}
	}
	}

	

void load_Array(D_ARRAY *data,const char *name){
	FILE *f = fopen(name,"rb");
	if(f == NULL)
		assert(0 && "FAIL !!!");	
	fread(data->array,1,data->_size,f);
	data->index_of_last = data->_size;
	update_Index_Of_Last(data);
	fclose(f);
}

void save_Array(D_ARRAY *data,const char *name){
	
	FILE *f = fopen(name,"wb");
	if(f == NULL)
		assert(0 && "FAIL !!!");
	update_Index_Of_Last(data);
	fwrite(data->array,1,data->index_of_last,f); 
	//data->index_of_last = data->_size;
	
	
}




int main() {
		D_ARRAY compresed, decompresed;
		
		
		init_Array(&decompresed,2000000 * 2);
		init_Array(&compresed,2000000);
		//load_Array(&decompresed,"tsh.txt");
		//print_Array(&decompresed);

	  //int size = fastlz_compress_level(2,decompresed.array,decompresed.index_of_last,compresed.array);
	  //compresed.index_of_last = size;
		//save_Array(&compresed,"comp.txt");
		//print_Array(&decompresed);

		load_Array(&compresed,"comp.txt");
		
	//	printf("\n\nLast %d First %d\n",decompresed._size,compresed._size);
		int size = fastlz_decompress(compresed.array,compresed.index_of_last,decompresed.array,2000000*2);
		printf("\nsize = %d",size);
		//decompresed.index_of_last = size;
		save_Array(&decompresed,"dec.txt");
		
		//save_Array(&compresed,"comp1.txt");
		//print_Array(&compresed);
		//load_Array(&)

	}
/*	FILE *f = fopen("tsh.txt","r");
	D_ARRAY data, data1,data2;
	init_Array(&data,20000);
	//uint
	if(f == NULL)
		return -1;
	while(!feof(f)){
		uint8_t a = fgetc(f);
		insert_Array(&data,a);

	}
	fclose(f);
	init_Array(&data1,data._size);
	init_Array(&data2,data._size);
	data1.index_of_last = data.index_of_last;
	data2.index_of_last = data.index_of_last;
	//print_Array(&data);
	//void *data_uncompres = &data.array;
	fastlz1_compress(data.array,data.index_of_last,data1.array);
	FILE *fc = fopen("compresed.txt","w");
	if(fc == NULL)
		return -1;
	size_t time_reapeat = 0;
	for(size_t i = 0;i < data1.index_of_last;i++){
		if(data1.array[i] == 0){
			time_reapeat++;
		}
		else{
			time_reapeat = 0;
		}
		if(time_reapeat == 20){
			data1.index_of_last = i;
			//printf("Heat");
		}
	}


	fwrite(data1.array,data1.index_of_last,1,fc);
	fclose(fc);


//#pragma omp parallel
	fastlz_decompress(data1.array,data2.index_of_last,data2.array,data2.index_of_last);
	FILE *dec = fopen("decompresed.txt","w");
	if(dec == NULL){
		return -1;
	}
	fwrite(data2.array,data2.index_of_last,1,dec);
	fclose(dec);

	//print_Array(&data2);
	printf("Nesto\n\n");
*/

