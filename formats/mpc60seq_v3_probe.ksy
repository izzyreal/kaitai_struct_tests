meta:
  id: mpc60seq_v3_probe

seq:
  - id: file_id
    contents: [0x03]
  - id: file_format_version
    contents: [0x03]
  - id: sequence_body
    size-eos: true
