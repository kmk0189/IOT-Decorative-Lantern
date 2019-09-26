/*
**  Copyright (c) 2019, Kasey Koscianski.
*****************************************************************************
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Affero General Public License as published
**  by the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Affero General Public License for more details.
**
**  You should have received a copy of the GNU Affero General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************
*/

#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "kkbb01.hpp"

constexpr uint8_t  spi0        = 0;
constexpr uint32_t spi_min_spd = 500000;
constexpr uint32_t spi_max_spd = 32000000;

constexpr uint8_t sig_enable_pin = 6;
constexpr uint8_t pwr_enable_pin = 10;

Kkbb01::Kkbb01(uint32_t tx_speed)
{
    wiringPiSetup();

    pinMode(pwr_enable_pin, OUTPUT);
    pinMode(sig_enable_pin, OUTPUT);

    digitalWrite(pwr_enable_pin, HIGH);

    if (tx_speed < spi_min_spd)
    {
        tx_speed = spi_min_spd;
    }
    else if (tx_speed > spi_max_spd)
    {
        tx_speed = spi_max_spd;
    }

    fd = wiringPiSPISetup(spi0, tx_speed);
}

void Kkbb01::write_message(std::vector<uint8_t> message)
{
    // This board has the ability to turn off the line driver chip, so
    // we have to turn it on before we transmit.
    digitalWrite(sig_enable_pin, HIGH);

    // We give the chip a few ticks to start up.
    delayMicroseconds(10);

    ssize_t result = write(fd, message.data(), message.size());

    // Wait for the attached devices to trigger.
    delayMicroseconds(10);

    // Then we turn it off to save power.
    digitalWrite(sig_enable_pin, LOW);

    if (result < 0)
    {
        std::cerr << "Unable to send message." << std::endl;
    }
    else if (result < (ssize_t)message.size())
    {
        std::cerr << "Unable to send full message, only " << message.size() - result << " bytes were sent." << std::endl;
    }
}
