# expr_bytes_to_s_type.ksy: /instances/bad/value:
# 	error: to_s: argument #0: expected string literal, got IntNum(123)
#
meta:
  id: expr_bytes_to_s_type
seq:
  - id: buf
    size: 5
instances:
  bad:
    value: |
      buf.to_s(123)
