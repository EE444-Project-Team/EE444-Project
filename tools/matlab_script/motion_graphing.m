% motion_graphing.m
% written by: Ashley Schultz
% Displays data sent from motion sensor


clear all
device = serialport('COM9', 460800, 'Timeout',20000, 'Parity',"even");
poll_msp(device);

function poll_msp(device)
    while(1)
        flush(device);
        buff = read(device, 1, "uint8");
        disp('Got command')
        switch(buff(1))
            % Time request
            case 1
                t = datetime('now');
                time = datevec(t);
                flush(device)
                f_day = uint8(time(3));
                write(device, f_day, "uint8");
                language = 'local';
                DayForm = 'long';
                [DayNumber,~] = weekday(t,DayForm,language);
                dow_f = uint8(DayNumber);
                write(device, dow_f, "uint8");
                hour_f = uint8(time(4));
                write(device, hour_f, "uint8");
                min_f = uint8(time(5));
                write(device, min_f, "uint8");
                sec_f = uint8(time(6));
                write(device, sec_f, "uint8");
                disp('Time updated')
            case 2
                % Alarm trip
                day = read(device, 1, "uint8");
                dow = read(device, 1, "uint8");
                hour = read(device, 1, "uint8");
                minute = read(device, 1, "uint8");
                second = read(device, 1, "uint8");
                disp('Motion sensor tripped at')
                disp(datetime([2026 4 day hour minute second]))
                [~,Dayname] = weekday(datetime([2026 4 day hour minute second]), 'long', 'local');
                disp('At day of the week: ')
                disp(Dayname)
        end
    end
end