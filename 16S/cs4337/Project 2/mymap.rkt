#lang racket
(provide mymap)
(define (mymap function inputlist)
    (if (not(empty? inputlist))
        (cons (function (car inputlist)) (mymap function (cdr inputlist)))
        empty
    )
)
