# Monitor

### Functionality

See the CPU usage, memory usage, read and write speed on a computer the program runs on.

### How to use

There is a Makefile letting you be able to run the program easily! Just use the command

```shell
make run
```

If you want to remove the complied files, you can use the command

```shell
make clean
```

### Trouble I met

Not sure if I measure CPU usage, memory usage, read and write speed correctly.

Here I put some references for you to know what measurement bases on. I only refer the formula and machanism they used, didn't copy their code. But maybe what they looks would be a little similiar to each other.

* CPU usage
   * https://www.kernel.org/doc/Documentation/filesystems/proc.txt
   * https://stackoverflow.com/questions/3769405/determining-cpu-utilization
   * http://www.coctec.com/docs/linux/show-post-185667.html
* Memory usage
   * my program chose to display "Non cache/buffer memory" in https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop
* Read/Write speed
   * https://stackoverflow.com/questions/37248948/how-to-get-disk-read-write-bytes-per-second-from-proc-in-programming-on-linux
   * https://www.kernel.org/doc/Documentation/ABI/testing/procfs-diskstats
   * Trace code of https://github.com/sysstat/sysstat/blob/master/iostat.c
