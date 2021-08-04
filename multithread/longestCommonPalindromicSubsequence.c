#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct sequenceInfo{
  char* sequence;
  int len;
};

void *odd(struct sequenceInfo* seqInfo){
  char* seq = seqInfo->sequence;
  int len = seqInfo->len;
  int currMaxOdd = 0;
  for(int i = 1; i < len - 1; i++){
    int low = i - 1;
    int high = i + 1;
    int tempMaxOdd = 1;
    while(low >= 0 && high < len && seq[low] == seq[high]){
      low--;
      high++;
      tempMaxOdd = tempMaxOdd + 2;
    }
    if(tempMaxOdd > currMaxOdd){
      currMaxOdd = tempMaxOdd;
    }
  }
  int* res = malloc(sizeof(int));
  *res = currMaxOdd;
  printf("%d\n", currMaxOdd);
  return (void*)&res;
}

// time complexity: O(n^2)
// space complexity: O(1)
void *even(struct sequenceInfo* seqInfo){
  char* seq = seqInfo->sequence;
  int len = seqInfo->len;
  int currMaxEven = 0;
  for(int i = 0; i < len - 1; i++){
    int low = i;
    int high = i + 1;
    int tempMaxEven = 0;
    while(low >= 0 && high < len && seq[low] == seq[high]){
      low--;
      high++;
      tempMaxEven = tempMaxEven + 2;
    }
    if(tempMaxEven > currMaxEven){
      currMaxEven = tempMaxEven;
    }
  }
  int* res = malloc(sizeof(int));
  *res = currMaxEven;
  printf("%d\n", currMaxEven);
  return (void*)&res;
}

// execute both "odd" and "even" in parallel on spereate threads, then compare their results
// total time complexity: O(n^2)
int main(void){
  char seq0[] = "asdhjbajshsj";
  char seq1[] = "asdhjbajshhsjsf";
  int len0 = sizeof(seq0)/sizeof(seq0[0]) - 1;
  int len1 = sizeof(seq1)/sizeof(seq1[0]) - 1;
  struct sequenceInfo str0 = {(char*)seq0, len0};
  struct sequenceInfo str1 = {(char*)seq1, len1};
  struct sequenceInfo* s0 = &str0;
  struct sequenceInfo* s1 = &str1;

  odd(s0);
  even(s1);
  return 0;
}
