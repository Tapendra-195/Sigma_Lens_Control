# Sigma_Lens_Control
Code to control the 15mm Sigma Art Lens. Sigma lens use E-Mount Sony's protol to communicate. The backend code is used in Teensy 4.0 microcontroller and the frontend code is used to set the focus and aperture of the lens.

# To Compile Frontend:
- cd frontend
- mkdir bin
- cd bin
- qmake ..
- make

# To upload backend firmware:
- Open backend.ino in arduino ide then compile and upload.

# To do:
- Implement timeout in each state. If no message is received from the lens for n sec, the lens should timeout. Can be easily implemented with the update method in the states.
