meta:
  id: mpc60all_v3_probe

types:
  song_step:
    seq:
      - id: sequence_number
        type: u1
      - id: repeats
        type: u1

  song:
    seq:
      - id: step_count
        type: u1
      - id: song_number
        type: u1
        if: step_count != 0
      - id: end_status
        type: u1
        if: step_count != 0
      - id: loop_back_step_number
        type: u1
        if: step_count != 0
      - id: song_name
        type: str
        encoding: ASCII
        size: 16
        if: step_count != 0
      - size: 5
        if: step_count != 0
      - id: steps
        type: song_step
        repeat: expr
        repeat-expr: step_count
        if: step_count != 0

seq:
  - id: file_id
    contents: [0x04]
  - id: file_format_version
    contents: [0x03]
  - id: total_number_of_bytes_in_all_sequences
    type: u4le
  - id: sequence_bodies
    size: total_number_of_bytes_in_all_sequences
  - id: songs
    type: song
    repeat: until
    repeat-until: _.step_count == 0
