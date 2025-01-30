#lang racket

(define base 10000)

(define (nat-to-blist n)
  (if (zero? n) empty (cons (modulo n base) (nat-to-blist (quotient n base)))))

(define (blist-to-nat bl)
  (if (empty? bl) 0 (+ (first bl) (* base (blist-to-nat (rest bl))))))

(define (add bl1 bl2)
  (define (add-helper bl1 bl2 carry)
    (cond
      [(and (empty? bl1) (empty? bl2) (zero? carry)) empty]
      [(and (empty? bl1) (empty? bl2)) (cons carry empty)]
      [(empty? bl1) (add-helper (cons 0 empty) bl2 carry)]
      [(empty? bl2) (add-helper bl1 (cons 0 empty) carry)]
      [else
       (let* ([sum (+ (first bl1) (first bl2) carry)]
              [bigit (modulo sum base)]
              [new-carry (quotient sum base)])
         (cons bigit (add-helper (rest bl1) (rest bl2) new-carry)))]))
  (add-helper bl1 bl2 0))

(define (mult bl1 bl2)
  (define (mult-helper bigit bl carry)
    (if (empty? bl)
        (if (zero? carry) empty (cons carry empty))
        (let* ([prod (+ (* bigit (first bl)) carry)]
               [bigit (modulo prod base)]
               [new-carry (quotient prod base)])
          (cons bigit (mult-helper bigit (rest bl) new-carry)))))
  
  (define (mult-by-bigit bigit bl shift)
    (append (make-list shift 0) (mult-helper bigit bl 0)))
  
  (define (mult-helper2 bl1 bl2 shift)
    (if (empty? bl1)
        empty
        (add (mult-by-bigit (first bl1) bl2 shift)
             (mult-helper2 (rest bl1) bl2 (+ shift 1)))))
  
  (mult-helper2 bl1 bl2 0))

;; (blist-to-nat (add (nat-to-blist 3) (nat-to-blist 3)))
;; (blist-to-nat (mult (nat-to-blist 3) (nat-to-blist 3)))