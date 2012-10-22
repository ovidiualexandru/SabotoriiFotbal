function set_value(var_string, val, s)
cmd_code = 0;
%fread(s); % empty the buffer
switch var_string
    case 'stop time'
        cmd_code = 48;
    case 'get ball power'
        cmd_code = 49;
    case 'get ball time'
        cmd_code = 50;
    case 'turn 90 power'
        cmd_code = 51;
    case 'turn 90 time'
        cmd_code = 52;
    case 'turn 180 power'
        cmd_code = 53;
    case 'turn 180 time'
        cmd_code = 54;
    case 'go back power'
        cmd_code = 55;
    case 'go back time'
        cmd_code = 56;
    case 'veer power min'
        cmd_code = 57;
    case 'veer power max'
        cmd_code = 58;
    case 'go forward power'
        cmd_code = 59;
    case 'suck power'
        cmd_code = 60;
    case 'spit power'
        cmd_code = 61;
    case 'hold power'
        cmd_code = 62;
    case 'servo raise'
        cmd_code = 63;
    case 'servo lower'
        cmd_code = 64;
    case 'servo move time'
        cmd_code = 65;
end
fwrite(s, uint8(cmd_code));
fwrite(s, uint8(val));
end