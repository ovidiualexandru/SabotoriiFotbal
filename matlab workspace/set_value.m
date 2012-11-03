function set_value(var_string, val, s)
cmd_code = 0;
%fread(s); % empty the buffer
switch var_string
    case 'stop time'
        cmd_code = 80;
    case 'get ball power'
        cmd_code = 81;
    case 'get ball time'
        cmd_code = 82;
    case 'turn 90 power'
        cmd_code = 83;
    case 'turn 90 time'
        cmd_code = 84;
    case 'turn 180 power'
        cmd_code = 85;
    case 'turn 180 time'
        cmd_code = 86;
    case 'go back power'
        cmd_code = 87;
    case 'go back time'
        cmd_code = 88;
    case 'veer power min'
        cmd_code = 89;
    case 'veer power max'
        cmd_code = 90;
    case 'go forward power'
        cmd_code = 91;
    case 'suck power'
        cmd_code = 92;
    case 'spit power'
        cmd_code = 93;
    case 'hold power'
        cmd_code = 94;
    case 'servo raise'
        cmd_code = 95;
    case 'servo lower'
        cmd_code = 96;
    case 'servo move time'
        cmd_code = 97;
	case 'turn 10 power'
		cmd_code = 98;
	case 'turn 10 time'
		cmd_code = 99;
end
fwrite(s, uint8(cmd_code));
fwrite(s, uint8(val));
end