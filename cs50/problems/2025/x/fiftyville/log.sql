-- Keep a log of any SQL queries you execute as you solve the mystery.

--Discover the crime scenes details happened in July 28,2024 in Humphrey Street
SELECT description
  FROM crime_scene_reports
    WHERE year = 2024
    AND month = 07
    AND day = 28
    AND street LIKE "%Humphrey Street%";

--Details about the inverviews with the witnesses about the incident of the theft of CS50Duck.
SELECT transcript
  FROM interviews
    WHERE year = 2024
    AND month = 07
    AND day = 28
    AND transcript LIKE "%bakery%";

--Discover the account number of suspect, who have withdrawed money in Legget Street ATM early in the day
SELECT person_id
  FROM bank_accounts
    WHERE account_number IN (
      SELECT account_number
        FROM atm_transactions
          WHERE atm_location LIKE "%Leggett Street%"
          AND year = 2024
          AND month = 07
          AND day = 28
          AND transaction_type LIKE "withdraw"
    );


--Earliest flight on July 29, 2024 from fiftyville
WITH earliest_flight AS (
  SELECT destination_airport_id
    FROM flights
      WHERE year = 2024
      AND month = 07
      AND day = 29
      AND origin_airport_id IN (
          SELECT id
            FROM airports
              WHERE city LIKE "%Fiftyville%"
      )
        ORDER BY hour ASC, minute ASC LIMIT 1
)
--Passagers on the earliest flight on July 29, 2024 from fiftyville
SELECT passport_number
  FROM passengers
    WHERE flight_id IN earliest_flight;

--Phone calls with duration less than a minute to discover the suspect and accomplice numbers
SELECT caller, receiver
  FROM phone_calls
      WHERE year = 2024
        AND month = 07
        AND day = 28
        AND duration < 60;

--Discover the license_plate of cars in the parking of the bakery that stayed equal or less than 10 minutes
SELECT license_plate
  FROM bakery_security_logs
      WHERE year = 2024
        AND month = 07
        AND day = 28
        AND hour = 10
        AND minute BETWEEN 15 AND 25
        AND activity LIKE "exit";


--Now discover the suspect that must be included in phone, account, flight, and license plate querys (Bruce)
--CTE to organizate the logic
WITH earliest_flight AS (
  SELECT id
    FROM flights
      WHERE year = 2024
      AND month = 07
      AND day = 29
      AND origin_airport_id IN (
          SELECT id
            FROM airports
              WHERE city LIKE "%Fiftyville%"
      )
        ORDER BY hour ASC, minute ASC
          LIMIT 1
),
passengers_passports AS (
  SELECT passport_number
    FROM passengers
      WHERE flight_id IN earliest_flight
),
cars_plates AS (
  SELECT license_plate
    FROM bakery_security_logs
        WHERE year = 2024
          AND month = 07
          AND day = 28
          AND hour = 10
          AND minute BETWEEN 15 AND 25
          AND activity LIKE "exit"
),
suspects_account_bank AS (
  SELECT person_id
    FROM bank_accounts
      WHERE account_number IN (
        SELECT account_number
          FROM atm_transactions
            WHERE atm_location LIKE "%Leggett Street%"
            AND year = 2024
            AND month = 07
            AND day = 28
            AND transaction_type LIKE "withdraw"
      )
),
suspect_phone AS (
  SELECT caller
    FROM phone_calls
        WHERE year = 2024
          AND month = 07
          AND day = 28
          AND duration <= 60
)

SELECT *
  FROM people
    WHERE id IN suspects_account_bank
      AND license_plate IN cars_plates
      AND phone_number IN suspect_phone
      AND passport_number IN passengers_passports;


--Discovered the suspect number i can get the accomplience and the flight (Robin)
SELECT name
  FROM people
    WHERE phone_number IN (
      SELECT receiver
        FROM phone_calls
            WHERE year = 2024
              AND month = 07
              AND day = 28
              AND duration <= 60
              AND caller = "(367) 555-5533"
    );

--Now get the city they were going (New York City)
WITH earliest_flight AS (
  SELECT destination_airport_id
    FROM flights
      WHERE year = 2024
      AND month = 07
      AND day = 29
      AND origin_airport_id IN (
          SELECT id
            FROM airports
              WHERE city LIKE "%Fiftyville%"
      )
        ORDER BY hour ASC, minute ASC LIMIT 1
)

SELECT city
  FROM airports
    WHERE id IN earliest_flight;
