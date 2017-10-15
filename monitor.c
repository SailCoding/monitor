#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
  FILE *fp;
  char read_buf[100], cpu_name[20];
  int  lc = 0; //loop counter
  long last_cpu_stat[7], cpu_stat[7], last_cpu_used, cpu_used;
  long last_cpu_total = 0, cpu_total = 0;
  double cpu_usage;
  while(1){
    // cpu monitor
    fp = fopen("/proc/stat","r");
    fgets(read_buf, 100, fp); // read the 1st line
    char *splited_stats = strtok(read_buf," ");
    while(lc < 8){
      last_cpu_stat[lc] = atol(splited_stats);
      splited_stats = strtok(NULL," ");
      lc++;
    }
    fclose(fp);
    lc = 0; // reset counter
    sleep(1);
    fp = fopen("/proc/stat","r");
    fgets(read_buf, 100, fp); // read the 1st line
    splited_stats = strtok(read_buf," ");
    while(lc < 8){
      cpu_stat[lc] = atol(splited_stats);
      splited_stats = strtok(NULL," ");
      lc++;
    }
    fclose(fp);
    for(lc = 0; lc<8; lc++) last_cpu_total += last_cpu_stat[lc];
    for(lc = 0; lc<8; lc++) cpu_total += cpu_stat[lc];
    last_cpu_used = last_cpu_stat[0]+last_cpu_stat[1]+last_cpu_stat[2];
    cpu_used = cpu_stat[0]+cpu_stat[1]+cpu_stat[2];
    cpu_usage = (double)(cpu_used-last_cpu_used)/(cpu_total-last_cpu_total)*100;

    // print stats
    printf("CPU usage: %lf%c\n", cpu_usage, 37);

    lc = 0;
  }
  return 0;
}