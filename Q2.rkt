#lang racket

(define-struct dict (word num))

(define (string->integer str)
  (if (and (string? str) (regexp-match? #rx"^-?[0-9]+$" str))
      (string->number str)
      "error"))

(define (input->list lst)
  (let ([c (read-char)])
  (cond
    [(eof-object? c) (reverse lst)]
    [else (input->list (cons c lst))]
    )))

(define (search-accumulator num lst)
  (cond
    [(empty? lst) empty] 
    [(= (dict-num (first lst)) num) (first lst)]
    [else (search-accumulator num (rest lst))]
    ))

(define (helper lst acc cur n)
  (cond
    [(empty? lst) (if (number? (string->integer cur)) (display (dict-word (search-accumulator (string->integer cur) acc))) (display cur))]
    [(char-whitespace? (first lst))
     (begin
       (if (number? (string->integer cur)) (display (dict-word (search-accumulator (string->integer cur) acc))) (display cur))
       (if (equal? #\newline (first lst)) (newline) (display " "))  
       (helper (rest lst)
                    (if (and (not (string=? cur "")) (string? (string->integer cur))) (cons (make-dict cur n) acc) acc)
                    "" 
                    (if (and (not (string=? cur "")) (string? (string->integer cur))) (add1 n) n)
                    )
       )] 
    [else (helper (rest lst) acc (string-append cur (string (first lst))) n)]
    )) 

(define (main x) (helper (input->list empty) empty "" 0))
(main 0)