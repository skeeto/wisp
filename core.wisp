(defun 1+ (n)
  (+ 1 n))

(defun length (lst)
  (if (nullp lst)
      0
    (1+ (length (cdr lst)))))

(defmacro setq (var val)
  (list (quote set) (list (quote quote) var) val))

;; The provided function should be able to accept a single argument
(defun reduce (f lst)
  (if (= (length lst) 1)
      (f (car lst))
    (f (car lst) (reduce f (cdr lst)))))

(defun apply (f lst)
  (if (not (listp lst))
      (throw 'wrong-type-argument lst)
    (eval (cons f lst))))
