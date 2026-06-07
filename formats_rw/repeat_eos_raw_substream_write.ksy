meta:
  id: repeat_eos_raw_substream_write
  endian: le
seq:
  - id: entry_size
    type: u1
  - id: entries
    type: entry
    size: entry_size
    repeat: eos
types:
  entry:
    seq:
      - id: left
        type: u1
      - id: right
        type: u1
