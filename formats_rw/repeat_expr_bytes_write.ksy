meta:
  id: repeat_expr_bytes_write
seq:
  - id: count
    type: u1
  - id: names
    type: str
    size: 4
    encoding: ASCII
    repeat: expr
    repeat-expr: count
  - id: payloads
    size: 3
    repeat: expr
    repeat-expr: count
