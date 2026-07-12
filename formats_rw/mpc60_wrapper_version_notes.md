## MPC60 Wrapper Version Notes

This fixture set treats the leading wrapper bytes on MPC60-family `.SEQ` / `.ALL`
files as a firmware-specific detail, not a model-specific protocol boundary.

Observed status so far:

- `SEQ` wrapper uses file id `0x03`
- `ALL` wrapper uses file id `0x04`
- `mpc60scsi` firmware `2.14` was freshly confirmed to use wrapper version byte `0x02`
  for both `.SEQ` and `.ALL`
- older plain `mpc60` `2.12` evidence points to wrapper version byte `0x03`
  for both `.SEQ` and `.ALL`
- fresh July 2026 body-level validation showed that real `2.12` `0x03` files,
  when sliced to their actual FAT file sizes, parse cleanly with the existing
  MPC3000 v3 body/layout

So the working interpretation is:

- wrapper version selection is firmware provenance
- the `0x03` family is currently best understood as a shared MPC3000/MPC60
  body/layout family rather than a distinct MPC60-only body

Important limitation of the write fixtures in this directory:

- `mpc60seq_write.ksy` / `mpc60all_write.ksy` currently exercise wrapper-byte
  selection and serializer plumbing
- they do not yet claim that the real `0x03` family body is identical to the
  simplified `mpc60all_write` body used by these smoke tests

Practical implication:

- do not assume "MPC60 file" implies one fixed wrapper version
- do not assume "MPC3000 file" implies a disjoint wrapper family
- treat wrapper version as something selected from firmware provenance
