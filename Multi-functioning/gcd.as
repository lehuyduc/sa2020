var:
    int i n x y tmp condi res
    int[] a[100] b[100]

text:
    SET n 5

    SETA a 0 98
    SETA b 0 56

    SETA a 1 48
    SETA b 1 18

    SETA a 2 59
    SETA b 2 21

    SETA a 3 144
    SETA b 3 60

    SETA a 4 1000000007
    SETA b 4 17

    SET i 0
    LABEL LOOP1:
        GETA x a i      // x = a[i]
        GETA y b i      // y = b[i]

        // while (y!=0) {
        //     x %= y;
        //     swap(x,y);
        // }
        LABEL GCD:      // gcd(x,y)
            CMPE condi y 0
            JUMPIF RESULT condi

            MOD x y
            SET tmp x
            SET x y
            SET y tmp

            JUMPIF GCD 1

        LABEL RESULT:
            PRINTINT x      // x is the gcd after the function

        INC i
        CMPL condi i n
    JUMPIF LOOP1 condi

    PRINTLN
