var:
    int i n tmp f1 f2 j
    int[] f[100]

text:
    SET n 10
    SETA f 0 5
    SETA f 1 10

    SET i 2
    LABEL POS1:
        SET j i         // j = i
        SUB j 1         // j = j-1
        GETA f1 f j    // tmp = f[j]

        SET j i
        SUB j 2
        GETA f2 f j

        SET tmp f1
        ADD tmp f2

        SETA f i tmp    // f[i] = tmp

        PRINTINT tmp
        PRINTLN

        INC i
        CMPLE tmp i n    // tmp = i==n,
    JUMPIF POS1 tmp

    GETA tmp f n
    PRINTINT tmp
    PRINTLN

