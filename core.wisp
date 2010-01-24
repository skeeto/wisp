(defun 1+ (n)
  (+ 1 n))

(defun length (lst)
  (if (nullp lst)
      0
    (1+ (length (cdr lst)))))

(defmacro setq (var val)
  (list (quote set) (list (quote quote) var) val))
