#include <stdio.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <cmath>

#include "../AxFastLog.hpp"
#include "TcpClient.hpp"
#include <omp.h>

#define AX_FPATH "test.txt"
#define TEST_STR "TESTING 1, 2, 3"
#define AX_HOST "localhost"
#define AX_SPORT 9000
#define TEST_ITERS 100
#define MS_MULTI 1000
#define US_MULTI 1000000
#define NO_BUFFER_PRINTF
//from intel devel guide: indirect: http://stackoverflow.com/questions/459691/best-timing-method-in-ci
inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ (
      "xorl %%eax, %%eax\n"
      "cpuid\n"
      "rdtsc\n"
      : "=a" (lo), "=d" (hi)
      :
      : "%ebx", "%ecx");
    return (uint64_t)hi << 32 | lo;
}

std::pair<unsigned long long, double> test_printf_cycles(std::string testString, const int iter) {
   
    unsigned long long x, y, sum=0;
    double begin, end;
    testString+="\n";
    const char* testChars = testString.c_str();
    begin = omp_get_wtime();
    for(int i =0; i<=iter;++i) {
    	x = rdtsc();
    	printf(testChars);
   	y = rdtsc();
    	sum+=(y-x);
    }
    end=omp_get_wtime();
    return std::make_pair(sum/iter,end-begin);
}

std::pair<unsigned long long, double> test_fprintf_cycles(std::string testString, const int iter) {
   
    unsigned long long x, y, sum=0;
    double begin, end;
    testString+="\n";
    const char* testChars = testString.c_str(); 
    begin = omp_get_wtime();
    for(int i =0; i<=iter;++i) {
    	x = rdtsc();
    	fprintf(stdout, testChars);
    	y = rdtsc();
    	sum+=(y-x);
    }
    end = omp_get_wtime();
    return std::make_pair(sum/iter,end-begin);
}

std::pair<unsigned long long,double> test_cout_cycles(std::string testString, const int iter) {

    unsigned long long x, y, sum=0;
    double begin, end;
    begin = omp_get_wtime();
    for(int i =0; i<=iter;++i) {
	x = rdtsc();
    	std::cout << testString << std::endl;
    	y = rdtsc();
    	sum+=(y-x);
    }
    end = omp_get_wtime();
    return std::make_pair(sum/iter, end-begin);

}


std::pair<unsigned long long,double> test_axlog_cycles(AxFastLog& ax, std::string testString, const int iter) {
    
    unsigned long long x,y,sum=0;
    double begin,end;
    
    begin = omp_get_wtime();
    for(int i =0; i<=iter;++i) {
    	x = rdtsc();
    	ax.log(testString, LogEnums::INFO);
    	y = rdtsc();
    	sum+=(y-x);
    }
    end=omp_get_wtime();
    return std::make_pair(sum/iter,end-begin);
}


void cliTask(TcpClient& cli) {
try {
while(true) {
  boost::this_thread::interruption_point();
  cli.receive(1024);
}
}
catch (boost::thread_interrupted&) {cli.close_socket();}

}
int main()
{

    #ifdef NO_BUFFER_PRINTF
    setvbuf (stdout, NULL, _IONBF, 0);
    #endif
    AxFastLog axf (LogEnums::FILE, AX_FPATH);
    AxFastLog axs (LogEnums::SCKT, AX_SPORT);
    AxFastLog axc (LogEnums::CNSL);
    usleep(1000);
    TcpClient cli;
    cli.conn(AX_HOST, AX_SPORT, true);
    std::unique_ptr<boost::thread> cliThread = std::unique_ptr<boost::thread>(new boost::thread(&cliTask,cli));

    std::pair<unsigned long long, double> avg;

    std::string testString = "TESTING 1, 2, 3"; 
    
    usleep(10000);

    avg = test_printf_cycles(testString, TEST_ITERS);
    printf("PERF: AVG CYCLES OVER %d ITERS: printf: %llu\n",TEST_ITERS,avg.first);
    printf("PERF: WALLTIME OVER %d ITERS: printf: %f\n",TEST_ITERS,avg.second*US_MULTI);
    
    avg = test_fprintf_cycles(testString, TEST_ITERS);
    printf("PERF: AVG CYCLES OVER %d ITERS: fprintf: %llu\n",TEST_ITERS,avg.first);
    printf("PERF: WALLTIME OVER %d ITERS: fprintf: %f\n",TEST_ITERS,avg.second*US_MULTI);
    
    avg = test_cout_cycles(testString, TEST_ITERS);
    printf("PERF: AVG CYCLES OVER %d ITERS: cout: %llu\n",TEST_ITERS,avg.first);
    printf("PERF: WALLTIME OVER %d ITERS: cout: %f\n",TEST_ITERS,avg.second*US_MULTI);

    avg = test_axlog_cycles(axf, testString, TEST_ITERS); 
    printf("PERF: AVG CYCLES OVER %d ITERS: axf: %llu\n",TEST_ITERS,avg.first);
    printf("PERF: WALLTIME OVER %d ITERS: axf: %f\n",TEST_ITERS,avg.second*US_MULTI);
    
    avg = test_axlog_cycles(axc, testString, TEST_ITERS);
    printf("PERF: AVG CYCLES OVER %d ITERS: axc: %llu\n",TEST_ITERS,avg.first);
    printf("PERF: WALLTIME OVER %d ITERS: axc: %f\n",TEST_ITERS,avg.second*US_MULTI);

    avg = test_axlog_cycles(axs, testString, TEST_ITERS);
    printf("PERF: AVG CYCLES OVER %d ITERS: axs: %llu\n",TEST_ITERS,avg.first);
    printf("PERF: WALLTIME OVER %d ITERS: axs: %f\n",TEST_ITERS,avg.second*US_MULTI); 
    
    cliThread->interrupt();
    cliThread->join();
   
    
}
