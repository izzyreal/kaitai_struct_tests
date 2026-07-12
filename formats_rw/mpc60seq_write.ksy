meta:
  id: mpc60seq_write
  file-extension: seq
  imports:
    - mpc60all_write

seq:
  - id: file_id
    contents: [0x03]
  - id: file_format_version
    type: u1
    enum: seq_wrapper_version
  - id: sequence
    type: mpc60all_write::sequence

enums:
  # Observed status:
  #   - MPC60 SCSI v2.14 emits 0x02
  #   - earlier plain MPC60 v2.12 evidence points to 0x03
  # This appears to track firmware provenance rather than a strict model split.
  #
  # Important limitation:
  # this fixture only tests wrapper-byte emission against the simplified
  # `mpc60all_write::sequence` body used on the serializer branch. Real-world
  # `0x03` files are currently understood to share the MPC3000 v3 body/layout.
  seq_wrapper_version:
    0x02: v2
    0x03: v3
