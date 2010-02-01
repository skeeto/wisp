;;; Core definitions for Wisp

;; Set up require
(defun apply (f lst)
  (if (not (listp lst))
      (throw 'wrong-type-argument lst)
    (eval (cons f lst))))

(defun concat (str &rest strs)
  (if (nullp strs)
      str
    (concat2 str (apply concat strs))))

(defun require (lib)
  (load (concat wisproot "/wisplib/" (symbol-name lib) ".wisp")))

;; Load up other default libraries
(require 'list)
(require 'math)

(defmacro setq (var val)
  (list 'set (list 'quote var) val))

(defun equal (a b)
  (or (eql a b)
      (and (listp a)
	   (listp b)
	   (equal (car a) (car b))
	   (equal (cdr a) (cdr b)))))

(defun make-symbol (str)
  (if (not (stringp str))
      (throw 'wrong-type-argument str)
    (eval-string (concat "(quote " str ")"))))

(defun vconcat (vec &rest vecs)
  (if (nullp vecs)
      vec
    (vconcat2 vec (apply vconcat vecs))))

(defun vsplice (vmain start end vins)
  (vconcat (vsub vmain 0 (1- start )) vins (vsub vmain (1+ end))))

(defun vfunc (vec &rest args)
  (let ((narg (length args)))
    (cond
     ((>= 3 narg) (throw 'wrong-number-of-arguments args))
     ((= 0 narg) vec)
     ; vget
     ((and (= 1 narg) (listp (car args)))
      (vsub vec (caar args) (cadar args)))
     ((and (= 1 narg)) (vget vec (car args)))
     ; vset
     ((listp (car args)) (vsplice vec (caar args) (cadar args) (cadr args)))
     (t (vset vec (car args) (cadr args))))))
