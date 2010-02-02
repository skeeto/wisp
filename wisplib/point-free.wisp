;;; Point-free functions

;; Partial application
(defun partial (f &rest pargs)
  (list 'lambda '(&rest args)
	(list 'apply f (list 'append (list 'quote pargs) 'args))))

;; Function compose
(defun comp-build (f &rest fs)
  (if (nullp fs)
      (list 'apply f 'args)
    (list f (apply comp-build fs))))

(defun comp (&rest fs)
  (list 'lambda '(&rest args)
	(apply comp-build fs)))
