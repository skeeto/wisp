;;; Test equality functions

(require 'test)

;; eq
(assert-exit (eq 'a 'a))
(assert-exit (eq 'b 'b))
(assert-exit (not (eq 'a 'b)))
(assert-exit (not (eq 10 10)))

(let ((var 10))
  (assert-exit (eq var var)))

;; eql
(assert-exit (eql 100 100))
(assert-exit (eql 10 10))
(assert-exit (eql 10.1 10.1))

(assert-exit (not (eql 10 100)))

;; equal
(assert-exit (equal '(a b c) '(a b c)))
(assert-exit (equal '(a (b) c) '(a (b) c)))
(assert-exit (equal '(a (b 10) c) '(a (b 10) c)))
(assert-exit (equal '(a (b 10) nil c) '(a (b 10) nil c)))
(assert-exit (equal '(a (b 10) (10.7 nil) c) '(a (b 10) (10.7 nil) c)))

(assert-exit (not (equal '(a (b 10) (10.6 nil) c) '(a (b 10) (10.7 nil) c))))
(assert-exit (not (equal '(a (b 10) (10.6 nil) c) '(a (10) (10.7 nil) c))))
