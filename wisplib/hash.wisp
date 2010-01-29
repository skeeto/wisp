;;; Hash table definitions

;; this currently uses closed-hashing, but it would be better to
;; change this to open hashing

(defun make-hash-table (size)
  (list (make-vector size nil) size))

(defun hget (ht key)
  (let ((r nil)
	(ind (% (hash key) (cadr ht))))
    (while (vget (car ht) ind)
      (if (equal (car (vget (car ht) ind)) key)
	  (setq r (cdr (vget (car ht) ind))))
      (setq ind (1+ ind)))
    r))

(defun hput (ht key val)
  (let ((ind (% (hash key) (cadr ht))))
    (while (vget (car ht) ind)
      (if (equal (car (vget (car ht) ind)) o)
	  (setq lst nil))
      (setq ind (1+ ind)))
    (vset (car ht) ind (cons key val))))
