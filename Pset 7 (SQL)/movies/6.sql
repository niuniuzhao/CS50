-- the average rating of all movies released in 2012
-- output a table with a single column and a single row containing the average rating.

SELECT AVG(rating)
FROM ratings
JOIN movies ON movies.id = ratings.movie_id
WHERE year = 2012;
