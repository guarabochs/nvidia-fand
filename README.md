# nvidia-fand
===

"fan curve" thing i hacked up.

## usage
===

i have made this for my own system and i have no plans nor the time to do
anything beyond this at the moment. given that, do know that

- **this thing makes calls to nvml fan control functions, which disable
  automatic gpu fan control and leaves this responsability to the caller**.
  it is not my fault or responsiblity if you toast your hardware using this
  code. if in doubt, read the source before compiling.

- my nvidia gpu registers only a single fan, so all fan control commands are
  sent to fan 0.

- no binaries. please compile this yourself.
