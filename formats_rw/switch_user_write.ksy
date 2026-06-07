meta:
  id: switch_user_write
  endian: le
seq:
  - id: kind
    type: u1
  - id: body
    type:
      switch-on: kind
      cases:
        1: body_one
        2: body_two
types:
  body_one:
    seq:
      - id: value
        type: u1
  body_two:
    seq:
      - id: value
        type: u2
