#include "common.h"
#include <inttypes.h>
#include <string.h>
#include <time.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

typedef struct _cache_block
{
  bool valid;
  bool dirty_bit;
  uint32_t tag;
  uint8_t data[BLOCK_SIZE];
} cache_block;
//cache_block cache[MEM_SIZE/BLOCK_SIZE];
static cache_block* cache;
static uint32_t total_num;
static uint32_t group_num;
static uint32_t row_num;

// TODO: implement the following functions

/* 按照数据对起处理 */

static bool check_hit(uintptr_t addr, uint32_t *row_addr){
  addr /= BLOCK_SIZE;
  uint32_t block_addr = addr % group_num;
  uint32_t dst_tag = addr / group_num;
  cache_block* cur = cache + block_addr * row_num;
  for(uint32_t i = 0; i < row_num; i++){
    if((cur+i)->valid && (cur+i)->tag == dst_tag){
      *row_addr = i;
      return true;
    }
  }
  *row_addr = 0;
  return false;
}

static void allocate(uintptr_t addr, uint32_t *row_addr){
  addr /= BLOCK_SIZE;
  uint32_t block_addr = addr % group_num;
  uint32_t dst_tag = addr / group_num;
  uint32_t ram_row = rand() % row_num;
  cache_block* cur = cache + block_addr * row_num + ram_row;
  if(cur->valid && cur->dirty_bit){
    mem_write(cur->tag * group_num + block_addr, (uint8_t *)&(cur->data));
  }
  mem_read(addr, (uint8_t *)&(cur->data));
  cur->valid = true;
  cur->dirty_bit = false;
  cur->tag = dst_tag;
}

uint32_t cache_read(uintptr_t addr) {
  uint32_t row_addr = 0;
  if(check_hit(addr, &row_addr)){
    allocate(addr, &row_addr);
  }
  cache_block *cur =  cache + (addr / BLOCK_SIZE) % group_num + row_addr;
  uint32_t *p =(void *)(cur->data)+ (addr & ~0x3) % BLOCK_SIZE;
  return *p;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint32_t row_addr = 0;
  if(check_hit(addr, &row_addr)){
    allocate(addr, &row_addr);
  }
  cache_block *cur =  cache + addr / BLOCK_SIZE % group_num + row_addr;
  uint32_t *p =(void *)(cur->data)+ (addr & ~0x3) % BLOCK_SIZE;
  *p = (*p & ~wmask) | (data & wmask);
  cur->dirty_bit = true;
}

void init_cache(int total_size_width, int associativity_width) {
  srand(time(NULL));
  row_num = 1 << associativity_width;
  total_num = 1 << (total_size_width - BLOCK_WIDTH) ;
  group_num = total_num / row_num;
  cache = (cache_block *) malloc(sizeof(cache_block)*total_num);
  memset(cache, 0, total_num);

}

void display_statistic(void) {
}
