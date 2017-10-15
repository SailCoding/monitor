#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
  FILE *fp;
  char read_buf[100], cpu_name[20], mem_unit[5], device_name[20];
  int  lc = 0; //loop counter
  int major_device_num, minor_device_num;
  long last_cpu_stat[7], cpu_stat[7], last_cpu_used, cpu_used;
  long mem_total, mem_free, mem_ava, mem_buf, mem_cache;
  long last_cpu_total = 0, cpu_total = 0;
  unsigned long completed_read_num,  merged_read_num,  sector_read_num,  read_time,
                completed_write_num, merged_write_num, sector_write_num, write_time,
                io_num, io_time, weighted_io_time; //unsigned according to official doc
  unsigned long last_sector_read, last_sector_write, sector_read, sector_write;
  int millisecond_to_second = 100, bytes_to_megabytes = 2048;
  double cpu_usage, mem_usage, read_spd, write_spd;
  while(1){
    // cpu monitor 1
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

    // IO monitor 1
    fp = fopen("/proc/diskstats","r");
    while(fscanf(fp,"%u %u %s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
      &major_device_num, &minor_device_num, device_name,
      &completed_read_num,  &merged_read_num, &sector_read_num,  &read_time,
      &completed_write_num, &merged_write_num,&sector_write_num, &write_time,
      &io_num, &io_time, &weighted_io_time))
    {
      // stop while loop once sd* device appears
      if(strncmp(device_name,"sd", 2) == 0){

        last_sector_read = sector_read_num;
        last_sector_write= sector_write_num;
        break;
      }
    }
    fclose(fp);

    sleep(1);

    // cpu monitor 2
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

    // IO monitor 2
    fp = fopen("/proc/diskstats","r");
    while(fscanf(fp,"%u %u %s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
      &major_device_num, &minor_device_num, device_name,
      &completed_read_num,  &merged_read_num, &sector_read_num,  &read_time,
      &completed_write_num, &merged_write_num,&sector_write_num, &write_time,
      &io_num, &io_time, &weighted_io_time))
    {
      if(strncmp(device_name,"sd", 2) == 0){

        sector_read = sector_read_num;
        sector_write= sector_write_num;
        break;
      }

    }
    // sectors have a size of 512 bytes
    read_spd = (sector_read-last_sector_read)*millisecond_to_second/(double)(cpu_used-last_cpu_used)/bytes_to_megabytes;
    write_spd= (sector_write-last_sector_write)*millisecond_to_second/(double)(cpu_used-last_cpu_used)/bytes_to_megabytes;

    // memory monitor
    fp = fopen("/proc/meminfo","r");
    fscanf(fp,"%s %ld %s",read_buf,&mem_total,mem_unit);
    fscanf(fp,"%s %ld %s",read_buf,&mem_free,mem_unit);
    fscanf(fp,"%s %ld %s",read_buf,&mem_ava,mem_unit);
    fscanf(fp,"%s %ld %s",read_buf,&mem_buf,mem_unit);
    fscanf(fp,"%s %ld %s",read_buf,&mem_cache,mem_unit);
    fclose(fp);
    mem_usage = (double)(mem_total-mem_free-mem_buf-mem_cache)/mem_total*100;

    // print stats
    printf("CPU usage:\t%.2lf%c\n", cpu_usage, 37);
    printf("MEM usage:\t%.2lf%c\n", mem_usage, 37);
    printf("Read speed:\t%.2lfMB/s\n",read_spd);
    printf("Write speed:\t%.2lfMB/s\n\n",write_spd);
    lc = 0;
  }
  return 0;
}
