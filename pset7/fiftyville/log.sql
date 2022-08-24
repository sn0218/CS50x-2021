-- Keep a log of any SQL queries you execute as you solve the mystery.

-- look for what happened about the case
SELECT description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND street = "Chamberlain Street";
-- the CS50 duck took place at 10:15am at the courthhouse
-- interviews were conducted with 3 wnitnesses

-- check the transcript of interviews on that day to find some clues on the theft
SELECT name, transcript FROM interviews WHERE (month = 7 AND day = 28);

/*
Ruth: within 10 mins of the theft, she said a thief get into a car in the courthouse parking lot and drive away
check for the security footage fromthe courthouse parking lot in that time frame

Eugene: He recognized the thief who earlier this morning before he arrived at the courthouse
He saw the thief was withdrawing some money on Fifer Street at the ATM

Raymond: he heard the thief had a call with someone who were planning to take the earliest flight out of Fiftyville on 29 July and asked the person to purchase the flight ticket
*/

-- figure out the activity around 10:15am
SELECT * FROM courthouse_security_logs WHERE (month = 7 AND day = 28 AND hour = 10);

-- one entrace record with license_plate = 13FNH73 at 1014
-- 8 exit records with license_plate within 10 minutes
/*
260 | 2020 | 7 | 28 | 10 | 16 | exit | 5P2BI95
261 | 2020 | 7 | 28 | 10 | 18 | exit | 94KL13X
262 | 2020 | 7 | 28 | 10 | 18 | exit | 6P58WS2
263 | 2020 | 7 | 28 | 10 | 19 | exit | 4328GD8
264 | 2020 | 7 | 28 | 10 | 20 | exit | G412CB7
265 | 2020 | 7 | 28 | 10 | 21 | exit | L93JTIZ
266 | 2020 | 7 | 28 | 10 | 23 | exit | 322W7JE
267 | 2020 | 7 | 28 | 10 | 23 | exit | 0NTHK55
*/


-- check the people who left the courthouse part lot within the time frame
SELECT * FROM people
WHERE license_plate = "5P2BI95" OR license_plate = "94KL13X" OR license_plate = "6P58WS2" OR license_plate = "4328GD8"
OR license_plate = "G412CB7" OR license_plate = "L93JTIZ" OR license_plate = "22W7JE" OR license_plate = "NTHK55"


/*
Possible suspects:

id | name | phone_number | passport_number | license_plate
221103 | Patrick | (725) 555-4692 | 2963008352 | 5P2BI95
243696 | Amber | (301) 555-4174 | 7526138472 | 6P58WS2
396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
398010 | Roger | (130) 555-0289 | 1695452385 | G412CB7
467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X
*/

-- check the atm transactions withdrawing record on Fifer Street on 28 July
SELECT * FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = "Fifer Street";
/*
id | account_number | year | month | day | atm_location | transaction_type | amount
246 | 28500762 | 2020 | 7 | 28 | Fifer Street | withdraw | 48
264 | 28296815 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
266 | 76054385 | 2020 | 7 | 28 | Fifer Street | withdraw | 60
267 | 49610011 | 2020 | 7 | 28 | Fifer Street | withdraw | 50
269 | 16153065 | 2020 | 7 | 28 | Fifer Street | withdraw | 80
275 | 86363979 | 2020 | 7 | 28 | Fifer Street | deposit | 10
288 | 25506511 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
313 | 81061156 | 2020 | 7 | 28 | Fifer Street | withdraw | 30
336 | 26013199 | 2020 | 7 | 28 | Fifer Street | withdraw | 35
*/

-- check the bank_accounts to check person id to filter the suspects
SELECT * FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = "Fifer Street");

/*
account_number | person_id | creation_year
49610011 | 686048 | 2010
86363979 | 948985 | 2010
26013199 | 514354 | 2012
16153065 | 458378 | 2012
28296815 | 395717 | 2014
25506511 | 396669 | 2014
28500762 | 467400 | 2014
76054385 | 449774 | 2015
81061156 | 438727 | 2018
*/

-- filter the suspect by person_id who left the courthouse park lot and withdrew the money at ATM on Fifer Street
SELECT name FROM people WHERE ID IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = "Fifer Street"))
INTERSECT
SELECT name FROM people
WHERE license_plate = "5P2BI95" OR license_plate = "94KL13X" OR license_plate = "6P58WS2" OR license_plate = "4328GD8"
OR license_plate = "G412CB7" OR license_plate = "L93JTIZ" OR license_plate = "22W7JE" OR license_plate = "NTHK55";

/* 
Possible suspects: 

Danielle
Elizabeth
Ernest
*/

-- check the airports in Fiftyville
SELECT id, full_name FROM airports WHERE city = "Fiftyville";

-- check the flight on 29 July from Fiftyville
SELECT * FROM flights WHERE month = 7 AND day = 29 AND origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville");

/*
id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
18 | 8 | 6 | 2020 | 7 | 29 | 16 | 0
23 | 8 | 11 | 2020 | 7 | 29 | 12 | 15
36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20
43 | 8 | 1 | 2020 | 7 | 29 | 9 | 30
53 | 8 | 9 | 2020 | 7 | 29 | 15 | 20
*/

-- check passport number and phone_number by name of suspects
SELECT passport_number FROM people WHERE name IN (SELECT name FROM people WHERE ID IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = "Fifer Street"))
INTERSECT
SELECT name FROM people
WHERE license_plate = "5P2BI95" OR license_plate = "94KL13X" OR license_plate = "6P58WS2" OR license_plate = "4328GD8"
OR license_plate = "G412CB7" OR license_plate = "L93JTIZ" OR license_plate = "22W7JE" OR license_plate = "NTHK55");

/*
passport_number
7049073643
8496433585
5773159633
*/

-- check the flight_id of suspects by passport_number of suspects
SELECT flight_id FROM passengers 
WHERE passport_number 
IN 
(SELECT passport_number FROM people WHERE name IN (SELECT name FROM people WHERE ID IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = "Fifer Street"))
INTERSECT
SELECT name FROM people
WHERE license_plate = "5P2BI95" OR license_plate = "94KL13X" OR license_plate = "6P58WS2" OR license_plate = "4328GD8"
OR license_plate = "G412CB7" OR license_plate = "L93JTIZ" OR license_plate = "22W7JE" OR license_plate = "NTHK55"));

/*
flight_id | passport_number | seat
11 | 8496433585 | 5D
26 | 7049073643 | 2C
36 | 5773159633 | 4A
36 | 8496433585 | 7B
48 | 8496433585 | 7C
*/

-- filter the flight_id on 29 July from Fiftyville with the flight_id of suspects 
SELECT id FROM flights WHERE month = 7 AND day = 29 AND origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
INTERSECT
SELECT flight_id FROM passengers 
WHERE passport_number 
IN (SELECT passport_number FROM people WHERE name IN (SELECT name FROM people WHERE ID IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = "Fifer Street"))
INTERSECT
SELECT name FROM people
WHERE license_plate = "5P2BI95" OR license_plate = "94KL13X" OR license_plate = "6P58WS2" OR license_plate = "4328GD8"
OR license_plate = "G412CB7" OR license_plate = "L93JTIZ" OR license_plate = "22W7JE" OR license_plate = "NTHK55"));

-- the flight id of suspects is confirmed to 36 

-- find the passport number of suspects in the 3 suspects who took the flight_id = 36
SELECT passport_number FROM 
(SELECT * FROM passengers WHERE passport_number IN (SELECT passport_number FROM people WHERE name IN (SELECT name FROM people WHERE ID IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = "Fifer Street"))
INTERSECT
SELECT name FROM people
WHERE license_plate = "5P2BI95" OR license_plate = "94KL13X" OR license_plate = "6P58WS2" OR license_plate = "4328GD8"
OR license_plate = "G412CB7" OR license_plate = "L93JTIZ" OR license_plate = "22W7JE" OR license_plate = "NTHK55")))
WHERE flight_id = 36;

/*
passport_number
5773159633
8496433585
*/

-- check phone calls on 28 July
SELECT * FROM phone_calls WHERE month = 7 AND day = 28;

-- check the phone_number of the two suspects by passport_number
SELECT phone_number FROM people WHERE passport_number IN (5773159633, 8496433585);

/*
id | name | phone_number | passport_number | license_plate
467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X
*/

-- check the phone calls on 28 July match the suspect numbers 
SELECT * FROM phone_calls WHERE month = 7 AND day = 28 AND caller IN (SELECT phone_number FROM people WHERE passport_number IN (5773159633, 8496433585));
/*
id | caller | receiver | year | month | day | duration
233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7 | 28 | 45
236 | (367) 555-5533 | (344) 555-9601 | 2020 | 7 | 28 | 120
245 | (367) 555-5533 | (022) 555-4052 | 2020 | 7 | 28 | 241
285 | (367) 555-5533 | (704) 555-5790 | 2020 | 7 | 28 | 75
*/

-- The suspect phone number is 555-5533 who is Ernest

-- check the receiver of phone call less than a minute
SELECT receiver 
FROM 
(SELECT * FROM phone_calls WHERE month = 7 AND day = 28 AND caller IN (SELECT phone_number FROM people WHERE passport_number IN (5773159633, 8496433585)))
WHERE
duration < 60;

-- check the thief's accomplice by receiver number 
SELECT name FROM people 
WHERE 
phone_number = (SELECT receiver FROM 
(SELECT * FROM phone_calls WHERE month = 7 AND day = 28 AND caller IN (SELECT phone_number FROM people WHERE passport_number IN (5773159633, 8496433585)))
WHERE
duration < 60);
 
-- Berthold helped Ernest to escape

-- check the destination airport id 
SELECT destination_airport_id 
FROM
(SELECT * FROM flights WHERE month = 7 AND day = 29 AND origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville"))
WHERE id = 36;

-- the destination airport id is 4

-- check what city the thief escaped to
SELECT city FROM airports 
WHERE
id = (SELECT destination_airport_id 
FROM
(SELECT * FROM flights WHERE month = 7 AND day = 29 AND origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville"))
WHERE id = 36);

-- The thief escape to London