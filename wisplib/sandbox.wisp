;;; sandbox.wisp - removes "dangerous" functions for executing untrusted code

;; malicious code could make this large and crash the interpreter
(setq max-eval-depth nil)
