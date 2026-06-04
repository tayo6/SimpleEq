# Simple EQ VST
### Developed by **Adeniran Tayo** ###

A lightweight equalizer plugin with three essential bands: low cut, high cut, and a parametric notch filter with adjustable Q.

**NOTE**: I have not perfected the lookalike scripts for its user interface 

## Features

- **Low Cut Filter** – Removes rumble and low-end noise (20–500 Hz)
- **High Cut Filter** – Attenuates harsh highs and hiss (2–20 kHz)
- **Notch Filter** – Surgical elimination of problem frequencies with variable Q

## Controls

| Parameter       | Range         | Description                                       |
|----------------|---------------|---------------------------------------------------|
| Low Cut Freq    | 20 – 500 Hz   | Cutoff frequency for the high‑pass filter        |
| High Cut Freq   | 2 – 20 kHz    | Cutoff frequency for the low‑pass filter         |
| Notch Freq      | 20 – 20 kHz   | Center frequency of the notch                    |
| Notch Q         | 0.5 – 10      | Sharpness of the notch (higher = narrower)       |
| Notch Depth     | -24 – 0 dB    | Attenuation level at the notch center            |

## Usage

1. Use **Low Cut** to clean up subsonic content or stage rumble.
2. Use **High Cut** to reduce sibilance or high‑frequency noise.
3. Use **Notch** with high Q to isolate a ringing feedback frequency, or with low Q for gentle tonal shaping.

Simple, clean, and CPU‑friendly – ideal for quick corrective EQ tasks.