meta:
  id: nested_record_write
types:
  entry:
    seq:
      - id: tag
        contents: 'ENTR'
      - id: value
        type: u2le
seq:
  - id: header
    contents: 'ROOT'
  - id: version
    type: u1
  - id: entry
    type: entry
