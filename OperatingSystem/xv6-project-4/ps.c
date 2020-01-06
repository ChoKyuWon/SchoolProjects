#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv){
  //int nice;
  printf(1,"----------ps table----------\n");
  if(argc < 2){
    printf(2, "usage: ps [pid]\n");
    exit();
  }
  ps(atoi(argv[1]));
  //printf(1, "getnice of %s is %d\n", argv[1], nice);
  exit();
}
