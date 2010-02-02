;;; Memoization definitions
(require 'hash)

;; this isn't working 100% yet
(defun memoize (f)
  "Install memoize wrapper on given function."
  (setq *table* (make-hash-table 2000))
  (set f (list 'lambda '(&rest vars)
	       (list 'if '(hget *table* vars)
		     '(hget *table* vars)
		     (list 'hput '*table* 'vars
			   (list 'apply (value f) 'vars)))
	       '(hget *table* vars))))
