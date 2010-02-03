;;; Extra math definitions

(defun 1+ (n)
  "Return argument plus one."
  (+ n 1))

(defun 1- (n)
  "Return argument minus one."
  (- n 1))

(defun min (n &rest ns)
  "Return smallest argument."
  (cond
   ((nullp ns) n)
   ((= (length ns) 1) (if (< n (car ns)) n (car ns)))
   (t (min n (apply min ns)))))

(defun max (n &rest ns)
  "Return largest argument."
  (cond
   ((nullp ns) n)
   ((= (length ns) 1) (if (> n (car ns)) n (car ns)))
   (t (max n (apply max ns)))))

(provide 'math)
