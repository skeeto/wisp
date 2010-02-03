;;; List utility functions

;; Two letters

(defun cadr (lst)
  (car (cdr lst)))

(defun cdar (lst)
  (cdr (car lst)))

(defun caar (lst)
  (car (car lst)))

(defun cddr (lst)
  (cdr (cdr lst)))

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
  (first (cdr lst)))

(defun third (lst)
  (second (cdr lst)))

(defun fourth (lst)
  (third (cdr lst)))

(defun fifth (lst)
  (fourth (cdr lst)))

(defun sixth (lst)
  (fifth (cdr lst)))

(defun seventh (lst)
  (sixth (cdr lst)))

(defun eighth (lst)
  (seventh (cdr lst)))

(defun ninth (lst)
  (eighth (cdr lst)))

(defun tenth (lst)
  (ninth (cdr lst)))

;; General functions

(defun nth (n lst)
  "Return nth element of list."
  (if (= n 0)
      (car lst)
    (nth (- n 1) lst)))

(defun length (lst)
  "Return length of list."
  (if (nullp lst)
      0
    (1+ (length (cdr lst)))))

(defun reduce (f lst)
  "Reduce two-argument function across list."
  (if (= (length lst) 1)
      (f (car lst))
    (f (car lst) (reduce f (cdr lst)))))

(defun append (lst &rest lsts)
  "Concatenate any number of lists."
  (cond
   ((nullp lsts) lst)
   ((= 1 (length lst)) (cons (car lst) (apply append lsts)))
   (t (cons (car lst)
	    (append (cdr lst) (apply append lsts))))))

(provide 'list)
