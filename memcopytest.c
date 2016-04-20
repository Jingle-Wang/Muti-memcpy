#define _BSD_SOURCE

#include <stdio.h>

#include <string.h>
#include <sys/time.h>

#include <pthread.h>
#include <inttypes.h>
#include <assert.h>

#include <stdlib.h>
#include <time.h>
#define L (1024 * 1024 *1024 * 1  )


inline int min(int a,int b){
	return a<b?a:b;
}

char *s;
char *d;

struct memcpy_param_t{
	char *src;
	char *dst;
	size_t size;
};

typedef struct memcpy_param_t memcpy_param;


void * memcpy_single(void *param){
	memcpy_param *p = (memcpy_param *) param;
	memcpy(p->dst,p->src,p->size);
}

void parallel_memcpy(char *dst,char *src,size_t size){
	assert(dst!=NULL);
	assert(src!=NULL);
	assert(dst!=src);
	
	long long offset=0;
	pthread_t threads[4];
	memcpy_param params[4];
	for(int i=0;i<4;i++){
		offset += (size + 3)/4;
		memcpy_param p;
		p.src = src + offset;
		p.dst = dst + offset;
		p.size = min((size+3)/4,size-offset);
		params[i] = p;
		pthread_create(threads+i,NULL,memcpy_single,(void *)(params+i));
	}
	
	for(int i=0;i<4;i++){
		pthread_join(threads[i],NULL);
	}

}



int main()
{
	//int L = 1024 * 1024 * 1024;
	//char s[L];
	printf("hello world\n");	
	s=(char *)malloc(L);
	d=(char *)malloc(L);
	memset(s, 0xaa, sizeof(char) * L);
//	char d[L];
//	memset(d, '0', L);
	struct timeval start;
	struct timeval end;
	struct timeval res;
	printf("Test Begin\n");	
	gettimeofday(&start, NULL);	
//	parallel_memcpy(s,d,L);
	memcpy(s, d, L);
	gettimeofday(&end,NULL);
	timersub(&end, &start, &res);
	printf("%u.%u s for 1G memcpy\n", res.tv_sec, res.tv_usec);
	return 0;	
	
}
