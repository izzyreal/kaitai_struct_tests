meta:
  id: params_stateful_switch_write
  endian: le
seq:
  - id: count
    type: u1
  - id: events
    type: 'event(_index == 0 ? 0xff : events[_index - 1].next_kind)'
    repeat: expr
    repeat-expr: count
types:
  event:
    params:
      - id: prev_kind
        type: u1
    seq:
      - id: use_prev
        type: u1
      - id: kind
        type: u1
        if: use_prev == 0
      - id: body
        type:
          switch-on: actual_kind
          cases:
            1: body_one
            2: body_two
    instances:
      actual_kind:
        value: 'use_prev == 1 ? prev_kind : kind'
      next_kind:
        value: actual_kind
  body_one:
    seq:
      - id: value
        type: u1
  body_two:
    seq:
      - id: value
        type: u2
