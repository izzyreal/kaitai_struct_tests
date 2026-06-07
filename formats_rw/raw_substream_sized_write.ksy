meta:
  id: raw_substream_sized_write
  endian: le
seq:
  - id: payload_len
    type: u1
  - id: payload
    size: payload_len
    type: payload_type
types:
  payload_type:
    seq:
      - id: left
        type: u1
      - id: right
        type: u1
