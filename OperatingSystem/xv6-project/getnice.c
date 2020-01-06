#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv){
  int nice;
  if(argc < 2){
    printf(2, "usage: getnice [pid]\n");
    exit();
  }
  nice = getnice(atoi(argv[1]));
  printf(1, "getnice of %s is %d\n", argv[1], nice);
  exit();
}
