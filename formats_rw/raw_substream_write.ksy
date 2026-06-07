meta:
  id: raw_substream_write
  endian: le
seq:
  - id: prefix
    type: u1
  - id: payload
    size: 2
    type: payload_type
types:
  payload_type:
    seq:
      - id: left
        type: u1
      - id: right
        type: u1
