meta:
  id: mpc2000xlall_write
  file-extension: all
  bit-endian: le

types:
  location:
    seq:
      - id: bar
        type: u2le
      - id: beat
        type: u1
      - id: clock
        type: u1

  song_global:
    seq:
      - id: default_song_name
        type: str
        encoding: ASCII
        size: 16
      - id: ignore_tempo_change_events_in_sequence
        type: b1
      - type: b7

  track_status:
    seq:
      - id: unused_or_used
        type: b1
      - id: off_or_on
        type: b1
      - id: transmit_program_changes
        type: b1
      - type: b5

  defaults:
    seq:
      - id: sequence_name
        type: str
        encoding: ASCII
        size: 16
      - size: 6
      - id: tempo
        type: u2le
      - id: numerator
        type: u1
      - id: denominator
        type: u1
      - id: bar_count
        type: u2le
      - id: tick_count
        type: u2le
      - id: unknown1
        type: u4le
        repeat: expr
        repeat-expr: 4
      - id: unknown2
        size: 74
      - id: device_names
        type: str
        encoding: ASCII
        size: 8
        repeat: expr
        repeat-expr: 33
      - id: track_names
        type: str
        encoding: ASCII
        size: 16
        repeat: expr
        repeat-expr: 64
      - id: devices
        type: u1
        repeat: expr
        repeat-expr: 64
      - id: buses
        type: u1
        repeat: expr
        repeat-expr: 64
      - id: programs
        type: u1
        repeat: expr
        repeat-expr: 64
      - id: track_velocities
        type: u1
        repeat: expr
        repeat-expr: 64
      - id: track_statuses
        type: track_status
        repeat: expr
        repeat-expr: 64
      - size: 64

  sequencer:
    seq:
      - id: active_sequence
        type: u1
      - size: 1
      - id: active_track
        type: u1
      - size: 1
      - id: master_tempo
        type: u2le
      - id: tempo_source_is_sequence
        type: b1
      - type: b7
      - id: timing_correct
        type: u1
      - id: time_display_style
        type: b1
      - type: b7
      - id: second_sequence_enabled
        type: b1
      - type: b7
      - id: sequence_sequence_index
        type: u1

  count:
    seq:
      - id: enabled
        type: b1
      - type: b7
      - id: count_in_mode
        type: u1
      - id: click_volume
        type: u1
      - id: rate
        type: u1
      - id: enabled_in_play
        type: b1
      - type: b7
      - id: enabled_in_rec
        type: b1
      - type: b7
      - id: click_output
        type: u1
      - id: wait_for_key
        type: b1
      - type: b7
      - id: sound_source
        type: u1
      - id: accent_pad_index
        type: u1
      - id: normal_pad_index
        type: u1
      - id: accent_velo
        type: u1
      - id: normal_velo
        type: u1

  midi_output:
    seq:
      - id: soft_thru_mode
        type: u1

  midi_input:
    seq:
      - id: receive_channel
        type: u1
      - id: sustain_pedal_to_duration
        type: b1
      - type: b7
      - id: filter_enabled
        type: b1
      - type: b7
      - id: filter_type
        type: u1
      - id: multi_rec_enabled
        type: b1
      - type: b7
      - id: multi_rec_destination_tracks
        type: u1
        repeat: expr
        repeat-expr: 34
      - id: note_pass_enabled
        type: b1
      - type: b7
      - id: pitch_bend_pass_enabled
        type: b1
      - type: b7
      - id: pgm_change_pass_enabled
        type: b1
      - type: b7
      - id: ch_pressure_pass_enabled
        type: b1
      - type: b7
      - id: poly_pressure_pass_enabled
        type: b1
      - type: b7
      - id: exclusive_pass_enabled
        type: b1
      - type: b7
      - id: cc_pass_enabled
        type: b1
        repeat: expr
        repeat-expr: 128

  midi_sync:
    seq:
      - id: in_mode
        type: u1
      - id: out_mode
        type: u1
      - id: shift_early
        type: u1
      - id: send_mmc_enabled
        type: b1
      - type: b7
      - id: frame_rate
        type: u1
      - id: input
        type: u1
      - id: output
        type: u1

  midi_switch:
    seq:
      - id: controller
        type: u1
        doc: 0xFF is used for an unassigned footswitch controller.
      - id: function
        type: u1

  misc:
    seq:
      - id: tap_averaging
        type: u1
      - id: midi_sync_in_receive_mmc_enabled
        type: b1
      - id: midi_switch
        type: midi_switch
        repeat: expr
        repeat-expr: 4

  step_edit_options:
    seq:
      - id: auto_step_increment
        type: b1
      - id: duration_of_recorded_notes
        type: u1
      - id: tc_value_percentage
        type: u1

  sequence_meta:
    seq:
      - id: name
        type: str
        encoding: ASCII
        size: 16
      - id: is_used
        type: u2le

  song_step:
    seq:
      - id: sequence_index
        type: u1
      - id: repeat_count
        type: u1

  song:
    seq:
      - id: name
        type: str
        encoding: ASCII
        size: 16
      - id: steps
        type: song_step
        repeat: expr
        repeat-expr: 250
      - size: 2
      - id: is_used
        type: b1
      - id: loop_first_step
        type: u1
      - id: loop_last_step
        type: u1
      - id: is_loop_enabled
        type: b1
      - size: 6

  bar:
    seq:
      - id: ticks_per_beat
        type: u1
      - id: last_tick
        type: b24
    params:
      - id: idx
        type: s4

  tracks:
    seq:
      - id: names
        type: str
        encoding: ASCII
        size: 16
        repeat: expr
        repeat-expr: 64
      - id: device
        type: u1
        repeat: expr
        repeat-expr: 64
      - id: bus
        type: u1
        repeat: expr
        repeat-expr: 64
      - id: program_change
        type: u1
        repeat: expr
        repeat-expr: 64
      - id: velocity_ratio
        type: u1
        repeat: expr
        repeat-expr: 64
      - id: status
        type: track_status
        repeat: expr
        repeat-expr: 64
      - id: unknown
        size: 64

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

  pitch_bend_event:
    seq:
      - id: amount_bits_1
        type: b8
      - id: amount_bits_2
        type: b8
      - size: 1

  control_change_event:
    seq:
      - id: controller
        type: u1
      - id: value
        type: u1
        valid:
          min: 0
          max: 127
      - size: 1

  program_change_event:
    seq:
      - id: program
        type: u1
        valid:
          min: 0
          max: 127
      - size: 2

  ch_pressure_event:
    seq:
      - id: pressure
        type: u1
        valid:
          min: 0
          max: 127
      - size: 2

  poly_pressure_event:
    seq:
      - id: note
        type: u1
        valid:
          min: 0
          max: 127
      - id: pressure
        type: u1
        valid:
          min: 0
          max: 127
      - size: 1

  mixer_event:
    seq:
      - size: 3
      - id: param
        type: u1
      - id: pad_index
        type: u1
      - id: value
        type: u1
        valid:
          min: 0
          max: 100
      - size: 2

  exclusive_event:
    seq:
      - size: 3
      - id: bytes
        size: 2
      - id: mixer
        type: mixer_event
        if: bytes == [0xF0, 0x47]
      - size: 14

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
      - id: pitch_bend
        type: pitch_bend_event
        if: tick < 0xFFFFF and id == 0xE0
      - id: control_change
        type: control_change_event
        if: tick < 0xFFFFF and id == 0xB0
      - id: program_change
        type: program_change_event
        if: tick < 0xFFFFF and id == 0xC0
      - id: ch_pressure
        type: ch_pressure_event
        if: tick < 0xFFFFF and id == 0xD0
      - id: poly_pressure
        type: poly_pressure_event
        if: tick < 0xFFFFF and id == 0xA0
      - id: exclusive
        type: exclusive_event
        if: tick < 0xFFFFF and id == 0xF0
      - id: terminator
        if: tick >= 0xFFFFF
        size: 5

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
      - id: tracks
        type: tracks
      - size: 3587
      - id: bars
        type: bar(_index)
        repeat: expr
        repeat-expr: bar_count
      - size: 4 * (999 - bar_count)
      - size: 865
      - id: events
        type: event
        repeat: until
        repeat-until: _.tick == 0xFFFFF

seq:
  - id: magic
    contents: [MPC2KXL ALL 1.00]
  - id: defaults
    type: defaults
  - id: sequencer
    type: sequencer
  - size: 10
  - id: count
    type: count
  - id: midi_output
    type: midi_output
  - id: midi_input
    type: midi_input
  - id: midi_sync
    type: midi_sync
  - id: song_global
    type: song_global
  - size: 5
  - id: locations
    type: location
    repeat: expr
    repeat-expr: 9
  - id: misc
    type: misc
  - size: 3
  - id: step_edit_options
    type: step_edit_options
  - id: prog_change_to_seq
    type: b1
  - size: 78
  - id: sequences_metas
    type: sequence_meta
    repeat: expr
    repeat-expr: 99
  - id: songs
    type: song
    repeat: expr
    repeat-expr: 20
  - id: sequences
    type: sequence
    repeat: eos
