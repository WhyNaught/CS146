#lang racket

(define-struct dict (word num))

(define (search-accumulator word lst)
  (cond
    [(empty? lst) empty]
    [(string=? (dict-word (first lst)) word) (first lst)]
    [else (search-accumulator word (rest lst))]
    ))

(define (input->list lst)
  (let ([c (read-char)])
  (cond
    [(eof-object? c) (reverse lst)]
    [else (input->list (cons c lst))]
    )))

(define (helper lst acc cur n)
  (cond
    [(empty? lst) (if (not (empty? (search-accumulator cur acc))) (display (dict-num (search-accumulator cur acc))) (display cur))]
    [(char-whitespace? (first lst)) (begin
            (if (not (empty? (search-accumulator cur acc))) (display (dict-num (search-accumulator cur acc))) (display cur))
            (if (equal? #\newline (first lst)) (newline) (display " "))
            (helper (rest lst)
                    (if (and (not (string=? cur "")) (empty? (search-accumulator cur acc))) (cons (make-dict cur n) acc) acc)
                    ""
                    (if (and (not (string=? cur "")) (empty? (search-accumulator cur acc))) (add1 n) n))
            )] 
    [else (helper (rest lst) acc (string-append cur (string (first lst))) n)]
    ))

(define (main x) (helper (input->list empty) empty "" 0))

(main 0) 