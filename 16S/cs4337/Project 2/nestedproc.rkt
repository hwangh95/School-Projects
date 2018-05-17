#lang racket
(provide nestedproc)
(define (nestedproc func1 func2 val)
    (func2 (func1 val)) 
)
