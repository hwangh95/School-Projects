#lang racket
(provide luhn-test)
(define (luhn-test cc)
    (define (sum-odds args count)
        (if (not (zero? args))
            (if (= (modulo count 2) 1)
                (+ (modulo args 10) (sum-odds (quotient args 10) (add1 count)))
                (sum-odds (quotient args 10) (add1 count))
            )
            0
        )   
    )
    (define (sum-evens args count)
        (define (sum-double num)
            (+ (quotient (* num 2) 10) (modulo (* num 2) 10))
        )
        (if (not (zero? args))
            (if (= (modulo count 2) 0) ; if even digit
                (+ (sum-double (modulo args 10)) (sum-evens (quotient args 10) (add1 count)))
                (sum-evens (quotient args 10) (add1 count))
            )
            0
        )   
    )
    (if (= (modulo (+ (sum-odds cc 1) (sum-evens cc 1)) 10) 0)
       #t
       #f
    )
)
