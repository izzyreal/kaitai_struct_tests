meta:
  id: mpc3000seq_write
  file-extension: seq
  bit-endian: le

types:
  u3le:
    seq:
      - id: b12
        type: u2le
      - id: b3
        type: u1
    instances:
      value:
        value: 'b12 | (b3 << 16)'

  smpte_offset:
    seq:
      - id: hundredth_frames
        type: u1
        valid:
          min: 0
          max: 99
      - id: frames
        type: u1
        valid:
          min: 0
          max: 29
      - id: seconds
        type: u1
        valid:
          min: 0
          max: 59
      - id: minutes
        type: u1
        valid:
          min: 0
          max: 59
      - id: hours
        type: u1
        valid:
          min: 0
          max: 23

  sequence_header:
    seq:
      - id: sequence_number
        type: u1
      - id: sequence_length_in_bytes
        type: u3le
      - size: 5
      - id: sequence_name
        type: str
        encoding: ASCII
        size: 16
      - size: 1
      - id: loop_to_bar
        type: b1
      - type: b7
      - id: loop_to_bar_number
        type: u2le
      - id: number_of_bars
        type: u2le
      - id: length_in_ticks
        type: u4le
      - id: tempo
        type: u2le

  mixer:
    seq:
      - id: stereo_mix
        type: u1
        valid:
          min: 0
          max: 100
      - id: stereo_pan
        type: u1
        valid:
          min: 0
          max: 100
      - id: individual_out_mix
        type: u1
      - id: individual_out
        type: b7
      - id: follow_stereo
        type: b1

  delays:
    seq:
      - id: volume1
        type: u1
      - id: volume2
        type: u1
      - id: volume3
        type: u1
      - id: pan1
        type: u1
      - id: pan2
        type: u1
      - id: pan3
        type: u1
      - id: time1
        type: u2le
      - id: time2
        type: u2le
      - id: time3
        type: u2le
      - id: feedback1
        type: u1
      - id: feedback2
        type: u1
      - id: feedback3
        type: u1

  track_header:
    seq:
      - id: absolute_recorded_track_number
        type: s1
      - size: 23
        if: absolute_recorded_track_number == -1
      - id: user_track_number
        type: u1
        if: absolute_recorded_track_number != -1
      - id: track_mute
        type: b1
        if: absolute_recorded_track_number != -1
      - id: track_in_use
        type: b1
        if: absolute_recorded_track_number != -1
      - id: drum_track
        type: b1
        if: absolute_recorded_track_number != -1
      - type: b5
        if: absolute_recorded_track_number != -1
      - id: primary_port_channel_assignment
        type: u1
        if: absolute_recorded_track_number != -1
      - id: secondary_port_channel_assignment
        type: s1
        if: absolute_recorded_track_number != -1
      - id: track_name
        type: str
        encoding: ASCII
        size: 16
        if: absolute_recorded_track_number != -1
      - id: track_volume
        type: u1
        valid:
          min: 1
          max: 200
        if: absolute_recorded_track_number != -1
      - id: program_change_number
        type: u1
        if: absolute_recorded_track_number != -1
      - size: 1
        if: absolute_recorded_track_number != -1

  tempo_change:
    seq:
      - id: ticks_from_sequence_start
        type: u4le
      - id: factor1
        type: b12
      - id: factor2
        type: b4

  bar:
    seq:
      - id: ticks_per_beat
        type: u1
      - id: last_tick
        type: b24
    params:
      - id: idx
        type: s4

  note_event:
    seq:
      - id: duration_bits_3
        type: u1
      - id: velocity
        type: b7
      - id: variation_type_bit_1
        type: b1
      - id: variation_value
        type: b7
      - id: variation_type_bit_2
        type: b1

  event:
    seq:
      - id: tick
        type: b20
      - id: duration_bits_1
        type: b4
        if: tick < 0xFFFFF
      - id: track
        type: b6
        if: tick < 0xFFFFF
      - id: duration_bits_2
        type: b2
        if: tick < 0xFFFFF
      - id: id
        type: u1
        if: tick < 0xFFFFF
      - id: note_event
        type: note_event
        if: tick < 0xFFFFF and id <= 0x7F
      - id: terminator
        if: tick >= 0xFFFFF
        size: 5

  sequence_body:
    types:
      start_time:
        seq:
          - id: hours
            type: u1
          - id: minutes
            type: u1
          - id: seconds
            type: u1
          - id: frames
            type: u1
          - id: frame_decimals
            type: u1
    seq:
      - id: is_used
        type: u2le
      - id: index
        type: u1
      - size: 7
      - id: bar_count
        type: u2le
      - id: last_tick
        type: u4le
      - size: 16
      - id: loop_start_bar_index
        type: u2le
      - id: loop_end_bar_index
        type: u2le
      - id: loop_enabled
        type: b1
      - type: b7
      - id: start_time
        type: start_time
      - size: 6
      - id: last_tick2
        type: u4le
      - size: 52
      - id: device_names
        type: str
        encoding: ASCII
        size: 8
        repeat: expr
        repeat-expr: 33
      - id: track_headers
        type: track_header
        repeat: expr
        repeat-expr: 1
      - size: 3587
      - id: bars
        type: bar(_index)
        repeat: expr
        repeat-expr: bar_count
      - type: bar(_index)
        repeat: expr
        repeat-expr: 999 - bar_count
      - size: 865
      - id: events
        type: event
        repeat: eos

  sequence:
    seq:
      - id: name_part_1
        type: str
        terminator: 0xFF
        encoding: ASCII
        size: 8
      - id: name_part_2
        type: str
        terminator: 0xFF
        encoding: ASCII
        size: 8
        if: name_part_1 != ""
      - id: body
        type: sequence_body
        if: name_part_1 != ""

seq:
  - id: file_id
    contents: [0x03]
  - id: file_format_version
    contents: [0x03]
  - id: sequence_header
    type: sequence_header
  - id: smpte_offset
    type: smpte_offset
  - id: mixer
    type: mixer
    repeat: expr
    repeat-expr: 64
  - size: 2
  - id: delays
    type: delays
  - size: 3
  - size: 16
  - id: last_active_track
    type: u1
  - id: number_of_tempo_changes
    type: u1
  - id: number_of_active_track_headers
    type: u1
  - id: track_headers
    type: track_header
    repeat: expr
    repeat-expr: number_of_active_track_headers
  - id: tempo_changes
    type: tempo_change
    repeat: expr
    repeat-expr: number_of_tempo_changes
  - id: sequence
    type: sequence
