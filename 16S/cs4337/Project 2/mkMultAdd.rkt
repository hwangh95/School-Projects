#lang racket
(provide mkMultAdd)
(define (mkMultAdd val1 val2)
    (lambda (x) (+ (* x val1) val2))
)
