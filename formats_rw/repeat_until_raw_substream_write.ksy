meta:
  id: repeat_until_raw_substream_write
  endian: le
seq:
  - id: entry_size
    type: u1
  - id: entries
    type: entry
    size: entry_size
    repeat: until
    repeat-until: _.is_last == 1
types:
  entry:
    seq:
      - id: is_last
        type: u1
      - id: value
        type: u1
