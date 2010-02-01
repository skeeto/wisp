;;; Unit testing functions

(defun assert-exit (val)
  (if (not val)
      (exit -1)))
