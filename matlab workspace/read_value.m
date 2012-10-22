function val = read_value(var_string, s)
cmd_code = 0;
fread(s); % empty the buffer
switch var_string
    case 'stop time'
        cmd_code = 16;
    case 'get ball power'
        cmd_code = 17;
    case 'get ball time'
        cmd_code = 18;
    case 'turn 90 power'
        cmd_code = 19;
    case 'turn 90 time'
        cmd_code = 20;
    case 'turn 180 power'
        cmd_code = 21;
    case 'turn 180 time'
        cmd_code = 22;
    case 'go back power'
        cmd_code = 23;
    case 'go back time'
        cmd_code = 24;
    case 'veer power min'
        cmd_code = 25;
    case 'veer power max'
        cmd_code = 26;
    case 'go forward power'
        cmd_code = 27;
    case 'suck power'
        cmd_code = 28;
    case 'spit power'
        cmd_code = 29;
    case 'hold power'
        cmd_code = 30;
    case 'servo raise'
        cmd_code = 31;
    case 'servo lower'
        cmd_code = 32;
    case 'servo move time'
        cmd_code = 33;
    case 'sharp stanga'
        cmd_code = 34;
    case 'sharp dreapta'
        cmd_code = 35;
    case 'sharp jos'
        cmd_code = 36;
    case 'sharp sus'
        cmd_code = 37;
    case 'senz teren'
        cmd_code = 38;
end
fwrite(s, uint8(cmd_code));
val = fread(s, 1, 'uint8');
end