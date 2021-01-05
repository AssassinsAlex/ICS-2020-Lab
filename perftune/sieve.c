#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool vis[N];
static int  primes[N];

int *sieve(int n) {
  assert(n + 1 < N);
  //memset(vis, 0, sizeof(vis));
  int cnt = 0;
  primes[cnt++] = 2;
  primes[cnt++] = 3;
  for (int i = 2; i <= n; i++) {
    if(i%2 == 0 || i%3 == 0 ) continue;
    if(!vis[i]) primes[cnt++] = i;
    for(int j = 2; j < cnt && i * primes[j] <= n; j++)
    { 
      vis[i*primes[j]] = 1;
      if(i % primes[j] == 0) break;
    }
  }

  return primes;
}
