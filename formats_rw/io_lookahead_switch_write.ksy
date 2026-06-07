meta:
  id: io_lookahead_switch_write
  endian: le
seq:
  - id: count
    type: u1
  - id: events
    type: 'event(_index == 0 ? 0xff : events[_index - 1].actual_kind)'
    repeat: expr
    repeat-expr: count
types:
  event:
    params:
      - id: prev_kind
        type: u1
    seq:
      - id: event_header
        type: u1
        if: not using_prev
      - id: body
        type:
          switch-on: actual_kind
          cases:
            0x81: body_one
            0x82: body_two
    instances:
      status_byte_lookahead:
        pos: _io.pos
        type: u1
      using_prev:
        value: status_byte_lookahead < 0x80
      actual_kind:
        value: 'using_prev ? prev_kind : event_header'
  body_one:
    seq:
      - id: value
        type: u1
  body_two:
    seq:
      - id: value
        type: u2
