# Controller

| Example      | Topic                                        |
|--------------|----------------------------------------------|
| `pid/PIDSim` | a PID controller driving a value to a setpoint |

## PID controller

A PID controller computes a correction from the error `e = setpoint - measured value`:

```
output = Kp * e                      proportional: reacts to the present error
       + Ki * sum(e * dt)            integral:     removes the steady-state offset (the past)
       + Kd * (e - e_previous) / dt  derivative:   damps overshoot (predicts the future)
```

The output is clamped to `[min, max]`, for example a motor's maximum power.

| Gain too high | Effect                                   |
|---------------|------------------------------------------|
| `Kp`          | fast, but oscillates                     |
| `Ki`          | removes offset, but overshoots and winds up |
| `Kd`          | damps, but amplifies measurement noise   |

`pid.h/pid.cpp` is based on the MIT-licensed implementation by Bradley J. Snyder. It uses the
**pimpl** idiom: `PID` holds a `std::unique_ptr<PIDImpl>`, so the implementation can change without
recompiling the code that uses it. The class is unit tested in `tests/controller/PidTest.cpp`.
