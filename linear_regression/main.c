#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
//#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include "parsecsv.h"

extern float m_min(float*, uint64_t);
extern float m_max(float*, uint64_t);
extern long double m_mean(float*, uint64_t);
extern long double m_var(float*, uint64_t, long double*, float*);
extern long double m_std(long double*);
extern void m_scale_min_max(float*, uint64_t);
extern void m_col_bckw_copy(struct column*, struct column*, uint64_t, uint64_t);
extern void m_fill_dword(void*, uint32_t, uint64_t, uint64_t);
extern float m_mul_sum(float*, float*, uint64_t);
extern float m_rss(float*, float*, uint64_t);
extern float m_sst(float*, uint64_t, float);


extern uint64_t gauss(float*, float*, uint64_t, uint64_t);
extern uint64_t one_diagonal(uint64_t, float*, uint64_t);
extern void extransw(float*, float*, uint64_t, uint64_t);
extern uint64_t add_all(uint64_t, float*, uint64_t);

void min_max_scaler(struct column* col, uint64_t n_columns, uint64_t n_rows){
	for(uint64_t i = 0; i < n_columns; i++){
		m_scale_min_max(col[i].data, n_rows);
	}
}

long long int current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    //long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return 1000000 * te.tv_sec + te.tv_usec;
}

float* linear_regression_scalar_coeff(struct column** X, struct column** Y, uint64_t* n_columns, uint64_t n_rows){ 
	(*n_columns)++;
	(*X) = realloc((*X), (*n_columns)*sizeof(struct column));
	m_col_bckw_copy(((void*)((*X)+(*n_columns)-1))+8, ((void*)((*X)+(*n_columns)-2))+8, 0, (*n_columns)-1);

	(*X)[0].name = malloc(5);
	(*X)[0].data = malloc(4*n_rows);
	strcpy((*X)[0].name, "Ones");
	(*X)[0].data = malloc(n_rows*4);
	m_fill_dword((*X)[0].data, 0x3f800000, 0, n_rows);

	// for(uint64_t i = 0; i < (*n_columns); i++){
	// 	printf("--------%s--------\n", (*X)[i].name);
	// 	for(uint64_t j = 0; j < 10; j++){
	// 		printf("%f\n", (*X)[i].data[j]);
	// 	}
	// 	printf("--------------------------\n");
	// }
	//uint64_t matr_sz = (*n_columns)*((*n_columns)+1);
	//matr_sz += 8-(matr_sz%8);
	//printf("%ld\n", matr_sz);

	//float* matrix = malloc(4*(*n_columns)*(*n_columns));

	float* matrix = calloc((((*n_columns)+1) + 8-((*n_columns)+1)%8)*(*n_columns), 4);
	float* iter = matrix;
	for(uint64_t i = 0; i < (*n_columns); i++){
		for(uint64_t j = 0; j < (*n_columns); j++){
			//printf("%f\n", m_mul_sum((*X)[i].data, (*X)[j].data, n_rows));
			*iter = m_mul_sum((*X)[i].data, (*X)[j].data, n_rows);
			//printf("%f ", *iter);
			iter++;
		}
		*iter = m_mul_sum((*X)[i].data, (*Y)->data, n_rows);
		//printf("%f ", *iter);
		iter++;
		while(((iter-matrix)%8)){
			//printf("%f ", 0.0f);
			iter++;
		}
		//printf("\n");
	}

	float* answ = malloc(4*(*n_columns));
	if(add_all((*n_columns), matrix, ((*n_columns)+1) + 8-((*n_columns)+1)%8)){
		printf("MATRIX INCONSISTENT!\n");
		exit(0);
	}
	gauss(0, matrix, ((*n_columns)+1) + 8-((*n_columns)+1)%8, (*n_columns));
	one_diagonal((*n_columns), matrix, ((*n_columns)+1) + 8-((*n_columns)+1)%8);
	extransw(answ, matrix, ((*n_columns)+1) + 8-((*n_columns)+1)%8, (*n_columns));
	free(matrix);
	return answ;
}

// 
// 
// 
// 0x44567e1c, 0x0, 0x0, 0x0, 0x445b64ba, 0x0, 0x0, 0x0


float* linear_regression_scalar(struct column** train_X, struct column** train_Y, struct column** test_X, uint64_t* n_columns, uint64_t n_rows_train, uint64_t n_rows_test){
	min_max_scaler(*train_X, *n_columns, n_rows_train);
	//min_max_scaler(*train_Y, 1, n_rows_train);
	min_max_scaler(*test_X, *n_columns, n_rows_test);
	// printf("---------------------\n");
	// for(uint64_t i = 0; i < *n_columns; i++){
	// 	printf("%s\n", (*train_X)[i].name);
	// }
	// printf("---------------------\n");
	float* coef = linear_regression_scalar_coeff(train_X, train_Y, n_columns, n_rows_train);
	float* answ	= malloc(4*n_rows_test);
	float* answiter = answ;

	for(uint64_t i = 0; i < n_rows_test; i++){
		float tmp = coef[0];
		for(uint64_t j = 0; j < (*n_columns)-1; j++){
			tmp += coef[j+1] * (*test_X)[j].data[i];
		}
		*answiter = tmp;
		answiter++;
	}
	free(coef);
	return answ;
}


float m_moda(float* arr, uint64_t n){
	float max = 0.0f;
	uint64_t maxcount = 0;

	for(float* i = arr; i < arr+n; i++){
		uint64_t count = 1;
		for(float* j = i+1; j < arr+n; j++){
			if(*j == *i){
				count++;
			}else{
				//printf("%f %ld\n", *i, count);
				if(maxcount < count){
					max = *i;
					maxcount = count;
				}
				i = j;
				break;
			}
		}
	}
	return max;
}

float m_r2_score(float* test, float* predict, uint64_t n){
	float rss = m_rss(test, predict, n);
	float mean = m_mean(test, n);
	float sst = m_sst(test, n, mean);
	return 1 - (rss/sst);

}

uint64_t true_rand(uint64_t start, uint64_t end){
	register uint64_t r;
	asm("rdrand %0"
		:"=r"(r)
	);
	return start + r%(end-start+1);
}

int comp(const void* a, const void* b){
	return (int)((*((float*)a) - *((float*)b))*10000.0f);
}

int rand_comp(const void* a, const void* b){
	asm("rdrand %eax"
	);
}


void train_test_split(struct column** X, struct column** Y, float coeff, struct column** train_X, struct column** train_Y, struct column** test_X, struct column** test_Y, uint64_t n_columns, uint64_t n_rows){
	for(uint64_t i = 0; i < n_rows; i++){
		uint64_t index1 = true_rand(0, n_rows-1);
		uint64_t index2 = true_rand(0, n_rows-1);
		//printf("%ld %ld\n", index1, index2);
		float tmp = (*Y)->data[index1];
		(*Y)->data[index1] = (*Y)->data[index2];
		(*Y)->data[index2] = tmp;
		for(uint64_t j = 0; j < n_columns; j++){
			tmp = (*X)[j].data[index1];
			(*X)[j].data[index1] = (*X)[j].data[index2];
			(*X)[j].data[index2] = tmp;
		}
	}

	// for(uint64_t i = 0; i < n_columns; i++){
	// 	qsort((*X)[i].data, n_rows, 4, rand_comp);
	// }
	uint64_t n_rows_train = (uint64_t)(n_rows*coeff);
	uint64_t n_rows_test = n_rows - n_rows_train;

	(*train_X) = (struct column*)malloc(sizeof(struct column)*n_columns);
	(*test_X) = (struct column*)malloc(sizeof(struct column)*n_columns);

	(*train_Y) = (struct column*)malloc(sizeof(struct column));
	(*test_Y) = (struct column*)malloc(sizeof(struct column));

	for(uint64_t i = 0; i < n_columns; i++){
		(*train_X)[i].data = (float*)malloc(n_rows_train*4);
		(*test_X)[i].data = (float*)malloc(n_rows_test*4);
		(*train_X)[i].name = (*X)[i].name;
		memcpy((*train_X)[i].data, (*X)[i].data, n_rows_train*4);
		(*test_X)[i].name = (*X)[i].name;
		memcpy((*test_X)[i].data, (*X)[i].data+n_rows_train, n_rows_test*4);
		free((*X)[i].data);
	}
	(*train_Y)->data = (float*)malloc(n_rows_train*4);
	(*test_Y)->data = (float*)malloc(n_rows_test*4);
	(*train_Y)->name = (*Y)->name;
	(*test_Y)->name = (*Y)->name;
	memcpy((*train_Y)->data, (*Y)->data, n_rows_train*4);
	memcpy((*test_Y)->data, (*Y)->data+n_rows_train, n_rows_test*4);
	free((*Y)->data);
}





void main(int argc, char** argv){	

	FILE* file = fopen("Student_Performance.csv", "r+");

	uint64_t n_columns;

	struct column* columns = get_names(file, &n_columns);

	printf("%ld\n", n_columns);
	// for(uint64_t i = 0; i < n_columns; i++){
	// 	printf("%s\n", columns[i].name);
	// }	

	uint64_t n_rows;
	get_rows(file, columns, n_columns, &n_rows);
	


	printf("%ld\n", n_rows);	

	// for(uint64_t i = 0; i < n_rows; i++){
	// 	for(uint64_t j = 0; j < n_columns; j++){
	// 		printf("%f ", columns[j].data[i]);
	// 	}
	// 	printf("\n");
	// }


	struct column* response = (struct column*)malloc(sizeof(struct column)); 
	memcpy(response, &(columns[n_columns-1]), sizeof(struct column));
	columns = (struct column*)realloc(columns, (n_columns-1)*sizeof(struct column));
	n_columns--;

	struct column* copy_columns = malloc(sizeof(struct column)*n_columns);
	memcpy(copy_columns, columns, n_columns*sizeof(struct column));
	for(uint64_t i = 0; i < n_columns; i++){
		copy_columns[i].data = malloc(n_rows*4);
		memcpy(copy_columns[i].data, columns[i].data, n_rows*4);
	}



	for(uint64_t i = 0; i < n_columns; i++){
		printf("========================================\n");
		printf("Column №%ld, name: %s\n", i, columns[i].name);
		printf("Amount of elements: %ld\n", n_rows);
		printf("MIN value: %f\n", m_min(columns[i].data, n_rows));
	 	printf("MAX value: %f\n", m_max(columns[i].data, n_rows));
	 	long double mean_val = m_mean(columns[i].data, n_rows);
	 	printf("Mean value: %Lf\n", mean_val);
	 	qsort(columns[i].data, n_rows, 4, comp);
	 	printf("Mode value: %f\n", m_moda(columns[i].data, n_rows));
	 	printf("Quantiles:\n");
	 	printf("%f: %f\n", 0.25f, columns[i].data[(uint64_t)(n_rows*0.25f)]);
	 	printf("%f: %f\n", 0.5f, columns[i].data[(uint64_t)(n_rows*0.5f)]);
	 	printf("%f: %f\n", 0.75f, columns[i].data[(uint64_t)(n_rows*0.75f)]);
	 	float* tmparr = (float*)malloc(n_rows*4);
	 	long double variance = m_var(columns[i].data, n_rows, &mean_val, tmparr);
	 	free(tmparr);
	 	printf("Unbiased variance value: %Lf\n", variance);
	 	long double std_dev = m_std(&variance);
	 	printf("Standart deviation: %Lf\n", std_dev);
	 	printf("Coefficient of variation: %Lf\n", (std_dev/mean_val) * 100);
		printf("========================================\n");
	}

	for(uint64_t i = 0; i < n_columns; i++){
		free(columns[i].data);
	}
	free(columns);


	struct column* train_X;
	struct column* train_Y;

	struct column* test_X;
	struct column* test_Y;

	train_test_split(&copy_columns, &response, 0.7f, &train_X, &train_Y, &test_X, &test_Y, n_columns, n_rows);

	uint64_t n_rows_train = (uint64_t)(n_rows * 0.7f);
	uint64_t n_rows_test = n_rows - n_rows_train;

	// printf("---------------------\n");
	// for(uint64_t i = 0; i < n_columns; i++){
	// 	printf("%s\n", train_X[i].name);
	// }
	// printf("---------------------\n");

	long long int  time = current_timestamp();
	//usleep(800);
	float* answ = linear_regression_scalar(&train_X, &train_Y, &test_X, &n_columns, n_rows_train, n_rows_test);
	printf("TIME: %lld\n", current_timestamp()-time);

	printf("r2 score: %f\n", m_r2_score(test_Y->data, answ, n_rows_test));


	/*for(uint64_t i = 0; i < n_rows_test; i++){
		printf("%f ", answ[i]);
	}
	printf("\n");*/




}

