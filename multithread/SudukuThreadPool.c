#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#define threadNum 4

#define squareUnit 2
#define square 4
int** input;

int isValid = 1;

//
// threadpool for col calculations
//
typedef struct Task {
  int** input;
  int type;
  int index;
  int subX;
  int subY;
} Task;

Task taskQueue[3*square];
int taskCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void checkTask(Task* task){
  int** input = task -> input;
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  int type = task -> type;
  if(type == 0){
    int index = task -> index;
    for(int i = 0; i < square; i++){
      if(input[i][index] == -1){
        continue;
      }
      else{
        available[input[i][index]]++;
        if(available[input[i][index]] > 1){
          isValid = 0;
          printf("col index: %d fails\n", index);
          return;
        }
      }
    }
    printf("col index: %d passes\n", index);
  }
  if(type == 1){
    int index = task -> index;
    for(int i = 0; i < square; i++){
      if(input[index][i] == -1){
        continue;
      }
      else{
        available[input[index][i]]++;
        if(available[input[index][i]] > 1){
          isValid = 0;
          printf("row index: %d fails\n", index);
          return;
        }
      }
    }
    printf("row index: %d passes\n", index);
  }
  if(type == 2){
    int subX = task -> subX;
    int subY = task -> subY;
    for(int i = subX; i < subX + squareUnit; i++){
      for(int j = subY; j < subY + squareUnit; j++){
        if(input[i][j] == -1){
          continue;
        }
        else{
          available[input[i][j]]++;
          if(available[input[i][j]] > 1){
            isValid = 0;
            printf("square: %d %d fails\n", subX, subY);
            return;
          }
        }
      }
    }
    printf("square: %d %d passes\n", subX, subY);
  }
  return;
}

void* startThread(void* args){
  while(1){
    Task task;
    pthread_mutex_lock(&mutexQueue);
    task = taskQueue[0];
    int i;
    for(i = 0; i < taskCount; i++){
      taskQueue[i] = taskQueue[i + 1];
    }
    taskCount--;
    pthread_mutex_unlock(&mutexQueue);
    checkTask(&task);
    if(taskCount == 0){
      break;
    }
  }
}

void submitTask(Task task){
  pthread_mutex_lock(&mutexQueue);
  taskQueue[taskCount] = task;
  taskCount++;
  pthread_mutex_unlock(&mutexQueue);
  pthread_cond_signal(&condQueue);
}

int main(void){

  //
  // input example
  //
  int arrayIn[][square] = {
      {1,-1,-1,2},
      {2,-1,1,-1},
      {0,3,-1,-1},
      {1,-1,-1,0}
    };

  input = malloc(square * sizeof(int*));
  for(int i = 0; i < square; i++){
    input[i] = malloc(square * sizeof(int));
    for(int j = 0; j < square; j++){
      input[i][j] = arrayIn[i][j];
    }
  }

  //
  // execute threadpool
  //
  pthread_t th_col[threadNum];
  pthread_mutex_init(&mutexQueue, NULL);
  pthread_cond_init(&condQueue, NULL);
  for(int i = 0; i < threadNum; i++){
    if(pthread_create(&th_col[i], NULL, &startThread, NULL) != 0){
      printf("failed to create thread\n");
    }
  }

  for(int i = 0; i < square; i++){
    Task t = {
      .input = input,
      .type = 0,
      .index = i,
      .subX = -1,
      .subY = -1
    };
    submitTask(t);
  }

  for(int i = 0; i < square; i++){
    Task t = {
      .input = input,
      .type = 1,
      .index = i,
      .subX = -1,
      .subY = -1
    };
    submitTask(t);
  }

  int subX = 0;
  int subY = 0;
  for(int i = 0; i < squareUnit; i++){
    subY = 0;
    for(int j = 0; j < squareUnit; j++){
      Task t = {
        .input = input,
        .type = 2,
        .index = -1,
        .subX = subX,
        .subY = subY
      };
      submitTask(t);
      subY = subY + squareUnit;
    }
    subX = subX + squareUnit;
  }

  for(int i = 0; i < threadNum; i++){
    if(pthread_join(th_col[i], NULL) != 0){
      printf("failed to join thread\n");
    }
  }
  pthread_mutex_destroy(&mutexQueue);
  pthread_cond_destroy(&condQueue);

  //
  // output result
  //
  switch(isValid){
    case 0:
      printf("does not pass\n");
      break;
    case 1:
      printf("passes\n");
      break;
  }

  free(input);
  return 1;
}
