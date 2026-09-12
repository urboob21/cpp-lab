# Date and time

| Example | Topic |
|---|---|
| `Time` | `<ctime>` timestamps and `strftime`, `<chrono>` calendar dates |

See also `core/concurrency/Timing` for measuring elapsed time with `<chrono>`.

---
# Date and Time
- <ctime> : provides functions and types to work with date and time values including parsing and formatting (inherited from C)

- <chrono>: provides facilities to deal with the duration time points and clocks (std11), it is more modern and type-safe approach

## ctime
- Data Types:
  - `time_t`: for timestamps
  - `struct tm`: for datetime structes 
  - Use `mktime()` to convert struct tm to time_t, and `localtime()` and `gmtime()` for vice versa
- `strftime()` format:
>%a	Short representation of the weekday	Fri
%b	Short representation of the month name	Dec
%B	Full representation of the month name	December
%d	Day of the month with leading zero	09
%e	Day of the month with leading spaces	 9
%H	24-hour format of an hour	14
%I	12-hour format of an hour	02
%M	Minutes within an hour	30
%p	AM or PM	PM
%S	Seconds within a minute	01
%y	2-digit year representation	23
%Y	4-digit year representation	2023