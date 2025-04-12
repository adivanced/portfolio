float c_m_min(float* array, uint64_t n){
	float min = array[0];
	for(uint64_t i = 1; i < n; i++){
		if(min > array[i]){
			min = array[i];
		}
	}
	return min;
}

float c_m_max(float* array, uint64_t n){
	float max = array[0];
	for(uint64_t i = 1; i < n; i++){
		if(max < array[i]){
			max = array[i];
		}
	}
	return max;
}

void c_m_scale_min_max(float* array, uint64_t n){
	float min = c_m_min(array, n);
	float max = c_m_max(array, n);

	for(uint64_t i = 0; i < n; i++){
		array[i] = (array[i] - min)/(max-min);
	}
}

void c_m_sigmoid(float* array, uint64_t n){
	for(uint64_t i = 0; i < n; i++){
		array[i] = 1.0f / (1.0f + expf(-1.0f*array[i]));
	}
}

void c_m_sub_arr(float* dst, float* src, uint64_t n){
	for(uint64_t i = 0; i < n; i++){
		dst[i] -= src[i];
	}
}

float c_m_sum(float* array, uint64_t n){
	float sum = 0;
	for(uint64_t i = 0; i < n; i++){
		sum += array[i];
	}
	return sum;
}

void c_m_dot(float* dst, float* matr, float* row, uint64_t n_col, uint64_t n_row){ // УМНОЖЕНИЕ МАТРИЦЫ НА СТОЛБЕЦ!!!!!!!!!!!!!!!!!
	for(uint64_t i = 0; i < n_row; i++){
		float tmp = 0;
		for(uint64_t j = 0; j < n_col; j++){
			tmp += matr[i * n_col + j] * row[j];
		}
		dst[i] = tmp;
	}
}

void c_m_dot_w_add(float* dst, float* matr, float* row, uint64_t n_col, uint64_t n_row, float adder){
	for(uint64_t i = 0; i < n_row; i++){
		float tmp = 0;
		for(uint64_t j = 0; j < n_col; j++){
			tmp += matr[i * n_col + j] * row[j];
		}
		dst[i] = tmp + adder;
	}
}

void c_m_dot_w_mul(float* dst, float* matr, float* row, uint64_t n_col, uint64_t n_row, float multp){
	for(uint64_t i = 0; i < n_row; i++){
		float tmp = 0;
		for(uint64_t j = 0; j < n_col; j++){
			tmp += matr[i * n_col + j] * row[j];
		}
		dst[i] = tmp*multp;
	}
}

void c_m_sub_arr_mul_ss(float* dst, float* src, uint64_t n, float multp){
	for(uint64_t i = 0; i < n; i++){
		dst[i] -= src[i]*multp;
	}
}