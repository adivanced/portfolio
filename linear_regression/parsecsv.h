struct column{
	char* name;
	float* data;
};



char rdbuf[4096];

struct column* get_names(FILE* file, uint64_t* n){
	fgets(rdbuf, 4096, file);
	*n = 1;
	for(char* i = rdbuf; *i != 0; i++){
		if(*i == ','){(*n)++;*i = 0;}
		if(*i == '\n'){*i = 0;}
	}

	char* iter = rdbuf;
	struct column* names = (struct column*)malloc((*n)*sizeof(struct column));

	for(uint64_t i = 0; i < *n; i++){
		uint64_t len = strlen(iter)+1;
		names[i] = (struct column){(char*)malloc(len), 0};
		memcpy(names[i].name, iter, len);
		iter += len;
	}
	return names;
}

void get_rows(FILE* file, struct column* columns, uint64_t n_columns, uint64_t* n_rows){
	*n_rows = 0;
	while(fgets(rdbuf, 4096, file)){
		(*n_rows)++;
		for(char* i = rdbuf; *i != 0; i++){
			if(*i == ','){*i=0;}
			if(*i == '\n'){*i=0;}
		}
		char* iter = rdbuf;
		for(uint64_t i = 0; i < n_columns; i++){
			columns[i].data = realloc(columns[i].data, (*n_rows)*4);
			if((*iter >= '0' && *iter <= '9') || *iter == '-'){
				sscanf(iter, "%f", &(columns[i].data[(*n_rows)-1]));
			}else if(*iter == 'Y'){
				columns[i].data[(*n_rows)-1] = 1.0f;
			}else if(*iter == 'N'){
				columns[i].data[(*n_rows)-1] = 0.0f;
			}else{
				printf("UNPARSIBLE INPUT!!!\n");
				exit(0);
			}
			iter += strlen(iter)+1;
		}
	}
}
