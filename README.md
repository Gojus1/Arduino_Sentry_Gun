# Arduino_Sentry_Gun
A smart rubber band sentry gun that can operate in both manual and automatic modes, featuring motion detection and precise aiming control.

## Features
- Dual Operation Modes:
- Manual Mode: Full control over aiming with rotary encoder
- Auto Mode: Autonomous scanning and target engagement
- Motion Detection: PIR sensor for target acquisition
- Auto-Fire Capability: Automatically engages detected targets
- Manual Override: Fire manually in both modes
- Serial Monitoring: Real-time status feedback

## Hardware Components
- Arduino Uno (or compatible)
- 2x Servo Motors (SG90 or similar)
- Aim Servo: Controls aiming direction
- Shooter Servo: Fires rubber bands
- Rotary Encoder (5-pin)
- PIR Motion Sensor
- Push button
- Jumper Wires
- Breadboard

## Controls & Operation
- Mode Switching
- Press the Mode Button to toggle between Manual and Auto modes
- Serial monitor displays current mode

### Manual Mode
- Rotary Encoder: Control aim servo position (0-180°)
- Encoder Button: Fire rubber band
- Full manual control over aiming and firing

### Auto Mode
- Automatic Scanning: Aim servo automatically sweeps back and forth
- Motion Detection: PIR sensor detects targets
- Auto-Fire: Automatically fires when motion is detected
- Manual Fire: Press encoder button to fire manually (only when target detected)
