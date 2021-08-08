#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>

// main prog
struct str{
  char* seq;
  int len;
};

void *odd(void* arg){
  const struct str *index = arg;
  int maxAns = 1;
  for(int i = 1; i < index->len; i++){
    int low = i - 1;
    int high = i + 1;
    int currMax = 1;
    while(low >= 0 && high < index->len && index->seq[low] == index->seq[high]){
      low--;
      high++;
      currMax=currMax+2;
    }
    if(currMax > maxAns){
      maxAns = currMax;
    }
  }

  return (void *)(intptr_t)maxAns;
}

void *even(void* arg){
  const struct str *index = arg;
  int maxAns = 0;
  for(int i = 0; i < index->len; i++){
    int low = i;
    int high = i + 1;
    int currMax = 0;
    while(low >= 0 && high < index->len && index->seq[low] == index->seq[high]){
      low--;
      high++;
      currMax=currMax+2;
    }
    if(currMax > maxAns){
      maxAns = currMax;
    }
  }

  return (void *)(intptr_t)maxAns;
}

// timer
long timediff(clock_t t1, clock_t t2) {
    long elapsed;
    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000000;
    return elapsed;
}

int main(void){
  // timer
  clock_t ti1, ti2;
  long elapsed;

  ti1 = clock();

  // main program
  char seq[] = "aaasadsdafasdhjkagsdfjhasjbjflASHBFHJASKBHFKJASBFASSDHJGVAHDGVsakhjfdsakjfuadsfk";
  int len = sizeof(seq)/sizeof(seq[0])-1;

  struct str s = {seq, len};

  pthread_t t0;
  pthread_t t1;
  intptr_t res0;
  intptr_t res1;
  if (pthread_create(&t0, NULL, &odd, &s)!=0){
    return 0;
  }
  if (pthread_create(&t1, NULL, &even, &s)!=0){
    return 00;
  }
  if(pthread_join(t0, (void*)&res0)!=0){
    return 1;
  }
  if(pthread_join(t1, (void*)&res1)!=0){
    return 11;
  }

  printf("%d\n", (int)(res0 > res1 ? res0 : res1));

  // end timer
  ti2 = clock();
  elapsed = timediff(ti1, ti2);
  printf("elapsed: %ld microseconds\n", elapsed);
  return 0;
}
