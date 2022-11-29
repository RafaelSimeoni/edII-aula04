#include <stdio.h>
#include <stdlib.h>

typedef struct {
	short int h,m,s;
} t_time;

typedef struct {
	t_time * key;
	char * value;
} t_timetable_item;

typedef struct {
	t_timetable_item * table;
	int size,n;
} t_timetable;

t_time * t_time_init(short int h, short int m, short int s){
	t_time * nt;

	if(h < 0 || h > 23) return(NULL);
	if(m < 0 || m > 59) return(NULL);
	if(s < 0 || s > 59) return(NULL);

	nt = (t_time *) malloc(sizeof(t_time));
	
	nt->h = h;
	nt->m = m;
	nt->s = s;

	return(nt);
}

int t_time_cmp(t_time * ta, t_time * tb){
	int total_sa, total_sb;

	total_sa = ta->h*3600 + ta->m*60 + ta->s;
	total_sb = tb->h*3600 + tb->m*60 + tb->s;

	if(total_sa > total_sb)
		return(1);
	else if(total_sa < total_sb)
		return(-1);
	else
		return(0);
}

void t_time_free(t_time * t){
	free(t);
}

int t_time_get_h(t_time * t){ return t->h;}
int t_time_get_m(t_time * t){ return t->m;}
int t_time_get_s(t_time * t){ return t->s;}

t_timetable * t_timetable_init(int size){
	t_timetable * tt;

	tt = (t_timetable *) malloc(sizeof(t_timetable));
	tt->table = (t_timetable_item*) malloc(sizeof(t_timetable_item)* size); tt->size = size;
	tt->n = 0;

	return(tt);
}

void t_timetable_put(t_timetable * tt,t_time * key, char * value){
	int i,j;

	if(tt->n == tt->size) exit(-1);
	
	i = 0;
  	while(i < tt->n && t_time_cmp(key, tt->table[i].key) > 0){
		i++;
	}

	if(tt->n > i && t_time_cmp(key, tt->table[i].key) != 0){
		for (j = tt->n-1; j >= i; --j) {
			tt->table[j+1].key = tt->table[j].key;		
			tt->table[j+1].value = tt->table[j].value;		
		} 
	}

	tt->table[i].key = key;
	tt->table[i].value = value;
	if(tt->n == i) tt->n++;
}

char * t_timetable_get(t_timetable * tt, t_time * key){
	int lo,hi,m,x;

	lo = 0;
	hi = tt->n-1;

	while(lo <= hi){
		m = (lo + hi)/2;

		x = t_time_cmp(tt->table[m].key, key);
		if(x == 0){
			return(tt->table[m].value);
		}
		else if(x == 1){
			hi = m - 1;
		}
		else{
			lo = m + 1;
		}
	}

	return(NULL);
}

void t_timetable_print(t_timetable * tt){
	int i, h, m, s;
	t_time * t;

	for (i = 0; i < tt->n; ++i) {
		t = tt->table[i].key;
		h = t_time_get_h(t);
		m = t_time_get_m(t);
		s = t_time_get_s(t);

		printf("[%02d] - %02d:%02d:%02d => %s\n",i, h, m, s, tt->table[i].value);
	}
}


void t_timetable_put_seq(t_timetable * tt,t_time * key, char * value){
	if(tt->n == tt->size) exit(-1);
	
	int i;

	for (i = 0; i < tt->n; ++i) {
		if(t_time_cmp(key, tt->table[i].key) == 0)
			break;
	}
	tt->table[i].key = key;
	tt->table[i].value = value;
	
	if(i == tt->n)
		tt->n++;
}

char * t_timetable_get_seq(t_timetable * tt, t_time * key){
	int i;

	for (i = 0; i < tt->n; ++i) {
		if(t_time_cmp(key, tt->table[i].key) == 0)
			return(tt->table[i].value);
	}

	return(NULL);
}

int main(){
	t_time *ta;
	t_timetable * tt;
	char * str;
	size_t len;

	int h,m,s,size;

	printf("Digite o numero de registros da tabela:");
	scanf("%d",&size);
	printf("\n");
	tt = t_timetable_init(size);

	printf("Digite a key no formato (hh:mm:ss). hh com valor negativo continua o programa\n");
	scanf("%d:%d:%d",&h,&m,&s);
	
	while(h >= 0){
		getchar();
		ta = t_time_init(h,m,s);
		printf("Digite o value para esta key\n");
		str = NULL;
		len = getline(&str, &len, stdin);
		str[len-1] = '\0';

		t_timetable_put(tt,ta,str);
	    printf("Digite a key no formato (hh:mm:ss). hh com valor negativo continua o programa\n");
		scanf("%d:%d:%d",&h,&m,&s);
	}
	

	printf("\nImpressão da tabela:\n");
	t_timetable_print(tt);
	
	printf("Digite a key no formato (hh:mm:ss) para ser buscada. hh com valor negativo encerra o programa\n");
	scanf("%d:%d:%d",&h,&m,&s);
	while(h >= 0){
		ta = t_time_init(h,m,s);
		str = t_timetable_get(tt,ta);

		if(str)
			printf("valor encontrado:\n%02d:%02d:%02d => %s\n",h,m,s,str);
		else
			printf("%02d:%02d:%02d => Valor não encontrado\n",h,m,s);
	
	    printf("Digite a key no formato (hh:mm:ss) para ser buscada. hh com valor negativo encerra o programa\n\n");
		scanf("%d:%d:%d",&h,&m,&s);
	}	
}
