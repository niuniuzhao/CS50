-- list the titles of all movies in which both Bradley Cooper and Jennifer Lawrence starred.
-- output a table with a single column for the title of each movie.


-- Find titles for the overlap of the IDs of movies associated with BC and JL
SELECT title
FROM movies
WHERE id IN
(
    -- Find the IDs of movies associated with Bradley Cooper’s ID
    SELECT movie_id
    FROM stars
    WHERE person_id = (
        -- Find the ID of Bradley Cooper
        SELECT id
        FROM people
        WHERE name = 'Bradley Cooper'
    )
)
AND id IN
(
    -- Find the IDs of movies associated with Jennifer Lawrence’s ID
    SELECT movie_id
    FROM stars
    WHERE person_id = (
        -- Find the ID of Jennifer Lawrence
        SELECT id
        FROM people
        WHERE name = 'Jennifer Lawrence'
    )
);
