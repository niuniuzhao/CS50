-- Keep a log of any SQL queries you execute as you solve the mystery.

-- .schema always!
.schema

-- Find crime scene description
SELECT description
FROM crime_scene_reports
WHERE year = 2023 AND month = 7 AND day = 28
AND street = 'Humphrey Street';

-- Description mentions INTERVIEWS TODAY. Look for interviews.
SELECT *
FROM interviews
WHERE year = 2023 AND month = 7 AND day = 28;

-- based on Ruth's interview: check bakery security log for car license plate in the relevant time frame
SELECT name
FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25;

-- based on Eugene's interview: check ATM transactions earlier in the morning
SELECT name
FROM people
where id IN (

    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (

        SELECT account_number
        FROM atm_transactions
        WHERE year = 2023 AND month = 7 AND day = 28
        )
    );

-- based on Raymond's interview: check phone calls less than a minute
SELECT name
FROM people
where phone_number IN (

    SELECT caller
    FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60
    );

-- OK they all seemed to work. LET'S FIND WHO FITS ALL THREE CONDITIONS
SELECT name
FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25
AND people.id IN (

    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE year= 2023 AND month = 7 AND day = 28)
    )
AND phone_number IN (

    SELECT caller
    FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60
    );

-- o there're still two suspects. (Diana? Bruce?)

-- try looking at the earliest flight on the next day
SELECT name
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = (
        SELECT id
        FROM flights
        WHERE year = 2023 AND month = 7 AND day = 29
        ORDER BY hour, minute
        LIMIT 1
        )
    );

-- I think we found who it is... Just to be sure:
SELECT name
FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25
AND people.id IN (

    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE year= 2023 AND month = 7 AND day = 28)
    )
AND phone_number IN (

    SELECT caller
    FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60
    )
AND passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = (
        SELECT id
        FROM flights
        WHERE year = 2023 AND month = 7 AND day = 29
        ORDER BY hour, minute
        LIMIT 1
        )
    );

-- VOILA!
-- It's BRUCE.

-- accomplice who? check the phone calls
SELECT name
FROM people
WHERE phone_number IN (
    SELECT receiver
    FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60 AND caller = (
        SELECT phone_number
        FROM people
        WHERE name = 'Bruce'
        )
    );

-- and where did Bruce go?
SELECT city
FROM airports
WHERE id = (
    SELECT destination_airport_id
    FROM flights
    WHERE year = 2023 AND month = 7 AND day = 29
    ORDER BY hour, minute
    LIMIT 1
    );

-- CASE CLOSED
