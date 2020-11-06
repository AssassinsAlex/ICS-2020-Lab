#include <stdint.h>
#include <stddef.h>

// TODO: replace with your definition

typedef struct asm_jmp_buf{
    uint64_t gpr[8];
}asm_jmp_buf[1];
#include <setjmp.h> // remove this
#define asm_jmp_buf jmp_buf

int64_t asm_add(int64_t a, int64_t b);
int     asm_popcnt(uint64_t x);
void   *asm_memcpy(void *dest, const void *src, size_t n);
int     asm_setjmp(asm_jmp_buf env);
void    asm_longjmp(asm_jmp_buf env, int val);

int64_t asm_add(int64_t a, int64_t b){
    asm(
        "addq %%rbx, %%rax;"
        : "=a"(a)
        : "a"(a),"b"(b)    
    );
    return a;
    
}

int  asm_popcnt(uint64_t x){
    int s = 0;
    asm(
        "movq $0, %%rcx;"
        "0:movq %%rbx, %%rdx;"
        "shrq %%cl, %%rdx;"
        "and $1, %%rdx;"
        "addq %%rdx, %%rax;"
        "incq %%rcx;"
        "cmp $64, %%rcx;"
        "jl 0b;"
        : "=a"(s)
        : "a"(s),"b"(x)
        : "rdx", "rcx"
    );
    printf("%d\n",s);
    return s;;
}

void *asm_memcpy(void *dest, const void *src, size_t n){
    asm(
        "test %0, %0;"
        "je 1f;"
        "test %1, %1;"
        "je 1f;"
        "movq $0, %%rax;"
        "0: cmp %2, %%rax;"
        "jge 1f;" 
        "movb (%1, %%rax), %%cl;"
        "movb %%cl, (%0, %%rax);"
        "incq %%rax;"
        "jmp 0b;"
        "1:"
        : 
        : "r"(dest), "r"(src), "r"(n)
        : "rax", "rcx"
    );
    return dest;
}

int     asm_setjmp(asm_jmp_buf env){
    int ret;
    asm(
        "movq $0, %%rax;"
        "movq %%rbx, (%%rdi);"
        "movq %%rbp, 0x8(%%rdi);"
        "movq %%rsp, 0x10(%%rdi);"
        "movq %%r12, 0x18(%%rdi);"
        "movq %%r13, 0x20(%%rdi);"
        "movq %%r14, 0x28(%%rdi);"
        "movq %%r15, 0x30(%%rdi);"
        "movq 0x8(%%rbp), %%rdx;"
        "movq %%rdx, 0x38(%%rdi);"
        : "=a"(ret)
        : "D"(env)
        : "rdx"
    );
    return ret;
}

void    asm_longjmp(asm_jmp_buf env, int val){
    asm(
        "movq (%%rdi), %%rbx;"
        "movq 0x8(%%rdi), %%rbp;"
        "movq 0x10(%%rdi), %%rsp;"
        "movq 0x18(%%rdi), %%r12;"
        "movq 0x20(%%rdi), %%r13;"
        "movq 0x28(%%rdi), %%r14;"
        "movq 0x30(%%rdi), %%r15;"
        "movq %%rsi, %%rax;"
        "movq 0x38(%%rdi), %%rdx;"
        "jmpq *%%rdx;"
        :
        : "D"(env), "S"(val) 
        : "rdx"
    );
}
