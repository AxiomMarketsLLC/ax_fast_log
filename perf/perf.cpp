#include <stdio.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include "../AxFastLog.hpp"

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



void test_printf_cycles(std::string testString) {
    unsigned long long x;
    unsigned long long y;
    const char* testChars = testString.c_str();
    x = rdtsc();
    printf(testChars);
    y = rdtsc();
    printf("\n");

    printf("PERF: CYCLES: printf: %lld\n",y-x);
}

void test_cout_cycles(std::string testString) {

    unsigned long long x;
    unsigned long long y;
    x = rdtsc();
    std::cout << testString << std::endl;
    y = rdtsc();
    printf("PERF: CYCLES: cout: %lld\n",y-x);
}

void test_axlog_cycles(std::string testString) {
    
    unsigned long long x;
    unsigned long long y;
    AxFastLog ax(LogEnums::FILE, "test.txt");
    x = rdtsc();
    ax.log(testString, LogEnums::INFO);
    y = rdtsc();
    printf("PERF: CYCLES: ax_log (file): %lld\n",y-x);

    AxFastLog axc(LogEnums::CNSL);
    x = rdtsc();
    axc.log(testString, LogEnums::INFO);
    y = rdtsc();
    printf("PERF: CYCLES: ax_log (cons): %lld\n",y-x);
}

int main()
{

   
    std::string testString = "TESTING 1, 2, 3"; 
    test_printf_cycles(testString);
    test_cout_cycles(testString);
    test_axlog_cycles(testString);

    
  
 
}
