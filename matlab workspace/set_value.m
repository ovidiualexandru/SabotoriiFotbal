function set_value(var_string, val, s)
cmd_code = 0;
%fread(s); % empty the buffer
switch var_string
    case 'stop time'
        cmd_code = 29;
    case 'get ball power'
        cmd_code = 30;
    case 'get ball time'
        cmd_code = 31;
    case 'turn 90 power'
        cmd_code = 32;
    case 'turn 90 time'
        cmd_code = 33;
    case 'turn 180 power'
        cmd_code = 34;
    case 'turn 180 time'
        cmd_code = 35;
    case 'go back power'
        cmd_code = 36;
    case 'go back time'
        cmd_code = 37;
    case 'veer power min'
        cmd_code = 38;
    case 'veer power max'
        cmd_code = 39;
    case 'go forward power'
        cmd_code = 40;
    case 'suck power'
        cmd_code = 41;
    case 'spit power'
        cmd_code = 42;
    case 'hold power'
        cmd_code = 43;
    case 'servo raise'
        cmd_code = 44;
    case 'servo lower'
        cmd_code = 45;
    case 'servo move time'
        cmd_code = 46;
end
fwrite(s, uint8(cmd_code));
fwrite(s, uint8(val));
end