## Some thinks about problem

Valid symbols is [A-Za-z\-].
For encode every symbol of this range we can use only 6 bits log2(28 * 2 + 1) \approx 5.8 \approx 6.
Thus, we can reduce number of possible values by 2^(2*bytes). Then we need compute 2^(6*hash_bytes - known_bytes*6).
In our case this will be 2^(6*16 - 6*4) = 2^72 options.
(Bit mask of this space is 0b01..'....)

If this assumption turns out to be incorrect, then we will be made to compute 2^84 options.