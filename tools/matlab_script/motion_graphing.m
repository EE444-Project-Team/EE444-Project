% motion_graphing.m
% written by: Ashley Schultz
% Displays data sent from motion sensor

clear all;
device = serialport('COM9', 9600, 'Timeout',20, 'Parity',"even");
index = 1;
xl = animatedline('Color', 'r');
yl = animatedline('Color', 'g');
zl = animatedline('Color', 'b');
% Before the next loop, SYNCHRONIZE (Checksum?)
while(1)
a = read(device, 3, "int16");
addpoints(xl,index, a(1));
addpoints(yl,index, a(2));
addpoints(zl,index, a(3));
axis ([index-850 index+150 -6000 10000]);
%axis ([index-1850 index+150 -inf inf]);
drawnow limitrate;
index = index + 1;
end