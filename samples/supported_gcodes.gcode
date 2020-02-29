M73  ; set progress
M115 ; query firmware
G90 ; use absolute coordinates
G91 ; use relative coordinates
M83 ; extruder relative mode

G92 E0.0 ; set position of axes

G20 ; set units to inches
G21 ; set units to millimeters

M3 ; spindle on
M5 ; spindle off

G53 ; move in machine coords
G54 ; to G59, set workspace coords or change to workspace

M17 ; steppers on
M18 ; steppers off

M203 ; set max feedrate
M92 ; set steps per unit
G94; ^

G27 ; park toolhead
G28 ; auto-home

M0 ; stop
M112 ; emergency stop!
M410 ; stop steppers now
M2 ; end of program
M108 ; resume

M400 ; wait for moves to finish
