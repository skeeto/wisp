;;; Set functions

;; Member was already defined so it could be used by require

(defun union (a b)
  "Combine two sets by the union set function."
  (if (nullp a)
      b
    (if (member (car a) b)
	(union (cdr a) b)
      (cons (car a) (union (cdr a) b)))))

(defun adjoin (el lst)
  "Add element to set if it's not already a member of that set."
  (if (member el lst)
      lst
    (cons el lst)))

(defun intersection (a b)
  "Return set containing elements only present in both sets."
  (if (nullp a)
      nil
    (if (member (car a) b)
	(cons (car a) (intersection (cdr a) b))
      (intersection (cdr a) b))))

(provide 'set)
