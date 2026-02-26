#pragma once
const int MAX_BUFFER_SIZE = 256;



/*
//Constants for pretending to be a Lens, not needed to pretend to be a camera.
const byte init05[] ={0xF0, 0x75, 0x00, 0x01, 0x01, 0x05, 0x00, 0x11, 0x00, 0x11, 0x00, 0x00, 0x11, 0x00, 0x07, 0x27, 0x00, 0x27, 0x00, 0x64, 0x01, 0x64, 0x01, 0xAA, 0x13, 0x01, 0x00, 0x07, 0x80, 0xFF, 0x9B, 0x00, 0x96, 0x00, 0x27, 0x01, 0x00, 0x00, 0xB1, 0x09, 0x42, 0x3F, 0x3A, 0x2C, 0x01, 0x23, 0x09, 0x07, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x06, 0x06, 0x9B, 0x00, 0xD0, 0x04, 0x11, 0x01, 0xF4, 0xFD, 0xD0, 0x04, 0x11, 0x01, 0xF4, 0xFD, 0x02, 0x23, 0x47, 0x43, 0x09, 0x10, 0x0C, 0x97, 0xF1, 0x20, 0x97, 0xF1, 0x20, 0xCF, 0x11, 0x55 };

const byte init06[] = {0xF0, 0x30, 0x00, 0x01, 0x01, 0x06, 0x02, 0x00, 0x9E, 0x25, 0x00, 0x00, 0x10, 0x4E, 0x20, 0x0F, 0x32, 0x00, 0x00, 0x21, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x9E, 0x25, 0x00, 0x41, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xDF, 0x03, 0x55};
*/

// ============================================================
// Magnetometer Calibration (LIS3MDL)
// ============================================================
//
// This calibration corrects:
//   1) Hard-iron offsets (constant bias from nearby materials)
//   2) First-order soft-iron distortion (per-axis scaling)
//
// Units: microtesla (µT)
//
// ------------------------------------------------------------
// HOW TO CALIBRATE (repeat if hardware / mounting changes):
//
// 1. Enable MAG CAL MODE (high-rate Bx,By,Bz streaming).
// 2. Rotate sensor slowly through FULL 3D orientations:
//    - roll, pitch, yaw
//    - figure-8 motions
//    - 30–60 seconds minimum
// 3. Track min/max for each axis:
//      Bx_min, Bx_max
//      By_min, By_max
//      Bz_min, Bz_max
//
// 4. Compute OFFSETS:
//      ox = (Bx_max + Bx_min)/2
//      oy = (By_max + By_min)/2
//      oz = (Bz_max + Bz_min)/2
//
// 5. Compute SCALE:
//      rx = (Bx_max - Bx_min)/2
//      ry = (By_max - By_min)/2
//      rz = (Bz_max - Bz_min)/2
//
//      r_avg = (rx + ry + rz)/3
//
//      sx = r_avg / rx
//      sy = r_avg / ry
//      sz = r_avg / rz
//
// 6. Apply in firmware:
//      bx = (bx - ox) * sx
//      by = (by - oy) * sy
//      bz = (bz - oz) * sz
//
// ------------------------------------------------------------
// NOTES:
// - Perform calibration in final mechanical configuration
//   (inside housing, with nearby metal present).
// - Recalibrate if mounting, wiring, or nearby materials change.
// - For higher precision, a full ellipsoid fit can replace this.
//
// IMPORTANT:
// When collecting calibration data, you MUST disable any existing calibration.
// Set:
//
//   MAG_OX = 0
//   MAG_OY = 0
//   MAG_OZ = 0
//
//   MAG_SX = 1
//   MAG_SY = 1
//   MAG_SZ = 1
//
// Otherwise you will be calibrating already-corrected data,
// which leads to incorrect offsets and scaling.
// ============================================================

// -------- CURRENT CALIBRATION (Feb 2026, Rev2 board) --------

// Hard-iron offsets (muT)
constexpr float MAG_OX = -29.18f;
constexpr float MAG_OY =  4.085f;
constexpr float MAG_OZ = -17.76f;

// Soft-iron scale factors (dimensionless)
constexpr float MAG_SX = 0.7777f;
constexpr float MAG_SY = 1.1627f;
constexpr float MAG_SZ = 1.1701f;

