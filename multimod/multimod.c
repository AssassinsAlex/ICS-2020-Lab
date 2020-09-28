#include <stdint.h>

#define MOD_ADD_U(x, y, m) ((x)+ (y) < (x)) ? (((x+y+1)%(m) + (~0UL)%(m))%(m)) : (((x)+(y))%(m)) 


static void cal_mpow_2(uint64_t b, uint64_t m, uint64_t b_pow_2[64]){
    b_pow_2[0] = b;
    for(int i = 1; i < 64; i++){
        b_pow_2[i] = MOD_ADD_U(b_pow_2[i-1], b_pow_2[i-1], m);
    }   
}


uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t result = 0; 
    uint64_t b_pow_2[64];

    cal_mpow_2(b, m, b_pow_2);

    for(int i = 0; i < 64; i++){
        uint8_t flag = (a >> i) & 1;
        result = MOD_ADD_U(result, (flag==1)?b_pow_2[i]:0, m);
    }
    return result;
}
