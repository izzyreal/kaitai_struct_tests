meta:
  id: mpc3000all_write
  file-extension: all
  bit-endian: le
  imports:
    - mpc3000seq_write

seq:
  - id: all_file_header
    type: all_file_header
  - id: sequences
    type: sequence
    repeat: until
    repeat-until: (_io.pos - 6) - all_file_header.total_number_of_bytes_in_all_sequences == -1
  - id: songs
    type: song
    repeat: until
    repeat-until: songs[_index - 1].number_of_steps == 0x00

types:
  all_file_header:
    seq:
      - id: file_id
        contents: [0x04]
      - id: file_version
        contents: [0x03]
      - id: total_number_of_bytes_in_all_sequences
        type: u4le

  sequence:
    seq:
      - id: misc_chunks
        type: misc_chunks
      - id: events
        type: mpc3000seq_write::event
        repeat: until
        repeat-until: _.tick == 0xFFFFF

  misc_chunks:
    seq:
      - id: sequence_header
        type: mpc3000seq_write::sequence_header
      - id: smpte_offset
        type: mpc3000seq_write::smpte_offset
      - id: mixer
        type: mpc3000seq_write::mixer
        repeat: expr
        repeat-expr: 64
      - size: 2
      - id: delays
        type: mpc3000seq_write::delays
      - size: 3
      - size: 16
      - id: last_active_track
        type: u1
      - id: number_of_tempo_changes
        type: u1
      - id: number_of_active_track_headers
        type: u1
      - id: track_headers
        type: mpc3000seq_write::track_header
        repeat: expr
        repeat-expr: number_of_active_track_headers
      - id: tempo_changes
        type: mpc3000seq_write::tempo_change
        repeat: expr
        repeat-expr: number_of_tempo_changes
    instances:
      position_after_tempo_changes:
        value: _io.pos

  song:
    seq:
      - id: number_of_steps
        type: u1
      - id: song_body
        type: song_body
        if: number_of_steps != 0x00 and number_of_steps != 0xFF

    types:
      song_body:
        seq:
          - id: song_number
            type: u1
          - id: end_status
            type: u1
          - id: loop_back_step_number
            type: u1
          - id: song_name
            type: str
            encoding: ASCII
            terminator: 0x20
            size: 16
          - id: smpte_offset
            type: mpc3000seq_write::smpte_offset
          - id: steps
            type: step
            repeat: expr
            repeat-expr: _parent.number_of_steps

        types:
          step:
            seq:
              - id: sequence_number
                type: u1
              - id: repetition_count
                type: u1
