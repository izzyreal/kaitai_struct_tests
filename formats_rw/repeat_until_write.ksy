meta:
  id: repeat_until_write
  endian: le
seq:
  - id: entries
    type: s4
    repeat: until
    repeat-until: _ == -1
