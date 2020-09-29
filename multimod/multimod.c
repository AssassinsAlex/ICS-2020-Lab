#include <stdint.h>

static uint64_t m_pow_2[64];



static uint64_t mod(uint64_t a, uint64_t m ){
    int i  = 63;
    while(i >= 0 && a > m){
        if(a > m_pow_2[i])
            a = a-m_pow_2[i];
        else
            i--;
    }return a;
}

static uint64_t mod_add_u(uint64_t x, uint64_t y, uint64_t m){
    if(x + y < x)
        return mod(mod( x+y+1,m) + mod(UINT64_MAX,m),m);
    else
        return mod(x+y,m);
    
}


static void cal_mpow_2(uint64_t b, uint64_t m, uint64_t b_pow_2[64]){
    b_pow_2[0] = b;
    for(int i = 1; i < 64; i++){
        b_pow_2[i] = mod_add_u(b_pow_2[i-1], b_pow_2[i-1], m);
    }   
}


uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t result = 0; 
    uint64_t b_pow_2[64];

    cal_mpow_2(b, m, b_pow_2);

    m_pow_2[0] = m;
    for(int i = 1; i < 64; i++){
        if(m_pow_2[i-1]+ m_pow_2[i-1]> m_pow_2[i-1]){
            m_pow_2[i] = m_pow_2[i-1] + m_pow_2[i-1];
        }else
        {
            m_pow_2[i] = UINT64_MAX;
        }   
    }

    for(int i = 0; i < 64; i++){
        uint8_t flag = (a >> i) & 1;
        result = mod_add_u(result, flag?b_pow_2[i]:0, m);
    }
    return result;
}
