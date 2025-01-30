#lang racket

(define-struct token (type value))

(define (tokenize str)
  (define delimiters '(#\space #\( #\) #\'))
  (define (is-delimiter? char)
    (member char delimiters))
  
  (define (split-into-tokens chars current-token tokens)
    (cond
      [(empty? chars)
       (if (empty? current-token)
           (reverse tokens)
           (reverse (cons (list->string current-token) tokens)))]
      [(is-delimiter? (car chars))
       (let ([new-tokens (if (empty? current-token)
                             tokens 
                             (cons (list->string current-token) tokens))])
         (if (eq? (car chars) #\space)
             (split-into-tokens (cdr chars) '() new-tokens)
             (split-into-tokens (cdr chars) '() (cons (string (car chars)) new-tokens))))]
      [else
       (split-into-tokens (cdr chars) (cons (car chars) current-token) tokens)]))
  
  (split-into-tokens (string->list str) '() '()))

(define (list->string2 lst)
  (format "~a" lst))

(define (string-is-number? str)
  (not (false? (string->number str))))

(define (strings->tokens lst acc)
  (cond
    [(empty? lst) acc]
    [(string-is-number? (first lst)) (strings->tokens (rest lst) (cons (make-token 'num (string->number (first lst))) acc))]
    [(string=? (first lst) "(") (strings->tokens (rest lst) (cons (make-token 'lp "(") acc))]
    [(string=? (first lst) ")") (strings->tokens (rest lst) (cons (make-token 'rp ")") acc))]
    [(string=? (first lst) "+") (strings->tokens (rest lst) (cons (make-token 'op '+) acc))]
    [(string=? (first lst) "*") (strings->tokens (rest lst) (cons (make-token 'op '*) acc))]
    [else (strings->tokens (rest lst) (cons (make-token 'id (string->symbol (first lst))) acc))]
    ))

(define (lst->tokens lst acc)
  (cond
    [(empty? lst) (reverse acc)]
    [(number? (first lst)) (lst->tokens (rest lst) (cons (make-token 'num (first lst)) acc))]
    [(or (equal? (first lst) '+) (equal? (first lst) '*)) (lst->tokens (rest lst) (cons (make-token 'op (first lst)) acc))]
    [(symbol? (first lst)) (lst->tokens (rest lst) (cons (make-token 'id (first lst)) acc))]
    [(list? (first lst))
     (lst->tokens (rest lst)
                  (strings->tokens (tokenize (list->string2 (first lst))) acc))]
                  ;;(append acc (strings->tokens (tokenize (list->string2 (first lst))) empty)))]
    ))

(define (infix->prefix expr)
  ;; Entry point for converting an infix expression to prefix notation
  (cond
    ;; If the expression is a single number or identifier, return it as is
    [(number? expr) expr]
    [(symbol? expr) expr]
    ;; If the expression is a list, parse it
    [(list? expr) (parse-expression expr)]
    ;; Otherwise, it's an invalid input
    [else (error "bad expression")]))

;; Recursive descent parser
(define (parse-expression tokens)
  (define-values (ast remaining) (parse-plus tokens))
  (if (null? remaining)
      ast
      (error "bad expression")))

;; Parse `+` operations, which have the lowest precedence
(define (parse-plus tokens)
  (let loop ([tokens tokens] [accumulator '()])
    (define-values (term remaining) (parse-mul tokens))
    (cond
      ;; If we see a `+`, continue accumulating terms
      [(and (pair? remaining) (equal? (first remaining) '+))
       (loop (rest remaining) (append accumulator (list term)))]
      ;; Otherwise, stop parsing `+`
      [else (values (if (null? accumulator)
                        term
                        (cons '+ (append accumulator (list term))))
                    remaining)])))

;; Parse `*` operations, which have higher precedence
(define (parse-mul tokens)
  (let loop ([tokens tokens] [accumulator '()])
    (define-values (factor remaining) (parse-factor tokens))
    (cond
      ;; If we see a `*`, continue accumulating terms
      [(and (pair? remaining) (equal? (first remaining) '*))
       (loop (rest remaining) (append accumulator (list factor)))]
      ;; Otherwise, stop parsing `*`
      [else (values (if (null? accumulator)
                        factor
                        (cons '* (append accumulator (list factor))))
                    remaining)])))

;; Parse factors (numbers, identifiers, or sub-expressions in parentheses)
(define (parse-factor tokens)
  (cond
    ;; If the token is a number or identifier, return it
    [(number? (first tokens)) (values (first tokens) (rest tokens))]
    [(symbol? (first tokens)) (values (first tokens) (rest tokens))]
    ;; If the token is an open parenthesis, parse the sub-expression
    [(equal? (first tokens) "(")  
     (define-values (subexpr remaining) (parse-plus (rest tokens)))
     (if (and (pair? remaining) (equal? (first remaining) ")"))
         (values subexpr (rest remaining))
         (error "bad expression"))]
    ;; Otherwise, it's an invalid token
    [else (error "bad expression")])) 

(define (show-values lst acc)
  (cond
    [(empty? lst) (reverse acc)]
    [else (show-values (rest lst) (cons (token-value (first lst)) acc))]
    ))

(define (in->pre lst)
  (cond
    [(list? lst) (infix->prefix (show-values (lst->tokens lst empty) empty))]
    [else lst]
    ))  