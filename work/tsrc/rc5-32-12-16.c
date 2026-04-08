/* RC5REF.C -- Reference implementation of RC5-32/12/16 in C. */
/* Copyright (C) 1995 RSA Data Security, Inc.                 */
/* R.L. Rivest                                                */
/* Small change - ADD(), SUB() - working on 64-bit systems.   */

#include <stdio.h>
#include <string.h>


typedef unsigned long int WORD; /* Should be 32-bit = 4 bytes         */
#define BIB   (8)               /* Bits In Byte                       */
#define w     32                /* word size in bits                  */
#define wmask 0xFFFFFFFF
#define r     12                /* number of rounds                   */
#define b     16                /* number of bytes in key             */
#define c     ((BIB*b+(w-1))/w) /* number words in key: ceil(BIB*b/w) */
#define t     (2*(r+1))         /* size of table S = 2*(r+1) words    */
WORD S[t];                      /* expanded key table                 */
WORD P = 0xb7e15163, Q = 0x9e3779b9;  /* magic constants              */

/* Rotation operators, x must be unsigned, to get logical right shift */
#define ROTL(x,y)   (((x)<<((y)&(w-1))) | ((x)>>(w-((y)&(w-1)))))
#define ROTR(x,y)   (((x)>>((y)&(w-1))) | ((x)<<(w-((y)&(w-1)))))
#define ADD(x, y)   ( ((x) + (y)) & wmask )
#define SUB(x, y)   ( ((x) - (y)) & wmask )
#define MAX2(x, y)  ( (x) > (y) ? (x) : (y) )

#define ZEROMEM(x)  ( memset(&(x), 0, sizeof((x))) )

void RC5_ENCRYPT(WORD *pt, WORD *ct) /* 2 WDRD input pt/output ct     */
{
    int   i = 0;
    WORD  A =ADD(pt[0], S[0]);
    WORD  B =ADD(pt[1], S[1]);

    for ( i = 1; i <= r; i++ )  {
        A = ADD(ROTL(A ^ B, B), S[2*i]);
        B = ADD(ROTL(B ^ A, A), S[2*i+1]);
    }
    ct[0] = A;
    ct[1] = B;
}

void RC5_DECRYPT(WORD *ct, WORD *pt)  /* 2 WORD input ct/output pt    */
{
    int   i = 0;
    WORD  A = ct[0];
    WORD  B = ct[1];

    for ( i = r; i > 0; i-- ) {
        B = ROTR(SUB(B, S[2*i+1]), A) ^ A;
        A = ROTR(SUB(A, S[2*i]),   B) ^ B;
    }
    pt[0] = SUB(A, S[0]);
    pt[1] = SUB(B, S[1]);
}

void RC5_SETUP(unsigned char *K)      /* secret input key K[0...b-1]  */
{
    int   i = 0;
    int   j = 0;
    int   k = 0;
    WORD  u = w/BIB;
    WORD  A = 0;
    WORD  B = 0;
    WORD  L[c];

    ZEROMEM(L);

    /* Initialize L, then S, then mix key into S */
    for ( i= b - 1, L[c-1] = 0; i != -1; i-- )  {
        L[i/u] = ADD((L[i/u] << BIB), K[i]);
    }
    for ( S[0] = P, i = 1; i < t; i++ ) {
        S[i] = ADD(S[i-1], Q);
    }
    for ( A = B = i = j = k = 0;
          k < 3 * MAX2(t, c);
          k++, i = (i + 1) % t, j = (j + 1) %c )  /* 3*t > 3*c  */
    {
        A = S[i] = ROTL(ADD(S[i], ADD(A, B)), 3);
        B = L[j] = ROTL(ADD(L[j], ADD(A, B)), ADD(A, B));
    }
}


int main(int argc, char *argv[])
{
    int   rc      = 0;
    int   i       = 0;
    int   j       = 0;
    WORD  pt1[2]  = { 0, 0 };
    WORD  pt2[2]  = { 0, 0 };
    WORD  ct[2]   = { 0, 0 };
    unsigned char key[b];

    ZEROMEM(key);

    if ( sizeof(WORD) != 4 )  {
        printf("RC5 warning: WORD has %d bytes.\n", (int)sizeof(WORD));
    }
    printf("%s\n", "RC5-32/12/16 examples:");
    for ( i = 1; i < 6; i++ ) {
        /* Initialize pt1 and key pseudorandomly based on previous ct */
        pt1[0] = ct[0];
        pt1[1] = ct[1];
       for ( j = 0; j < b; j++) {
          key[j] = ct[0] % (255-j);
       }
        /* Setup, encrypt, and decrypt */
        RC5_SETUP(key);
        RC5_ENCRYPT(pt1, ct);
        RC5_DECRYPT(ct, pt2);
        /* Print out results, checking for decryption failure */
        printf("\n%d, key = ", (int)i);
        for ( j = 0; j < b; j++ ) {
            printf("%.2X ", key[j]);
        }
        printf("\n   plaintext %.8lX %.8lX ---> ciphertext %.8lX %.8lX \n",
               pt1[0], pt1[1], ct[0], ct[1]);
        if ( pt1[0] != pt2[0] || pt1[1] != pt2[1] ) {
            printf("DecryptionError!");
            rc++;
        }
    }

    return rc;
}


/* EOF */
