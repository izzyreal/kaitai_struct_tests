meta:
  id: repeat_expr_user_write
  endian: le
types:
  entry:
    seq:
      - id: value
        type: u2
      - id: code
        contents: 'OK'
seq:
  - id: count
    type: u1
  - id: entries
    type: entry
    repeat: expr
    repeat-expr: count
