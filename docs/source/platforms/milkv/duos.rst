.. |yes| image:: ../../images/yes.png
.. |no| image:: ../../images/no.png

.. role:: underline
   :class: underline

Milk-V Duos
==========

+----------------+-----------------+
| System on Chip | Sophgo SG2002   |
+----------------+-----------------+
| Number GPIO    | 39              |
+----------------+-----------------+
| Status         | Fully supported |
+----------------+-----------------+

Supported Features
------------------

+----------------+-----------------+
| Read / Write   | |yes|           |
+----------------+-----------------+
| Interrupt      | |yes|           |
+----------------+-----------------+

GPIO Mapping
------------

J3:
+------+----+-----+---+-----+----+-----+----+------+-----+----+----+-----+
| 5V   | 5V | GND | 8 | 10  | 12 | GND | 16 | 18   | GND | 22 | 24 | 26  |
+------+----+-----+---+-----+----+-----+----+------+-----+----+----+-----+
| 3.3V | 3  | 5   | 7 | GND | 11 | 13  | 15 | 3.3V | 19  | 21 | 23 | GND |
+------+----+-----+---+-----+----+-----+----+------+-----+----+----+-----+

.. image:: duos.jpg

J4:
+-----+-----+-----+-----+------+----+----+-----+----+----+-----+----+----+
| N/A | N/A | N/A | N/A | 3.3V | 41 | 39 | GND | 35 | 33 | GND | 29 | 27 |
+-----+-----+-----+-----+------+----+----+-----+----+----+-----+----+----+
| 5V  | 50  | 48  | 46  | 44   | 42 | 40 | GND | 36 | 34 | GND | 30 | 28 |
+-----+-----+-----+-----+------+----+----+-----+----+----+----+----+-----+

Additional
----------

- Blue LED: 0

Note: GPIO 0 is the system status indicator LED of DuoS.
Before using wiringX to control this LED, its blinking function needs to be disabled in the system.
Otherwise, the control will not take effect and no error message will be returned.
