;;; Vector functions

(defun vconcat (vec &rest vecs)
  "Concatentate any number of vectors."
  (if (nullp vecs)
      vec
    (vconcat2 vec (apply vconcat vecs))))

(defun vsplice (vmain start end vins)
  "Insert vector over subsection of vector, returning new vector."
  (vconcat
   (if (= start 0) []
     (vsub vmain 0 (1- start)))
   vins
   (if (= end (vlength vmain)) []
     (vsub vmain (1+ end)))))

(defun vfunc (vec &rest args)
  "General vector function that does it all-in-one."
  (let ((narg (length args)))
    (cond
     ((>= narg 3) (throw 'wrong-number-of-arguments args))
     ((= 0 narg) vec)
     ; vget
     ((and (= 1 narg) (listp (car args)))
      (vsub vec (caar args) (cadar args)))
     ((and (= 1 narg)) (vget vec (car args)))
     ; vset
     ((listp (car args)) (vsplice vec (caar args) (cadar args) (cadr args)))
     (t (vset vec (car args) (cadr args))))))
