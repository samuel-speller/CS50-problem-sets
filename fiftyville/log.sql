-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Have a look at the crime scene reports to see what they contain.
SELECT *
FROM crime_scene_reports;

/* EVIDENCE GAINED (list wasn't too long so i just scrolled down)
295 | 2021 | 7     | 28  | Humphrey Street      | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions
the bakery. |
*/


-- Get the interviews from the three witnesses on the day of the robbery
SELECT name, transcript
FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28;

/* EVIDENCE GAINED
| Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
            If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot
            in that time frame.                                                          |
| Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery,
            I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call,
            I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the
            person on the other end of the phone to purchase the flight ticket. |
*/


--check bakery logs for the 10 minutes after the time of the robbery
SELECT *
FROM bakery_security_logs
WHERE month = 7
AND day = 28
AND year = 2021
AND hour = 10
AND minute BETWEEN 15 AND 25;

--check the people table for those licence plates
SELECT *
FROM people
WHERE license_plate IN
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7
    AND day = 28
    AND year = 2021
    AND hour = 10
    AND minute BETWEEN 15 AND 25);

/* EVIDENCE GAINED
+--------+---------+----------------+-----------------+---------------+
|   id   |  name   |  phone_number  | passport_number | license_plate |
+--------+---------+----------------+-----------------+---------------+
| 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
| 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
| 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
| 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
| 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
| 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
| 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
| 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
+--------+---------+----------------+-----------------+---------------+
*/


--check the ATM on Leggett street
SELECT *
FROM atm_transactions
WHERE year = 2021
AND month = 7
AND day = 28
AND transaction_type = 'withdraw'
AND atm_location LIKE 'Leggett%';


--check bank accounts table joined with people to see if there is a match with one of the withdrawl bank accounts
SELECT distinct(name)
FROM people
    JOIN bank_accounts ON bank_accounts.person_id = people.id
    JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE bank_accounts.account_number IN
    (SELECT account_number
    FROM atm_transactions
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND transaction_type = 'withdraw'
    AND atm_location LIKE 'Leggett%');

/* EVIDENCE GAINED
+---------+
|  name   |
+---------+
| Bruce   |
| Diana   |
| Brooke  |
| Kenny   |
| Iman    |
| Luca    |
| Taylor  |
| Benista |
+---------+
*/


--compare both tables for matching names
SELECT name
FROM people
WHERE license_plate IN
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7
    AND day = 28
    AND year = 2021
    AND hour = 10
    AND minute BETWEEN 15 AND 25)
    AND name IN
        (SELECT distinct(name)
        FROM people
        JOIN bank_accounts ON bank_accounts.person_id = people.id
        JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
        WHERE bank_accounts.account_number IN
            (SELECT account_number
            FROM atm_transactions
            WHERE year = 2021
            AND month = 7
            AND day = 28
            AND transaction_type = 'withdraw'
            AND atm_location LIKE 'Leggett%'));

/* EVIDENCE GAINED
+-------+
| name  |
+-------+
| Iman  |
| Luca  |
| Diana |
| Bruce |
+-------+
The thief is one of these people */

--Now check the phone records that are less than a minute
SELECT name
FROM phone_calls
JOIN people ON people.phone_number = phone_calls.caller
WHERE year = 2021
AND month = 7
AND day = 28
AND duration < 60;

--check names that are also in the atm/bakery list
SELECT name
FROM people
WHERE license_plate IN
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7
    AND day = 28
    AND year = 2021
    AND hour = 10
    AND minute BETWEEN 15 AND 25)
AND name IN
    (SELECT distinct(name)
    FROM people
    JOIN bank_accounts ON bank_accounts.person_id = people.id
    JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
    WHERE bank_accounts.account_number IN
        (SELECT account_number
        FROM atm_transactions
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND transaction_type = 'withdraw'
        AND atm_location LIKE 'Leggett%'))
AND name IN
    (SELECT name
    FROM phone_calls
    JOIN people ON people.phone_number = phone_calls.caller
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60);

/*EVIDENCE GAINED

+-------+
| name  |
+-------+
| Diana |
| Bruce |
+-------+
The thief is one of these people
*/

--now check the earliest flight out of fiftyville on the next day
SELECT *
FROM flights
JOIN airports ON airports.id = flights.origin_airport_id
WHERE flights.year = 2021
    AND flights.month = 7
    AND flights.day = 29
    AND airports.full_name LIKE 'Fiftyville%'
ORDER BY hour;

/*
+----+-------------------+------------------------+------+-------+-----+------+--------+
| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+----+-------------------+------------------------+------+-------+-----+------+--------+
| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
*/
--check passengers on that fight

SELECT name
FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
WHERE passengers.flight_id = 36;

--Bruce was on the flight but Diana wasn't!

--THE THEIF WAS BRUCE!!!!

--Check where he escaped to using the destination airport

SELECT full_name
FROM airports
WHERE id = 4;

/* He escaped to:
+-------------------+
|     full_name     |
+-------------------+
| LaGuardia Airport |
+-------------------+
(in new york)
*/

--check the phone call to see who his acomplice was

SELECT receiver
FROM phone_calls
JOIN people ON people.phone_number = phone_calls.caller
WHERE year = 2021
AND month = 7
AND day = 28
AND duration < 60
AND people.name = 'Bruce';

/*
+----------------+
|    receiver    |
+----------------+
| (375) 555-8161 |
+----------------+
*/

--check people for that number
SELECT name
FROM people
WHERE phone_number = '(375) 555-8161';

--Acomplice was Robin











