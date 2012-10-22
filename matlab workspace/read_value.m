function val = read_value(var_string, s)
cmd_code = 0;
fread(s); % empty the buffer
switch var_string
    case 'stop time'
        cmd_code = 11;
    case 'get ball power'
        cmd_code = 12;
    case 'get ball time'
        cmd_code = 13;
    case 'turn 90 power'
        cmd_code = 14;
    case 'turn 90 time'
        cmd_code = 15;
    case 'turn 180 power'
        cmd_code = 16;
    case 'turn 180 time'
        cmd_code = 17;
    case 'go back power'
        cmd_code = 18;
    case 'go back time'
        cmd_code = 19;
    case 'veer power min'
        cmd_code = 20;
    case 'veer power max'
        cmd_code = 21;
    case 'go forward power'
        cmd_code = 22;
    case 'suck power'
        cmd_code = 23;
    case 'spit power'
        cmd_code = 24;
    case 'hold power'
        cmd_code = 25;
    case 'servo raise'
        cmd_code = 26;
    case 'servo lower'
        cmd_code = 27;
    case 'servo move time'
        cmd_code = 28;
end
fwrite(s, uint8(cmd_code));
val = fread(s, 1, 'uint8');
end