#lang racket
(provide zipper)
(define (zipper list1 list2)
    (if (and (not(empty? list1)) (not (empty? list2)))
        (cons (list (car list1) (car list2)) (zipper (cdr list1) (cdr list2)))
        empty
    )
)
