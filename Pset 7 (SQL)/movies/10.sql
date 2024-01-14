-- list the names of all people who have directed a movie that received a rating of at least 9.0.
-- output a table with a single column for the name of each person.
-- If a person directed more than one movie that received a rating of at least 9.0, they should only appear in your results once.


-- Find the names of people with those people IDs.
SELECT name
FROM people
WHERE id IN
(
    -- Find the IDs of people who directed those movies
    SELECT person_id
    FROM directors
    WHERE movie_id IN (

        -- Find the IDs of movies with at least a 9.0 rating
        SELECT movie_id
        FROM ratings
        WHERE rating >= 9.0
    )
);
