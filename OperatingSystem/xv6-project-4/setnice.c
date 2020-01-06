#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv){
  int res;
  if(argc < 3){
    printf(2, "usage: setnice [pid] [num]\n");
    exit();
  }
  res = setnice(atoi(argv[1]), atoi(argv[2]));
  if(res == 0){
      printf(1, "set nice to %d\n", atoi(argv[2]));
  }
  else if(res ==-1){
      printf(1, "Fail to set nice!\n");
  }
  
  exit();
}