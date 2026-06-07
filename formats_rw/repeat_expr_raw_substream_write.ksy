meta:
  id: repeat_expr_raw_substream_write
  endian: le
seq:
  - id: count
    type: u1
  - id: entry_size
    type: u1
  - id: entries
    type: entry
    repeat: expr
    repeat-expr: count
    size: entry_size
types:
  entry:
    seq:
      - id: left
        type: u1
      - id: right
        type: u1
