# Challenge 1 - Force/Torque sensor read

## Proposed in 17/Jan/2021 - Deadline 23/Jan/2021

**Goal**: convert the readings from the file `can-messages-log.txt` - which contains CAN messages gotten from a force/torque sensor - into force (in Newtons) and torque (in Newton-meter) values.

Requirements:

* COMMENT YOUR CODE!
* Use what you know best about C++
* Try to learn what you need
* Also, raise questions in our **cpp-moderno** channel at Discord.
* Try your best to understand the sensor's manual

Input:

* `can-messages-log.txt`: file that have CAN messages read from the sensor. This is the structure of the file:

    ``` 
    interface message-id [payload-size] byte0 byte1 byte2 byte3 byte4 byte5 byte6 byte7
    ```

    We should note that we can have from 0 bytes to 8 bytes, depending on payload size.

    A portion of the real `can-messages-log.txt` is shown below:

    ```
    can1  7F2   [8]  3F 40 25 6A C1 DA D5 F4
    can1  7F3   [8]  3F F8 5F 58 C1 CC ED 0E
    can1  7F4   [8]  3F 9F F9 FC C1 DD 29 AD
    can1  702   [1]  00
    can1  000   [2]  82 00
    can1  7F0   [0] 
    can1  7F0   [8]  40 00 00 12 FE 3A E6 A8
    can1  701   [1]  00
    can1  7F0   [0] 
    can1  7F1   [6]  04 FA FA 93 01 43
    can1  7F0   [8]  40 00 00 43 FE 77 E6 EE
    can1  7F1   [6]  05 69 FA E0 01 83
    can1  000   [2]  81 00
    can1  000   [2]  82 00
    can1  000   [2]  02 00
    can1  000   [2]  80 00
    ```

    Remarks:

    * The messages sent by the sensor has an ID that starts with  `7F` (e.g. `7F2`, `7F5`, `7FF`, etc). The other ones were sent from Beaglebone Black or from the EPOS motor driver. If you want to know more about CAN protocol, go to this [CAN bus Wikipedia article](https://en.wikipedia.org/wiki/CAN_bus)

    * Each message received from the sensor was sent after a request from Beaglebone Black. The requests and the responses are explained in the sensor's manual `ati-netcanoem.pdf`
        * You will need to get the **calibration matrix** and the **counts per force** and **counts per torque** from the log.

* ```ati-netcanoem.pdf```: sensor's manual. It has all the information needed to understand sensor's CAN messages.

    * If any questions, please, send them in our channel :)

