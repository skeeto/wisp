;;; Unit testing functions

(defun assert-exit (val)
  "If argument is nil, exit interpreter with error."
  (if (not val)
      (exit -1)))

(provide 'test)
