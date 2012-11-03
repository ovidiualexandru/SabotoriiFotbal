function action(cmd_string, s)
cmd_code = 0;
switch cmd_string
    case 'turn left'
        cmd_code = 1;
    case 'turn right'
        cmd_code = 2;
    case 'turn around'
        cmd_code = 3;
    case 'go back'
        cmd_code = 4;
    case 'veer left'
        cmd_code = 5;
    case 'veer right'
        cmd_code = 6;
    case 'raise'
        cmd_code = 7;
    case 'lower'
        cmd_code = 8;
    case 'suck'
        cmd_code = 9;
    case 'spit'
        cmd_code = 10;
    case 'hold'
        cmd_code = 11;
    case 'get ball'
        cmd_code = 12;
    case 'go forward'
        cmd_code = 13;
    case 'stop'
        cmd_code = 14;
	case 'stop rola'
		cmd_code = 15;
	case 'step left'
		cmd_code = 16;
	case 'step right'
		cmd_code = 17;
	case 'center ball'
		cmd_code = 18;
	case 'center light'
		cmd_code = 19;
end
fwrite(s, uint8(cmd_code));
end