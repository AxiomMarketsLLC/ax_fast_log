#include <stdio.h>
#include <stdint.h>
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

int main()
{
    unsigned long long x;
    unsigned long long y;
    x = rdtsc();
    printf("%lld\n",x);
    y = rdtsc();
    printf("%lld\n",y);
    printf("PERF: CYCLES: printf: %lld\n",y-x);
}
