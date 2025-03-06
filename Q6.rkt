#lang racket
(require test-engine/racket-tests)

(struct bin (op fst snd) #:transparent)      
(struct fun (param body) #:transparent)      
(struct app (fn arg) #:transparent)          
(struct seq (fst snd) #:transparent)         
(struct set (var newval) #:transparent)      

(struct sub (name val) #:transparent)
(struct closure (var body env) #:transparent)
(struct result (val newstore) #:transparent)

;; lookup: symbol store -> value
(define (lookup var store)
  (cond
    [(empty? store) (error 'interp "unbound variable ~a" var)]
    [(symbol=? var (sub-name (first store)))
     (sub-val (first store))]
    [else (lookup var (rest store))]))

;; update-store: symbol value store -> new store
(define (update-store var newval store)
  (cond
    [(empty? store) (error 'set! "unbound variable ~a" var)]
    [(symbol=? var (sub-name (first store)))
     (cons (sub var newval) (rest store))]
    [else (cons (first store)
                (update-store var newval (rest store)))]))

;; op-trans: symbol -> (number number -> number)
(define (op-trans op)
  (match op
    ['+ +]
    ['* *]
    ['- -]
    ['/ /]))

;; parse: s-expression -> AST
(define (parse sx)
  (match sx
    [`(with ((,nm ,nmd)) ,bdy)
     (app (fun nm (parse bdy)) (parse nmd))]
    [`(seq ,x ,y) (seq (parse x) (parse y))]
    [`(set ,v ,e) (set v (parse e))]
    [`(+ ,x ,y) (bin '+ (parse x) (parse y))]
    [`(* ,x ,y) (bin '* (parse x) (parse y))]
    [`(- ,x ,y) (bin '- (parse x) (parse y))]
    [`(/ ,x ,y) (bin '/ (parse x) (parse y))]
    [`(fun (,x) ,bdy) (fun x (parse bdy))]
    [`(,f ,x) (app (parse f) (parse x))]
    [x x]))

;; Both the environment (env) and the store are lists of substitutions (sub).
(define (interp ast env store)
  (match ast
    [(? number? n)
     (result n store)]
    [(? symbol? v)
     (result (lookup v store) store)]
    [(fun v bdy)
     (result (closure v bdy env) store)]
    [(app fun-exp arg-exp)
     (define r-fun (interp fun-exp env store))
     (match (result-val r-fun)
       [(closure v bdy cl-env)
        (define r-arg (interp arg-exp env (result-newstore r-fun)))
        (let* ([new-binding (sub v (result-val r-arg))]
               [new-store (cons new-binding (result-newstore r-arg))]
               [extended-env (cons new-binding cl-env)])
          (interp bdy extended-env new-store))])]
    [(bin op x y)
     (define r1 (interp x env store))
     (define r2 (interp y env (result-newstore r1)))
     (result ((op-trans op)
              (result-val r1)
              (result-val r2))
             (result-newstore r2))]
    [(seq first-expr second-expr)
     (define r1 (interp first-expr env store))
     (interp second-expr env (result-newstore r1))]
    [(set v new-expr)
     (define r (interp new-expr env store))
     (define updated-store (update-store v (result-val r)
                                         (result-newstore r)))
     (result (result-val r) updated-store)]
    
    [else (error 'interp "unknown expression ~a" ast)]))

(test)
