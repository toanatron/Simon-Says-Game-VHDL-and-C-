# Simon-Says-VHDL-and-CPP
Using both hardware and software to create Simon Says game

This edition of Simon says uses the microblaze microprocessor of the Basys3 board to integrate software and hardware. 
The random generated sequence is through the hardware component pseudorandom number generator (prng).
This component takes in an input seed corresponding to the eight switches on the board.
Following this guide: https://academic.csuohio.edu/chu_p/rtl/fpga_mcs_vhdl_book/appendix_revised.pdf
Create hardware file in Vivado 2018.2 then export hardware and bitstream files to a location that is easily accessible.
Load Xilinx SDK 2018.2 create a new Hardware Platform Specification loading in the MMI, hdf, and bitstream files.
Then create a Board Support Package for a standalone OS.
Finally, create an empty application project, afterward add the C++ source files.

Video demonstration of game: https://youtu.be/tmpm70_OC94
