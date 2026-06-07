meta:
  id: vlq_base128_be_write
  bit-endian: be

seq:
  - id: groups
    type: group
    repeat: until
    repeat-until: not _.has_next

types:
  group:
    seq:
      - id: has_next
        type: b1
      - id: value
        type: b7

instances:
  last:
    value: groups.size - 1
  value:
    value: |
      (groups[last].value
      + (last >= 1 ? (groups[last - 1].value << 7) : 0)
      + (last >= 2 ? (groups[last - 2].value << 14) : 0)
      + (last >= 3 ? (groups[last - 3].value << 21) : 0)
      + (last >= 4 ? (groups[last - 4].value << 28) : 0)
      + (last >= 5 ? (groups[last - 5].value << 35) : 0)
      + (last >= 6 ? (groups[last - 6].value << 42) : 0)
      + (last >= 7 ? (groups[last - 7].value << 49) : 0)).as<u8>
