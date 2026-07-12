meta:
  id: mpc60seq_v2_probe
  bit-endian: le

enums:
  off_on:
    0: off
    1: on

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
      - id: frames
        type: u1
      - id: seconds
        type: u1
      - id: minutes
        type: u1
      - id: hours
        type: u1

  sequence_header:
    seq:
      - id: sequence_number
        type: u1
      - id: sequence_length_in_bytes
        type: u3le
      - id: offset_from_bottom_of_sequence_to_sequence_start
        type: u3le
      - id: sequence_name
        type: str
        encoding: ASCII
        size: 16
      - id: loop_to_bar
        type: b1
        enum: off_on
      - type: b7
      - id: loop_to_bar_number
        type: u2le
      - id: number_of_bars
        type: u2le
      - id: length_in_ticks
        type: u4le
      - id: tempo
        type: u2le
      - id: smpte_offset
        type: smpte_offset
      - id: stereo_mix
        type: u1
        repeat: expr
        repeat-expr: 32
      - id: stereo_pan
        type: u1
        repeat: expr
        repeat-expr: 32
      - id: echo_mix
        type: u1
        repeat: expr
        repeat-expr: 32
      - id: drum_tuning
        type: u2le
        repeat: expr
        repeat-expr: 32
      - id: last_active_user_track
        type: u1
      - id: number_of_tempo_changes
        type: u1
      - id: number_of_active_track_headers
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

  bar_event:
    seq:
      - id: bar_number1
        type: u1
      - id: bar_number2
        type: u1
      - id: numerator
        type: u1
      - id: denominator
        type: u1

  note_event:
    seq:
      - id: note_number
        type: u1
      - id: velocity
        type: u1
      - id: note_variation_value
        type: u1
      - id: duration_byte_1
        type: u1
      - id: duration_byte_2
        type: u1

  delta_time_event:
    seq:
      - id: delta_time
        type: u2le

  event:
    seq:
      - id: parsed_status
        type: u1
        if: is_first_event
      - id: track_number
        type: u1
        if: status != 0xA8 and status != 0x88 and status != 0xFF
      - id: event_body
        type:
          switch-on: status
          cases:
            0x88: delta_time_event
            0x98: note_event
            0xA8: bar_event
      - id: parsed_next_status
        type: u1
        if: remaining_byte_count > 2
    params:
      - id: is_first_event
        type: b1
      - id: preparsed_status
        type: u1
      - id: remaining_byte_count
        type: s4
    instances:
      status:
        value: 'preparsed_status == 0xFF ? parsed_status : preparsed_status'
      next_status:
        value: parsed_next_status

seq:
  - id: file_id
    contents: [0x03]
  - id: file_format_version
    contents: [0x02]
  - id: sequence_header
    type: sequence_header
  - id: track_headers
    type: track_header
    repeat: expr
    repeat-expr: sequence_header.number_of_active_track_headers
  - id: tempo_changes
    type: tempo_change
    repeat: expr
    repeat-expr: sequence_header.number_of_tempo_changes
  - id: events
    type: 'event(
      _index == 0,
      _index > 0 ? events[_index - 1].next_status : 0xFF,
      sequence_header.sequence_length_in_bytes.value - (_io.pos - events_start))'
    repeat: until
    repeat-until: sequence_header.sequence_length_in_bytes.value - (_io.pos - events_start) == 0

instances:
  events_start:
    value: _io.pos
