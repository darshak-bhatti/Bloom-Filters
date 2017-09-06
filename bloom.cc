//
//  main.cpp
//  bloomFilters
//
//  Created by Darshak Bhatti, Saloni Desai on 4/16/17.
//

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <cstdio>
#include <set>
#include <utility>
#include <limits.h>
#include <math.h>
#include <bitset>
#include <stdint.h>

#define MAX_LIMIT 2000000

using namespace std;

struct bf_t {
    bitset<2000000> ba[8];
};


unsigned int fnv(char * s){
    
    const uint64_t fnv_prime = 0xcbf29ce484222325;
    uint64_t hash = 0;
    unsigned int i = 0;
    unsigned int len = strlen(s);
    
    for (i = 0; i<len; i++) {
        hash *= fnv_prime;
        hash ^= (s[i]);
    }
    
    //hash = (hash>>21) ^ (hash & MASK_21);
    
    return hash;
    
}

unsigned int fnv1a(char * s){
    
    const uint64_t fnv_prime = 0xcbf29ce484222325;
    uint64_t hash = 0;
    unsigned int i = 0;
    unsigned int len = strlen(s);
    
    for (i = 0; i<len; i++) {
        hash ^= (s[i]);
        hash *= fnv_prime;
    }
    
    //hash = (hash>>21) ^ (hash & MASK_21);
    
    return hash;
    
}

unsigned int Murmur ( char * key, unsigned int seed )
{
    const uint64_t multiply = 0xc6a4a7935bd1e995;
    const int rotate = 47;
	int len = strlen(key);
    
    uint64_t hash = seed ^ (len * multiply);
    
    const unsigned int * data = (const unsigned int *)key;
    const unsigned int * end = data + (len/8);
    
    while(data != end)
    {
        uint64_t k = *data++;
        
        k *= multiply;
        k ^= k >> rotate;
        k *= multiply;
        
        hash ^= k;
        hash *= multiply;
    }
    
    const unsigned char * value = (const unsigned char*)data;
    
    switch(len & 7)
    {
        case 7: hash ^= uint64_t(value[6]) << 48;
        case 6: hash ^= uint64_t(value[5]) << 40;
        case 5: hash ^= uint64_t(value[4]) << 32;
        case 4: hash ^= uint64_t(value[3]) << 24;
        case 3: hash ^= uint64_t(value[2]) << 16;
        case 2: hash ^= uint64_t(value[1]) << 8;
        case 1: hash ^= uint64_t(value[0]);
            hash *= multiply;
    };
    
    hash ^= hash >> rotate;
    hash *= multiply;
    hash ^= hash >> rotate;
    
    return hash;
}


unsigned int douleHashing(char * s, int i)
{
    int l = strlen(s);

    return ((Murmur(s, 1) + fnv(s) + i*fnv1a(s)) % MAX_LIMIT);
    
    
}

bf_t * create_bf(){
    struct bf_t  *head;
    head = (bf_t *)malloc(sizeof(bf_t));
    for (int i = 0; i<8; i++) {
        head->ba[i].reset();
    }
    return head;
}

void insert_bf(bf_t *b, char *s)
{
    unsigned int hash = 0;
    
    for (int i =0; i<8; i++) {
        hash = douleHashing(s, i);
        b->ba[i].set(hash);
        
    }
}

int is_element(bf_t *b, char *q){
    
    unsigned int hash = 0;
    
    
    for (int i =0; i<8; i++) {
        hash = douleHashing(q, i);
        if(!(b->ba[i].test(hash))){
            return 0;
        }
    }
    return 1;
}



void sample_string_A(char *s, long i)
{  s[0] = (char)(1 + (i%254));
    s[1] = (char)(1 + ((i/254)%254));
    s[2] = (char)(1 + (((i/254)/254)%254));
    s[3] = 'a';
    s[4] = 'b';
    s[5] = (char)(1 + ((i*(i-3)) %217));
    s[6] = (char)(1 + ((17*i+129)%233 ));
    s[7] = '\0';
}
void sample_string_B(char *s, long i)
{  s[0] = (char)(1 + (i%254));
    s[1] = (char)(1 + ((i/254)%254));
    s[2] = (char)(1 + (((i/254)/254)%254));
    s[3] = 'a';
    s[4] = (char)(1 + ((i*(i-3)) %217));
    s[5] = (char)(1 + ((17*i+129)%233 ));
    s[6] = '\0';
}
void sample_string_C(char *s, long i)
{  s[0] = (char)(1 + (i%254));
    s[1] = (char)(1 + ((i/254)%254));
    s[2] = 'a';
    s[3] = (char)(1 + ((i*(i-3)) %217));
    s[4] = (char)(1 + ((17*i+129)%233 ));
    s[5] = '\0';
}
void sample_string_D(char *s, long i)
{  s[0] = (char)(1 + (i%254));
    s[1] = (char)(1 + ((i/254)%254));
    s[2] = (char)(1 + (((i/254)/254)%254));
    s[3] = 'b';
    s[4] = 'b';
    s[5] = (char)(1 + ((i*(i-3)) %217));
    s[6] = (char)(1 + ((17*i+129)%233 ));
    s[7] = '\0';
}
void sample_string_E(char *s, long i)
{  s[0] = (char)(1 + (i%254));
    s[1] = (char)(1 + ((i/254)%254));
    s[2] = (char)(1 + (((i/254)/254)%254));
    s[3] = 'a';
    s[4] = (char)(2 + ((i*(i-3)) %217));
    s[5] = (char)(1 + ((17*i+129)%233 ));
    s[6] = '\0';
}





int main(int argc, const char * argv[]) {
    //cout << "Hello, World!\n";
    
    long i,j;
    bf_t * bloom;
    bloom = create_bf();
    printf("Created Filter\n");
    for( i= 0; i< 1450000; i++ )
    {  char s[8];
        sample_string_A(s,i);
        insert_bf( bloom, s );
    }
    for( i= 0; i< 500000; i++ )
    {  char s[7];
        sample_string_B(s,i);
        insert_bf( bloom, s );
    }
    for( i= 0; i< 50000; i++ )
    {  char s[6];
        sample_string_C(s,i);
        insert_bf( bloom, s );
    }
    printf("inserted 2,000,000 strings of length 8,7,6.\n");
    
    for( i= 0; i< 1450000; i++ )
    {  char s[8];
        sample_string_A(s,i);
        if( is_element( bloom, s ) != 1 )
        {  printf("found negative error (1)\n"); exit(0); }
    }
    for( i= 0; i< 500000; i++ )
    {  char s[7];
        sample_string_B(s,i);
        if( is_element( bloom, s ) != 1 )
        {  printf("found negative error (2)\n"); exit(0); }
    }
    for( i= 0; i< 50000; i++ )
    {  char s[6];
        sample_string_C(s,i);
        if( is_element( bloom, s ) != 1 )
        {  printf("found negative error (3)\n"); exit(0); }
    }
    j = 0;
    for( i= 0; i< 500000; i++ )
    {  char s[8];
        sample_string_D(s,i);
        if( is_element( bloom, s ) != 0 )
            j+=1;
    }
    for( i= 0; i< 500000; i++ )
    {  char s[7];
        sample_string_E(s,i);
        if( is_element( bloom, s ) != 0 )
            j+=1;
    }
    printf("Found %d positive errors out of 1,000,000 tests.\n",j);
    printf("Positive error rate %f\%.\n", (float)j/10000.0);
    
    
    
    cout<<endl;
    return 0;
}







