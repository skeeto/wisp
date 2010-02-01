;;; Runs tests from the Wisp side

;; Stress test
(require 'examples)
(progn
  (fib 20)
  (fib 25)
  (fib 30))
