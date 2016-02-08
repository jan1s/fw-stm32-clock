# fw-clock
Common firmware for several stm32f10x based clock projects

## Commandline Interface ##

The clock offers a commandline interface over a serial port.

### Setting the time ###

Since the clock is working with UTC inside, the time needs to be set in UTC.
Conversion to the relevant timezone is enabled later by setting the timezone.
The time can be set with the `rtc_write` command.

`rtc_write <yr> <mon> <day> <hr> <min> <sec>`

### Setting the timezone ###

The timezone can be set with the `tz_write` command.

`tz_write (std|dst) <offset> <hour> <dow> <week> <month>`

- offset (offset in minutes for the given rule)
- hour   (hour of day, where the new time starts)
- dow    (dow day of week, 0=monday ... 6=sunday)
- week   (week of month, 1=first ... 3=third, 0=last)
- month  (month in year, 1=january ... 12=december)

For Germany just type:

`tz_write std 60 3 6 0 10`

`tz_write dst 120 2 6 0 3`

Most common Daylight Saving Time rules:

| Timezone      | Name | Offset | Month   | Week | Day of week | Hour |
| ------------- | ---- | ------ | ------- | ---- | ----------- | ---- |
| Europe/London | WET  | UTC+0  | March   | last | Sunday      | 1    |
|               | WEST | UTC+1  | October | last | Sunday      | 1    |
| Europe/Berlin | MEZ  | UTC+1  | March   | last | Sunday      | 1    |
|               | MESZ | UTC+2  | October | last | Sunday      | 1    |
| Europe/Athens | EET  | UTC+2  | March   | last | Sunday      | 1    |
|               | EEST | UTC+3  | October | last | Sunday      | 1    |
| Europe/Moscow | MSK  | UTC+4  |         |      |             |      |


## License ##

Some parts of the firmware are based on the [LPC11U_LPC13U_CodeBase][codebase] of [Kevin Townsend][microbuilder]. Some files are modified and should be commented so. Some other files adapt the scheme of coding mostly used in the codebase.

Where possible, all code is provided under a BSD style license, but each file is individually licensed and you should ensure that you fully understand the license terms and limitations of any files you use in your project.

[codebase]: https://github.com/microbuilder/LPC11U_LPC13U_CodeBase
[microbuilder]: http://www.microbuilder.eu/
