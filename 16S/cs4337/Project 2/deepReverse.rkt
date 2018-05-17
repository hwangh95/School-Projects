#lang racket
(provide deepReverse)
(define (deepReverse inputlist)
    (if (not(empty? inputlist))
        (if (not(list? inputlist))
            inputlist
            (append (deepReverse (cdr inputlist)) (list (deepReverse (car inputlist))))
        )
        empty
    )
)
