meta:
  id: fixed_record_write
  endian: le
seq:
  - id: magic
    contents: 'KSTR'
  - id: version
    type: u2
  - id: kind
    type: u1
    enum: record_kind
  - id: name
    type: str
    size: 4
    encoding: ASCII
  - id: payload
    size: 3
enums:
  record_kind:
    1: alpha
    2: beta
