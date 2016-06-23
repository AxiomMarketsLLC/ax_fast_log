#include <stdio.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <cmath>
#include "../AxFastLog.hpp"
#include "tcp_client.hpp"

#define AX_FPATH "test.txt"
#define AX_SPORT 9000
#define TEST_ITERS 1
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



unsigned long long test_printf_cycles(std::string testString, const int iter) {
   
    unsigned long long x;
    unsigned long long y;
    unsigned long long sum=0;
    const char* testChars = testString.c_str();
    for(int i =0; i<=iter;++i) {
    	x = rdtsc();
    	printf(testChars);
    	y = rdtsc();
    	sum+=(y-x);
   	printf("\n");
    }
    return sum/iter;
}

unsigned long long test_cout_cycles(std::string testString, const int iter) {

    unsigned long long x;
    unsigned long long y;
    unsigned long long sum=0;
    for(int i =0; i<=iter;++i) {
    	x = rdtsc();
    	std::cout << testString << std::endl;
    	y = rdtsc();
    	sum+=(y-x);
    }
    return sum/iter;

}

unsigned long long test_axlog_cycles(AxFastLog& ax, std::string testString, const int iter) {
    
    unsigned long long x;
    unsigned long long y;
    unsigned long long sum=0;
    
    for(int i =0; i<=iter;++i) {
    	x = rdtsc();
    	ax.log(testString, LogEnums::INFO);
    	y = rdtsc();
    	sum+=(y-x);
    }
    return sum/iter;
}

int main()
{

    AxFastLog axf (LogEnums::FILE, AX_FPATH);
    AxFastLog axs (LogEnums::SCKT, AX_SPORT);
    AxFastLog axc (LogEnums::CNSL);
	 
    tcp_client cli;

    unsigned long long avg;

    std::string testString = "TESTING 1, 2, 3"; 
    avg = test_printf_cycles(testString, TEST_ITERS);
    printf("PERF: AVG CYCLES OVER %d ITERS: printf: %llu\n",TEST_ITERS,avg);
    avg = test_cout_cycles(testString, TEST_ITERS);
    printf("PERF: AVG CYCLES OVER %d ITERS: cout: %llu\n",TEST_ITERS,avg);
    avg = test_axlog_cycles(axf, testString, TEST_ITERS); 
    printf("PERF: AVG CYCLES OVER %d ITERS: axf: %llu\n",TEST_ITERS,avg);
    avg = test_axlog_cycles(axc, testString, TEST_ITERS);
    printf("PERF: AVG CYCLES OVER %d ITERS: axc: %llu\n",TEST_ITERS,avg);
    
  
 
}
