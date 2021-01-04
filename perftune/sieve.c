#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool vis[N];
static int  primes[N];

int *sieve(int n) {
  assert(n + 1 < N);
  memset(vis, 0, sizeof(vis));
  int cnt = 0;
  int tmp;
  for (int i = 2; i <= n; i++) {
    if(!vis[i]) primes[cnt++] = i;
    for(int j = 0; j < cnt && 1ll * i *(tmp = primes[j]) <= n; j++)
    { 
      
      vis[i*tmp] = 1;
      if(i % tmp == 0) break;
    }
  }

  return primes;
}
