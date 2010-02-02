;;; Small example wisp functions

(defun fib (n)
  "Return nth Fibonacci number."
  (if (<= n 2) 1
    (+ (fib (- n 1)) (fib (- n 2)))))
