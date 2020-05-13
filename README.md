# README

A repo to decode signals from Alps rotary encoder.
This particular encoder has a mechanical click each half quadrature cycle.

This implementation use no interrupts, gives a new CW/CCW step each half quadrature cycle and solve the problem of a change in direction.