# Voltmeter-Using-Microcontroller-and-Resistors

A universal DC voltmeter sketch for any Arduino-compatible board, using an external resistor voltage divider and the onboard ADC. Configure the constants at the top of the sketch for your board and resistors. Just upload the `voltmeter.ino` to your microcontroller using Arduino IDE.
---

## How it works

The circuit uses a **potential divider** to scale down the input voltage to a level safe for the ADC pin. The microcontroller reads the scaled voltage and reconstructs the original input voltage using the known resistor ratio.

### Voltage divider

Two resistors are connected in series between the input voltage and ground:

```
V_in (+) ──[ R_TOP ]──┬──[ R_BOTTOM ]── GND
                       │
                     ADC_PIN
```

The voltage at the junction (ADC pin) is:

```math
V_{ADC} = V_{in} \times \frac{R_{BOTTOM}}{R_{TOP} + R_{BOTTOM}}
```

### ADC conversion

The ADC produces a raw integer reading proportional to the voltage at the pin:

```math
V_{ADC} = \frac{ADC_{raw}}{ADC_{MAX}} \times V_{ref}
```

### Measurement Formula

Substituting the ADC conversion into the divider formula gives the complete expression:

```math
V_{in} = \frac{ADC_{raw}}{ADC_{MAX}} \times V_{ref} \times \frac{R_{TOP} + R_{BOTTOM}}{R_{BOTTOM}} \times K_{cal}
```

Where $K_{cal}$ is a calibration factor (default `1.000`, adjust after measuring a known voltage).

### Choosing resistor values

Choose $R_{TOP}$ and $R_{BOTTOM}$ so that the maximum expected input voltage produces at most $V_{ref}$ at the ADC pin:

```math
\frac{R_{TOP} + R_{BOTTOM}}{R_{BOTTOM}} = \frac{V_{in,max}}{V_{ref}}
```

### Noise reduction — averaging

The sketch averages $N$ samples to reduce ADC noise:

```math
ADC_{raw} = \frac{1}{N} \sum_{i=1}^{N} ADC_i
```

A higher $N$ gives a cleaner reading at the cost of a slower update rate. The default is `NUM_SAMPLES = 100` with a 2 ms inter-sample delay (~200 ms per reading).

---

## Configuration

Edit only the constants at the top of the sketch:

| Constant | Description |
|---|---|
| `ADC_PIN` | ADC pin on your board (e.g. `A0`) |
| `R_TOP` | Top resistor value in ohms |
| `R_BOTTOM` | Bottom resistor value in ohms |
| `A0_MAX_VOLTAGE` | ADC reference voltage of your board (V) |
| `ADC_MAX` | Max ADC count for your board's bit depth |
| `CALIBRATION_FACTOR` | Fine-tune after measuring a known voltage |
| `NUM_SAMPLES` | Number of samples to average per reading |

### Board reference

| Board | `ADC_PIN` | `A0_MAX_VOLTAGE` | `ADC_MAX` |
|---|---|---|---|
| Arduino Uno / Nano | `A0` | `5.0` | `1023` |
| NodeMCU ESP8266 | `A0` | `3.3` | `1023` |
| ESP32 | `34` | `3.3` | `4095` |
| Arduino Due | `A0` | `3.3` | `4095` |
| STM32 (default) | `A0` | `3.3` | `4095` |

### Common divider configurations

| Input range | $R_{TOP}$ | $R_{BOTTOM}$ | Ratio |
|---|---|---|---|
| 0 – 5 V | 10 kΩ | 10 kΩ | 2.0× |
| 0 – 12 V | 45 kΩ | 15 kΩ | 4.0× |
| 0 – 25 V | 75 kΩ | 25 kΩ | 4.0× |
| 0 – 3.3 V | 0 Ω (wire) | any | 1.0× |

---

## Calibration (Not Mandatory)

1. Set `CALIBRATION_FACTOR = 1.000` and flash the sketch.
2. Apply a known, accurate voltage (e.g. 9.00 V verified with a trusted multimeter).
3. Note the reported voltage from the Serial Monitor (e.g. `8.84 V`).
4. Compute the new factor:

```math
K_{cal} = \frac{V_{known}}{V_{reported}} = \frac{9.00}{8.84} \approx 1.0181
```

5. Set `CALIBRATION_FACTOR = 1.0181` and reflash. The reading should now match.

---

## Wiring

```
V_in (+) ──[ R_TOP ]──┬──[ R_BOTTOM ]── GND
                       │
                     ADC_PIN (board)

V_in (−) ── GND (shared with board GND)
```

> **Important:** The GND of the voltage source being measured must be connected to the GND pin of the microcontroller board. Without this shared ground the reading will be meaningless.

---

## Serial output

```
ADC Raw: 512.0 | A0 Voltage: 1.648 V | Input Voltage: 6.592 V
```

---

## Notes

- Measure **DC voltages only**.
- Do not exceed the rated voltage for your resistors (power dissipation: $P = V_{in}^2 / (R_{TOP} + R_{BOTTOM})$).
- For voltages above ~60 V, use appropriately rated high-voltage resistors and exercise caution.
- This design measures voltages **relative to its own GND** — it is not suitable for floating or differential measurements without additional circuitry.
