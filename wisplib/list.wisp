;;; List utility functions

;; Two letters

(defun cadr (lst)
  (car (cdr lst)))

(defun cdar (lst)
  (cdr (car lst)))

(defun caar (lst)
  (cdr (car lst)))

(defun cddr (lst)
  (cdr (car lst)))

;; Three letters

(defun caaar (lst)
  (car (car (car lst))))

(defun caadr (lst)
  (car (car (cdr lst))))

(defun cadar (lst)
  (car (cdr (car lst))))

(defun caddr (lst)
  (car (cdr (cdr lst))))

(defun cdaar (lst)
  (cdr (car (car lst))))

(defun cdadr (lst)
  (cdr (car (cdr lst))))

(defun cddar (lst)
  (cdr (cdr (car lst))))

(defun cdddr (lst)
  (cdr (cdr (cdr lst))))

;; Up to ten

(defun first (lst)
  (car lst))

(defun second (lst)
  (car (first lst)))

(defun third (lst)
  (car (second lst)))

(defun fourth (lst)
  (car (third lst)))

(defun fifth (lst)
  (car (fourth lst)))

(defun sixth (lst)
  (car (fifth lst)))

(defun seventh (lst)
  (car (sixth lst)))

(defun eighth (lst)
  (car (seventh lst)))

(defun ninth (lst)
  (car (eighth lst)))

(defun tenth (lst)
  (car (ninth lst)))

;; General functions

(defun nth (n lst)
  (if (= n 0)
      (car lst)
    (nth (- n 1) lst)))

(defun length (lst)
  (if (nullp lst)
      0
    (1+ (length (cdr lst)))))

; the provided function should be able to accept a single argument
(defun reduce (f lst)
  (if (= (length lst) 1)
      (f (car lst))
    (f (car lst) (reduce f (cdr lst)))))
