#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "parsecsv.h"

#define USE_ASM 1

struct log_reg{
	uint64_t n_iter;
	float* weights;
	float l_rate;
	float bias;
};

extern void m_transp_matr_from_col(float*, struct column*, uint64_t, uint64_t);

extern void m_scale_min_max(float*, uint64_t);
extern void m_dot_w_add(float*, float*, float*, uint64_t, uint64_t, float);
extern void m_dot_w_mul(float*, float*, float*, uint64_t, uint64_t, float);
extern void m_sigmoid(float*, uint64_t);
extern void m_sub_arr(float*, float*, uint64_t);
extern float m_sum(float*, uint64_t);
extern void m_dot(float*, float*, float*, uint64_t, uint64_t);
extern void m_mul_arr_ss(float*, uint64_t, float);
extern void m_sub_arr_mul_ss(float*, float*, uint64_t, float);
extern float m_min(float*, uint64_t);
extern float m_max(float*, uint64_t);


#include "matlib_comp.h"

#if USE_ASM == 1
	#define scale_min_max m_scale_min_max
	#define dot_w_add m_dot_w_add
	#define dot_w_mul m_dot_w_mul
	#define sigmoid m_sigmoid
	#define sub_arr m_sub_arr
	#define sum m_sum
	#define dot m_dot
	#define mul_arr_ss m_mul_arr_ss
	#define sub_arr_mul_ss m_sub_arr_mul_ss
#else
	#define scale_min_max c_m_scale_min_max
	#define dot_w_add c_m_dot_w_add
	#define dot_w_mul c_m_dot_w_mul
	#define sigmoid c_m_sigmoid
	#define sub_arr c_m_sub_arr
	#define sum c_m_sum
	#define dot c_m_dot
	#define mul_arr_ss c_m_mul_arr_ss
	#define sub_arr_mul_ss c_m_sub_arr_mul_ss
#endif


long long int current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    //long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return 1000000 * te.tv_sec + te.tv_usec;
}



uint64_t true_rand(uint64_t start, uint64_t end){
	register uint64_t r;
	asm("rdrand %0"
		:"=r"(r)
	);
	return start + r%(end-start+1);
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

void logistic_regression_fit(float* X, float* XT, float* Y, struct log_reg* reg, uint64_t n_rows, uint64_t n_columns){
	float* pred = (float*)malloc(n_rows*4);
	float* dw = (float*)malloc(n_columns*4);

	// float* tmp = (float*)malloc(n_rows*4);
	//float* tmp1 = (float*)malloc(n_columns*4);
	//uint64_t tmp_flg = 0;

	for(uint64_t i = 0; i < reg->n_iter; i++){
		dot_w_add(pred, X, reg->weights, n_columns, n_rows, reg->bias);


		// m_dot_w_add(tmp, X, reg->weights, n_columns, n_rows, reg->bias);

		// for(uint64_t j = 0; j < n_rows; j++){
		// 	if(tmp[j] - pred[j] >= 0.00001f){
		// 		printf("Dot with add: I: %ld ASM: %f(%#010x) C: %f(%#010x)\n", j, tmp[j], *((uint32_t*)(&(tmp[j]))), pred[j], *((uint32_t*)(&(pred[j]))));
		// 	}
		// }

		sigmoid(pred, n_rows);		

		// m_sigmoid(tmp, n_rows);

		// for(uint64_t j = 0; j < n_rows; j++){
		// 	if(tmp[j] - pred[j] >= 0.001f){
		// 		printf("Sigmoid: I: %ld ASM: %f(%#010x) C: %f(%#010x)\n", j, tmp[j], *((uint32_t*)(&(tmp[j]))), pred[j], *((uint32_t*)(&(pred[j]))));	
		// 	}
		// }


		sub_arr(pred, Y, n_rows);

		//m_sub_arr(tmp, Y, n_rows);

		// for(uint64_t j = 0; j < n_rows; j++){
		// 	if(tmp[j] - pred[j] >= 0.001f){
		// 		printf("Sub array: I: %ld ASM: %f(%#010x) C: %f(%#010x)\n", j, tmp[j], *((uint32_t*)(&(tmp[j]))), pred[j], *((uint32_t*)(&(pred[j]))));	
		// 	}
		// }


		dot_w_mul(dw, XT, pred, n_rows, n_columns, (1.0f/n_columns) * 2.0f); 

		// m_dot_w_mul(tmp1, XT, pred, n_rows, n_columns, (1.0f/n_columns)*2.0f);

		// for(uint64_t j = 0; j < n_columns; j++){
		// 	if(tmp1[j] - dw[j] >= 0.000001f){
		// 		printf("Dot w mul: I: %ld ASM: %f(%#010x) C: %f(%#010x)\n", j, tmp1[j], *((uint32_t*)(&(tmp1[j]))), dw[j], *((uint32_t*)(&(dw[j]))));	
		// 	}
		// }

		sub_arr_mul_ss(reg->weights, dw, n_columns, reg->l_rate);

		reg->bias -= reg->l_rate * (1.0f/n_columns) * 2.0f * sum(pred, n_rows);

		//m_mul_arr_ss(dw, n_rows, reg->l_rate);
		//m_sub_arr(reg->weights, dw, n_rows);
		//break;
	}
	// printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	// for(uint64_t i = 0; i < n_rows; i++){
	// 	printf("%f\n", pred[i]);
	// }
	// printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

	free(pred);
	free(dw);
}

float* logistic_regression_predict(struct log_reg* reg, float* X, uint64_t n_rows, uint64_t n_columns, float percentile){
	float* pred = (float*)malloc(n_rows*4);

	dot_w_add(pred, X, reg->weights, n_columns, n_rows, reg->bias);
	sigmoid(pred, n_rows);
	//m_percentile(pred, n_rows, percentile);

	for(uint64_t i = 0; i < n_rows; i++){
		if(pred[i] > percentile){
			pred[i] = 1.0f;
		}else{
			pred[i] = 0.0f;
		}
	}

	return pred;
}



void col_to_matr_view(float* matr, struct column* col, uint64_t n_columns, uint64_t n_rows){
	for(uint64_t i = 0; i < n_rows; i++){
		for(uint64_t j = 0; j < n_columns; j++){
			matr[i*n_columns+j] = col[j].data[i];
		}
	}
}

float* delete_not_unique(float* arr, uint64_t n, uint64_t* n_u){
	float* unique = (float*)malloc(4);
	for(uint64_t i = 0; i < n; i++){
		for(uint64_t j = 0; j < *n_u; j++){
			if(unique[j] == arr[i]){
				goto e_iter;
			}
		}
		(*n_u)++;
		unique = (float*)realloc(unique, (*n_u)*4);
		unique[(*n_u)-1] = arr[i];
		e_iter:
		if(!(*n_u)){
			unique[0] = arr[i];
			*n_u = 1;
		}	
	}
	return unique;
}

float m_f1_score(float* test, float* pred, uint64_t n){

	uint32_t TP = 0;
	uint32_t FP = 0;
	uint32_t TN = 0;
	uint32_t FN = 0;

	uint64_t n_classes = 0;
	float* classes = delete_not_unique(test, n, &n_classes);

	uint32_t* class_stats = (uint32_t*)calloc(n_classes*4, 4);

	for(uint64_t i = 0; i < n_classes; i++){
		for(uint64_t j = 0; j < n; j++){
			if(test[j] == pred[j] && test[j] == classes[i]){
				TP++;
				class_stats[i*4]++;
			}else if(test[j] != pred[j] && pred[j] == classes[i]){
				FP++;
				class_stats[i*4 + 1]++;
			}else if(test[j] == pred[j] && test[j] != classes[i]){
				TN++;
				class_stats[i*4 + 2]++;
			}else if(test[j] != pred[j] && test[j] != classes[i]){
				FN++;
				class_stats[i*4 + 3]++;
			}
		}
	}
	return ((float)TP) / (((float)TP) + (1/((float)n_classes)) * (((float)FP) + ((float)FN)) );

}


int main(){
	FILE* file = fopen("diabetes.csv", "r+");

	uint64_t n_columns;

	struct column* columns = get_names(file, &n_columns);

	uint64_t n_rows;
	get_rows(file, columns, n_columns, &n_rows);

	// for(uint64_t i = 0; i < n_columns; i++){
	//  	printf("%s\n", columns[i].name);
	//  	for(uint64_t j = 0; j < n_rows; j++){
	//  		printf("%f\n", columns[i].data[j]);
	//  	}
	// }	

	printf("%ld %ld\n", n_columns, n_rows);
	for(uint64_t i = 0; i < n_columns; i++){
	 	printf("%s\n", columns[i].name);
	}	

	n_columns--;
	struct column* response = (struct column*)malloc(sizeof(struct column));
	response->name = columns[n_columns].name;
	response->data = columns[n_columns].data;
	columns = (struct column*)realloc(columns, n_columns*sizeof(struct column));


	struct column* train_X;
	struct column* train_Y;


	struct column* test_X;
	struct column* test_Y;

	train_test_split(&columns, &response, 0.7f, &train_X, &train_Y, &test_X, &test_Y, n_columns, n_rows);

	uint64_t n_rows_train = (uint64_t)(n_rows * 0.7f);
	uint64_t n_rows_test = n_rows - n_rows_train;

	for(uint64_t i = 0; i < n_columns; i++){
		// float* testbuf = (float*)malloc(n_rows_train*4);
		// memcpy(testbuf, train_X[i].data, n_rows_train*4);

		//m_scale_min_max(testbuf, n_rows_train);
		scale_min_max(train_X[i].data, n_rows_train);
		// for(uint64_t j = 0; j < n_rows_train; j++){
		// 	if(testbuf[j] != train_X[i].data[j]){
		// 		printf("I: %ld asm: %f C: %f\n", j, testbuf[j], train_X[i].data[j]);
		// 	}
		// }


		// if(m_min(train_X[i].data, n_rows_train) != c_m_min(train_X[i].data, n_rows_train)){
		// 	printf("!!!!!!!minasm %f\n", m_min(train_X[i].data, n_rows_train));
		// 	printf("!!!!!!!minC   %f\n", c_m_min(train_X[i].data, n_rows_train));
		// }
		// if(m_max(train_X[i].data, n_rows_train) != c_m_max(train_X[i].data, n_rows_train)){
		// 	printf("!!!!!!!maxasm %f\n", m_max(train_X[i].data, n_rows_train));
		// 	printf("!!!!!!!maxC   %f\n", c_m_max(train_X[i].data, n_rows_train));		
		// }


		// float* testbuf = (float*)malloc(n_rows_test*4);
		// memcpy(testbuf, test_X[i].data, n_rows_test*4);

		// m_scale_min_max(testbuf, n_rows_test);
		scale_min_max(test_X[i].data, n_rows_test);
		// for(uint64_t j = 0; j < n_rows_test; j++){
		// 	if(testbuf[j] != test_X[i].data[j]){
		// 		printf("I: %ld asm: %f C: %f\n", j, testbuf[j], train_X[i].data[j]);
		// 	}
		// }
		// return 0;

		
		// if(m_min(test_X[i].data, n_rows_test) != c_m_min(test_X[i].data, n_rows_test)){
		// 	printf("!!!!!!!minasm %f\n", m_min(test_X[i].data, n_rows_test));
		// 	printf("!!!!!!!minC   %f\n", c_m_min(test_X[i].data, n_rows_test));
		// }
		// if(m_max(test_X[i].data, n_rows_test) != c_m_max(test_X[i].data, n_rows_test)){
		// 	printf("!!!!!!!maxasm %f\n", m_max(test_X[i].data, n_rows_test));
		// 	printf("!!!!!!!maxC   %f\n", c_m_max(test_X[i].data, n_rows_test));		
		// }
		// printf("=========================================\n");
	}

	float* train_X_matr = (float*)malloc(4*n_rows_train*n_columns);
	float* test_X_matr = (float*)malloc(4*n_rows_test*n_columns);
	float* train_X_matr_T = (float*)malloc(4*n_rows_train*n_columns);
	float* test_X_matr_T = (float*)malloc(4*n_rows_test*n_columns);	

	col_to_matr_view(train_X_matr, train_X, n_columns, n_rows_train);
	col_to_matr_view(test_X_matr, test_X, n_columns, n_rows_test);

	m_transp_matr_from_col(train_X_matr_T, train_X, n_columns, n_rows_train);
	m_transp_matr_from_col(test_X_matr_T, test_X, n_columns, n_rows_test);

	struct log_reg reg = (struct log_reg){n_iter:5000, l_rate:0.001f, weights:(float*)calloc(n_columns, 4), bias:0.0f};

	uint64_t time = current_timestamp();

	logistic_regression_fit(train_X_matr, train_X_matr_T, train_Y->data, &reg, n_rows_train, n_columns);

	float* pred = logistic_regression_predict(&reg, test_X_matr, n_rows_test, n_columns, 0.5f);
//188315
//9778
	time = current_timestamp() - time;
	printf("Work time: %ld\n", time);

	uint64_t good = 0;
	uint64_t bad = 0;

	for(uint64_t i = 0; i < n_rows_test; i++){
		printf("%f %f", pred[i], test_Y->data[i]);
		if(pred[i] == test_Y->data[i]){
			printf(" +\n");
			good++;
		}else{
			printf(" -\n");
			bad++;
		}
	}

	float f1_score = m_f1_score(test_Y->data, pred, n_rows_test);

	printf("f1_score: %f\n", f1_score);
	printf("GOOD: %ld\n", good);
	printf("BAD:  %ld\n", bad);


	// float arr[20] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

	// for(uint64_t i = 0; i < 20; i++){
	// 	printf("%f ", arr[i]);
	// }
	// printf("\n");
	// m_sigmoid(arr, 20);
	// for(uint64_t i = 0; i < 20; i++){
	// 	printf("%f ", arr[i]);
	// }
	// printf("\n");

	// for(uint64_t i = 0; i < n_rows_test; i++){
	// 	printf("%f %f\n", pred[i], test_Y->data[i]);
	// }

	// float matr[40] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f,
	// 				  11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f,
	// 				  1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f,
	// 				  11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f};



	// float res[10];
	// float col[10] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
	// m_dot_w_mul(res, matr, col, 10, 4, 0.5f);

	// printf("[");
	// for(uint64_t i = 0; i < 10; i++){
	// 	printf("%f ", res[i]);
	// }
	// printf("]\n");

	// printf("%f %f %f %f %f\n", train_X_matr[0], train_X_matr[1], train_X_matr[2], train_X_matr[3], train_X_matr[4]);
	// printf("%f\n%f\n%f\n%f\n", train_X_matr[n_columns*1], train_X_matr[n_columns*2], train_X_matr[n_columns*3], train_X_matr[n_columns*4]);
	// printf("%f %f %f %f %f\n", train_X_matr_T[0], train_X_matr_T[1], train_X_matr_T[2], train_X_matr_T[3], train_X_matr_T[4]);
	// printf("%f\n%f\n%f\n%f\n", train_X_matr_T[n_rows_train*1], train_X_matr_T[n_rows_train*2], train_X_matr_T[n_rows_train*3], train_X_matr_T[n_rows_train*4]);

	
	for(uint64_t i = 0; i < n_columns; i++){
		printf("%f ", reg.weights[i]);
	}
	printf("\n%f\n", reg.bias);

	// #if USE_ASM == 1
	// FILE* result = fopen("csvs/asm.csv", "a");
	// #else 
	// FILE* result = fopen("csvs/C.csv", "a");
	// #endif

	// fprintf(result, "%ld,%ld\n", time, reg.n_iter);

	return 0;
}


