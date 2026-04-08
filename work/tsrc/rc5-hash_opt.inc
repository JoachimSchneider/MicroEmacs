/* RC5REF.C -- Reference implementation of RC5-32/12/16 in C. */
/* Copyright (C) 1995 RSA Data Security, Inc.                 */
/* R.L. Rivest                                                */
/* Small change - ADD(), SUB() - working on 64-bit systems.   */


typedef struct  {
    WORD_T_ m;      /* Bitmask for word                           */
    WORD_T_ P;      /* magic constant                             */
    WORD_T_ Q;      /* magic constant                             */
    WORD_T_ S[T_MAX]; /* expanded key table                       */
} WORDVALS_X_;

typedef RC5_ENCRYPT_X_T(p_rc5_parm_t, WORD_T_ *, WORD_T_ *);
void RC5_ENCRYPT_X_(p_rc5_parm_t R,
                    WORD_T_    *pt,  /* 2 Word input pt  */
                    WORD_T_    *ct   /* 2 Word output ct */
                   )
{
    int     i = 0;
    WORD_T_ A = ADD(pt[0], R->S[0], R->m);
    WORD_T_ B = ADD(pt[1], R->S[1], R->m);

    for ( i = 1; i <= R->r; i++ ) {
        A = ADD(ROTL(A ^ B, B, R->w), R->S[2*i], R->m);
        B = ADD(ROTL(B ^ A, A, R->w), R->S[2*i+1], R->m);
    }
    ct[0] = A;
    ct[1] = B;
}

typedef RC5_DECRYPT_X_T(rc5_parm_t *, WORD_T_ *, WORD_T_ *);
void RC5_DECRYPT_X_(p_rc5_parm_t R,
                    WORD_T_    *ct,  /* 2 Word input ct  */
                    WORD_T_    *pt   /* 2 Word output pt */
                   )
{
    int     i = 0;
    WORD_T_ A = ct[0];
    WORD_T_ B = ct[1];

    for ( i = R->r; i > 0; i-- )  {
        B = ROTR(SUB(B, R->S[2*i+1], R->m), A, R->w) ^ A;
        A = ROTR(SUB(A, R->S[2*i], R->m),   B, R->w) ^ B;
    }
    pt[0] = SUB(A, R->S[0], R->m);
    pt[1] = SUB(B, R->S[1], R->m);
}


/* EOF */
