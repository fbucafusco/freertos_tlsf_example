#ifndef TLSF_CONFIG_H
#define TLSF_CONFIG_H


/*
** Constants.
*/

/* Public constants: may be modified. */
enum tlsf_public
{
    /* log2 of number of linear subdivisions of block sizes. Larger
    ** values require more memory in the control structure. Values of
    ** 4 or 5 are typical.
    */
    SL_INDEX_COUNT_LOG2 = 5,
};


#endif
