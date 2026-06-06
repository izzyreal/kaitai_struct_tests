meta:
  id: repeat_expr_write
seq:
  - id: count
    type: u1
  - id: values
    type: u1
    repeat: expr
    repeat-expr: count
