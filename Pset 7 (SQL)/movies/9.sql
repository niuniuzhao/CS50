-- list the names of all people who starred in a movie released in 2004
-- ordered by birth year.
-- output a table with a single column for the name of each person.
-- People with the same birth year may be listed in any order.
-- If a person appeared in more than one movie in 2004, they should only appear in your results once.

-- Find the names of people with those people IDs
SELECT name
FROM people
WHERE id IN
(
    -- Find the IDs of people who starred in those movies
    SELECT person_id
    FROM stars
    WHERE movie_id IN (

        -- Find the IDs of movies released in 2004
        SELECT id
        FROM movies
        WHERE year = 2004
    )
)
ORDER BY birth;
