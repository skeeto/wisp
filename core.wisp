;;; Core definitions for Wisp

;; Set up require

(defun concat (str &rest strs)
  (if (nullp strs)
      str
    (concat2 str (apply concat strs))))

(defun require (lib)
  (load (concat wisproot "/wisplib/" (symbol-name lib) ".wisp")))

;; Load up other default libraries
(require 'list)
(require 'math)
(require 'vector)

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
