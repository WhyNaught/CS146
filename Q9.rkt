#lang racket

(provide compile-simpl)

(define (compile-pop) ;; stack mgmt.
  (list
   (list 'sub 'SP 'SP 1) 
   ))
(define (compile-push v)
   (list
    (list 'move (list 0 'SP) v)
    (list 'add 'SP 'SP 1) 
    ))

(define (compile-op op aexp1 aexp2)
  (append
   (compile-aexp aexp1) 
   (compile-aexp aexp2)
   (list
    (list op (list -2 'SP) (list -2 'SP) (list -1 'SP)) 
    )
   (compile-pop)
   ))
(define (compile-bexp bexp)
  (define (recursive-compile op exp-lst)
    (define (rec-comp-h op exp-lst) 
      (cond
        [(empty? exp-lst) '()]
        [true
         (append
          (compile-bexp (car exp-lst))
          (list
           (list op (list -2 'SP) (list -2 'SP) (list -1 'SP))
           )
          (compile-pop)
          (rec-comp-h op (cdr exp-lst)) 
          )]))
    (cond
      [(empty? exp-lst)
       (cond [(equal? op 'land) (compile-push #t)]
             [(equal? op 'lor) (compile-push #f)])]
                        
      [(empty? (cdr exp-lst))
       (compile-bexp (car exp-lst))]
      [true
       (append
        (compile-bexp (car exp-lst))
        (rec-comp-h op (cdr exp-lst))
        )]))
    
  (match bexp
    [`(=, aexp1, aexp2) (compile-op 'equal aexp1 aexp2)]
    [`(>, aexp1, aexp2) (compile-op 'gt aexp1 aexp2)]
    [`(<, aexp1, aexp2) (compile-op 'lt aexp1 aexp2)]
    [`(>=, aexp1, aexp2) (compile-op 'ge aexp1 aexp2)]
    [`(<=, aexp1, aexp2) (compile-op 'le aexp1 aexp2)]
    [`(!=, aexp1, aexp2) (compile-op 'not-equal aexp1 aexp2)]
    [`(not, bexp1) (append
                    (compile-bexp bexp1) 
                    (list
                     (list 'lnot (list -1 'SP) (list -1 'SP))
                     ))]
    [`(and,bexp1 ...) (recursive-compile 'land (cdr bexp))]
    [`(or,bexp1 ...) (recursive-compile 'lor (cdr bexp))]
    ['true (compile-push #t)]
    ['false (compile-push #f)]
    ))
(define (compile-aexp aexp)
  (match aexp
    [`(+, aexp1, aexp2) (compile-op 'add aexp1 aexp2)]
    [`(*, aexp1, aexp2) (compile-op 'mul aexp1 aexp2)]
    [`(-, aexp1, aexp2) (compile-op 'sub aexp1 aexp2)]
    [`(div, aexp1, aexp2) (compile-op 'div aexp1 aexp2)]
    [`(mod, aexp1, aexp2) (compile-op 'mod aexp1 aexp2)]
    [x (compile-push x)] ;; whether we have a num or id doesn't matter, A-primpl is left to interpret this
    ))

(define (compile-stmt stmt)
  (define (compile-iif bexp stmt1 stmt2 LABEL0 LABEL1 LABEL2)
    (append
     (compile-bexp bexp)
     (list
      (list 'branch (list -1 'SP) LABEL0) ;; branch with current stack value pushed by previous part
      (list 'jump LABEL1)
      (list 'label LABEL0)
      )
     (compile-pop) 
     (compile-stmt stmt1)
     (list
      (list 'jump LABEL2)
      (list 'label LABEL1)
      )
     (compile-pop) 
     (compile-stmt stmt2)
     (list
      (list 'label LABEL2)
      )
      ))
  (define (compile-while seq-lst bexp LABEL0 LABEL1 LABEL2)
    (append
     (list
      (list 'label LABEL0)
      )
     (compile-bexp bexp)
     (list
      (list 'branch (list -1 'SP) LABEL1) 
      (list 'jump LABEL2)
      (list 'label LABEL1)
      )
     (compile-pop) 
     (compile-stmt (cons 'seq seq-lst))
     (list
      (list 'jump LABEL0)
      (list 'label LABEL2)
      )
     (compile-pop)) 
       )
  (match stmt
    [`(print, exp)
     (cond
       [(string? exp)(list (list 'print-string exp))]
       [true (append
              (compile-aexp exp) 
              (list
               (list 'print-val (list -1 'SP))
               )
              (compile-pop)
              )])]
    [`(set, id, aexp)
     (append
      (compile-aexp aexp) 
      (list
       (list 'move id (list -1 'SP)) 
       )
      (compile-pop)
       )]
    [(cons 'seq (cons stmt1 lst)) ;; lst is either (cons stmt empty), empty, or other
     (cond [(empty? lst) (compile-stmt stmt1)]
           [true (append                  
                   (compile-stmt stmt1)
                   (compile-stmt (cons 'seq lst)))])]
    [`(iif, bexp, stmt1, stmt2)
     (compile-iif bexp stmt1 stmt2 (gensym "LABEL0")(gensym "LABEL1")(gensym "LABEL2"))]
    [`(skip) '()]
    [(cons 'while (cons bexp seq-lst))
     (compile-while seq-lst bexp (gensym "LABEL0") (gensym "LABEL1") (gensym "LABEL2"))]))

;; Parameters: Program - list 
(define (modify-vars vars)
  (define (conversion var)
    (list 'data (car var)
          (cadr var)))
  (map conversion vars))

;; main function body
(define (compile-simpl program)
  (define (compile-simpl-h instr)
    (match instr
      ['() '()]
      [(cons 'vars (cons data code))
       (append (compile-stmt (cons 'seq code))
               (modify-vars data))] 
      [stmt (compile-stmt stmt)]))
  (define instr-body (compile-simpl-h program))
  (define stack-pointer-loc (+ 3(length instr-body)))
  (reverse (cons (list 'data 'SP stack-pointer-loc) (reverse instr-body))))