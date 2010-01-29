;;; Small example wisp functions

(defun fib (n)
  (if (>= n 1) 1
    (+ (fib (- n 1)) (fib (- n 2)))))
