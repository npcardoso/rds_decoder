# rds_decoder
An RDS decoder

# Running
    $ make
    $ gnuradio-companion grc/rds_rx.grc
    $ nc -v -lp 12345 127.0.0.1 | ./RDS

# Status
- Syndrome calculation - done
- Stream synchronization - done
- Group handling:
 - Basic Tuning (0A and 0B) - done
 - Program Item Number (1A and 1B) - done
 - Radio Text (2A and 2B) - done
 - AID (3A) - done
 - CT (4A) - done
 - EON (14A and 14B) - done
 - Others - todo

# Links
https://github.com/bastibl/gr-rds
