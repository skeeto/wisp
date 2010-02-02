;;; Core definitions for Wisp

;; Set up require

(defun concat (str &rest strs)
  "Concatenate any number of strings."
  (if (nullp strs)
      str
    (concat2 str (apply concat strs))))

(defun require (lib)
  "Bring library functions into current environment."
  (load (concat wisproot "/wisplib/" (symbol-name lib) ".wisp")))

;; Load up other default libraries
(require 'list)
(require 'math)
(require 'vector)

(defmacro setq (var val)
  "Automatically quote the first argument for set."
  (list 'set (list 'quote var) val))

(defun equal (a b)
  "Return t if both arguments have similar structure and contents."
  (or (eql a b)
      (and (listp a)
	   (listp b)
	   (equal (car a) (car b))
	   (equal (cdr a) (cdr b)))))

(defun make-symbol (str)
  "Make symbol from string."
  (if (not (stringp str))
      (throw 'wrong-type-argument str)
    (read-string str)))

(defun doc-string (f)
  "Return documentation string for object."
  (if (symbolp f)
      (setq f (value f)))
  (if (listp f)
      (if (stringp (third f))
	  (third f))
    (cdoc-string f)))
