meta:
  id: if_write
types:
  payload:
    seq:
      - id: magic
        contents: 'OK'
      - id: value
        type: u1
seq:
  - id: has_label
    type: u1
  - id: has_payload
    type: u1
  - id: label
    type: str
    size: 4
    encoding: ASCII
    if: has_label != 0
  - id: payload
    type: payload
    if: has_payload != 0
