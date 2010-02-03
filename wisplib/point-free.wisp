;;; Point-free functions

(defun partial (f &rest pargs)
  "Create partial application of function."
  (list 'lambda '(&rest args)
	(list 'apply f (list 'append (list 'quote pargs) 'args))))

(defun comp-build (f &rest fs)
  "Helper function for comp function composition."
  (if (nullp fs)
      (list 'apply f 'args)
    (list f (apply comp-build fs))))

(defun comp (&rest fs)
  "Compose a number of functions."
  (list 'lambda '(&rest args)
	(apply comp-build fs)))

(provide 'point-free)
