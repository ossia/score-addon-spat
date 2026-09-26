# Spat
A new and wonderful [ossia score](https://ossia.io) add-on

## Spatialization (VBAP / MBAP)

A native process that spatializes mono sources over a speaker setup with the
VBAP (dome) and MBAP (cube) algorithms from
[SpatGRIS](https://github.com/GRIS-UdeM/SpatGRIS). It reads SpatGRIS speaker
setup files, and each source picks its algorithm independently.

![The process in score, next to its speaker view on the Satosphère dome](docs/gris-spatialization.png)

The node lists the inlets on the left, the settings shared by every source at
the top, then one row per source (position, azimuth and zenith spans, VBAP or
MBAP), four sources per page. The UI button opens a 3D view of the speaker
setup that follows the sources while playing and lights each speaker with the
level sent to it.

## Developing

### Test large speaker setups with an optimised build

A Debug build of score cannot process a large speaker setup in real time. With
the Satosphère dome (93 speakers, 104 output channels), 1024-frame buffers at
44.1 kHz and 4 sources, the audio thread uses 70–100 % of a core in Debug and
about 8 % in a RelWithDebInfo build. Most of the Debug cost is not in this
add-on: libossia checks every sample of every input and output for NaN and Inf
when `NDEBUG` is not defined (`OSSIA_DEBUG_MISBEHAVING_NODES` in
`graph_utils.hpp`). The spatializer itself takes 2–3 ms per buffer in Debug and
0.1–0.3 ms optimised, against a 23 ms budget.

On Linux with PipeWire this does not just cause dropouts: **score gets killed
with SIGKILL**, with nothing in the logs. PipeWire obtains realtime priority for
the audio thread through rtkit, and rtkit only grants it with an
`RLIMIT_RTTIME` of 200 ms (`RTTimeUSecMax`). A realtime thread that never
sleeps goes over that limit and the kernel kills the whole process. You can see
the limit on a running score with
`grep -i realtime /proc/$(pgrep -x ossia-score)/limits`.

What works:

- Configure a second tree with `-DCMAKE_BUILD_TYPE=RelWithDebInfo`. It keeps
  symbols, so gdb stacks stay readable, and it keeps up with the dome easily.
- In a Debug tree, use small speaker setups, or set `SCORE_AUDIO_BACKEND=dummy`
  to debug logic without sound: the dummy driver's thread is not realtime, so it
  is never killed.

The first MBAP layout for the dome takes 3.7 s to build in Debug (0.6 s
optimised). It is built in the background when the process is created or its
speaker setup changes, so a Debug build may start playback silent until it is
ready.
