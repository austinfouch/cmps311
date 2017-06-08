#include <sys/utsname.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
/*
** Author:  Austin Fouch
** Date:    01/28/2017
** Assgn:   homework2
**
** Program prints out integers 1 through 10000, system info, and usage times
**
**      Notes:    utime | stime
**      no sleep: 0.004 | 0.008
**      sleep(5): 0.004 | 0.012
**      sleep(10): 0.000 | 0.016
**
** sleep(int x) does not affect the outcome of utime or stime because the
**      function makes the calling thread sleep untill x seconds have elapsed.
**      After x seconds, thread wakes up and continues
**
** getrusage(int who, struct rusage *usage) , specifically when the struct
**      called is RUSAGE_SELF, will return resource usage statistics for the
**      calling process, which is the sum of the resources used by threads (in
**      this case 1).
**
** sleep(int x) does not affect the return values of getrusage(int, rusage) due
**      to the thread which rusage has values for is sleeping for x seconds,
**      taking up no resources during the sleep.
**
*/
int main () {

  // system info
  struct utsname unameData;
  // resource usage
  struct rusage usageData;
  struct timeval utime, stime;

  // print out 1 through 10k
  for( int i = 0; i <= 10000; i++) {

    printf("%i\n", i);
  }

  // calling sleep(int x) does not affect system or user resource usage
  //sleep(10);

  // print system info
  uname(&unameData);
  printf("Operating System: %s\n", unameData.sysname);
  printf("Machine Name: %s\n", unameData.nodename);
  printf("Release Level: %s\n", unameData.release);
  printf("Version Level: %s\n", unameData.version);
  printf("Hardware Platform: %s\n", unameData.machine);

  // get and print resource usage data (in time)
  getrusage(RUSAGE_SELF, &usageData);
  utime = usageData.ru_utime;
  stime = usageData.ru_stime;
  printf("User Time: %lu.%06u\n", utime.tv_sec, utime.tv_usec);
  printf("System Time: %lu.%06u\n", stime.tv_sec, stime.tv_usec);

  // exit prog
  return 0;
}
