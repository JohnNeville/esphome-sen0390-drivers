# ESPHome component for the DFRobot SEN0390 ambient light sensor

An ESPHome external component for the DFRobot SEN0390 (Gravity V30B) ambient light sensor, read
over I2C. It talks to the sensor directly, with no Arduino library, so it builds with the ESP-IDF
framework.

It was written for the [SunSprout](https://github.com/JohnNeville/SunSprout) garden sensors, but
it does not depend on that board.

## Use

```yaml
external_components:
  - source: github://JohnNeville/esphome-sen0390-drivers
    components: [dfrobot_sen0390]

i2c:
  sda: GPIO2
  scl: GPIO1

sensor:
  - platform: dfrobot_sen0390
    lux:
      name: "Garden Light"
    update_interval: 60s
```

| Option | Default | What it does |
|---|---|---|
| `lux` | required | The illuminance sensor, in lux. |
| `raw` | none | A text sensor with the four raw bytes read from register `0x00`, as hex. For debugging. |
| `address` | `0x4A` | The sensor's I2C address. |
| `update_interval` | `5s` | How often to read. |

See [`tests/test-dfrobot_sen0390.yaml`](tests/test-dfrobot_sen0390.yaml) for a configuration that
sets every option.

## How it reads

The sensor returns four bytes from register `0x00`. They are a 32-bit unsigned integer, least
significant byte first, and the lux value is that integer times 1.4, divided by 1000. The
formula is the one in DFRobot's own Arduino library for this sensor; this component only borrows
the formula, not the library. A failed read sets a warning on the component and publishes
nothing.

## Address clash

The sensor's default address, `0x4A`, is also an address of the ADS1115. If both are on one bus,
move the ADS1115 to another address with its `ADDR` pin or jumpers.

## Status

The component has been used in a garden sensor build. CI compiles it against the current ESPHome
release on every change, but there is no automated test against real hardware.

## Licence

MIT, see [`LICENSE`](LICENSE).
