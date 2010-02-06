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

(defun abs (x)
  "Return absolute value of number."
  (if (> x 0) x
    (- x)))

(defun nth-root (b n)
  "Return nth root of b."
  (if (< b 0)
      (throw 'domain-error b)
    (let ((x (/ b 2.0)))
      (while (> (abs (- (expt x n) b)) 0.00000001)
	(setq x (* (/ 1.0 n) (+ (* (1- n) x) (/ b (expt x (1- n)))))))
      x)))

(defun sqrt (b)
  "Square root of a number."
  (nth-root b 2))

(defun expt (b p)
  "Return the exponential."
  (cond
   ((< p 0) (/ 1 (expt b (- p))))
   ((= p 0) 1)
   ((< p 1) (nth-root b (/ 1 p)))
   (t (* b (expt b (1- p))))))

(provide 'math)
